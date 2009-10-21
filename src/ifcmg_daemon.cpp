/*
 
 The IF Contextual Media Group Kernel Version 3
 Source Code
 
 Written By Jeff Koftinoff <jeffk@contextualmediagroup.com>
 Copyright (c) 1995-2005
 By Contextual Media Group, Inc.
 http://www.contextualmediagroup.com/
 
 ALL RIGHTS RESERVED.
 
 */
#include "ifcmg_world_precompiled.hpp"
#include "ifcmg_daemon.hpp"
#include "ifcmg_syslog.hpp"

namespace ifcmg
{
#if defined(IFCMG_CONFIG_POSIX)    
  
  static string_t daemon_pid_file_name;
  
  void daemon_drop_root( 
                        string_t uid_name  
                        )
  {
    uid_t new_uid=0;
    uid_t new_gid=0;
    
    if( getuid()==0 )
    {
        // lookup uid and gid by user name
      struct passwd *pw = getpwnam( uid_name.c_str() );
      if( pw )
      {
        new_uid = pw->pw_uid;
        new_gid = pw->pw_gid;
      }
      else 
      {
          // that failed, maybe uid is actually a number
        new_uid = std::strtol( uid_name.c_str(), 0, 10 );
        pw = getpwuid( new_uid );
        if( pw )
          new_gid = pw->pw_gid;
      }
      
      if( setegid(new_gid)<0 ||
          seteuid(new_uid)<0 )
      {
        perror("error setting euid/egid");
        exit(1);
      }
    }
  }
  
  void daemonize(
                 bool real_daemon,
                 string_t identity,
                 string_t home_dir,
                 string_t pid_file,
                 string_t new_uid
                 )
  {
    daemon_pid_file_name = pid_file;
    
    if( home_dir.length()>0 )
    {
      mkdir(home_dir.c_str(),0750);      
      chdir(home_dir.c_str());
    }
    
    if( real_daemon )
    {
      switch(fork())
      {
        case -1:
          perror( "fork() failed" );
          exit(1);
        case 0:
          break;
        default:          
          exit(0);
          break;
      };      
    }
    logger = new system_logger_t( identity, !real_daemon );
    
    if( new_uid.length()>0 )
    {
      daemon_drop_root(new_uid);
    }
    
    if( daemon_pid_file_name.length()>0 )
    {
      int pid_fd = open( daemon_pid_file_name.c_str(), O_CREAT | O_TRUNC | O_WRONLY | O_NOFOLLOW, 0640 );
      if( pid_fd>=0 )
      {
        char tmpbuf[64];
        sprintf( tmpbuf, "%ld\n", (long)getpid() );
        int len = strlen(tmpbuf);
        if( write( pid_fd, tmpbuf, len)!=len )
        {
          logger->log_critical( "Error writing pid file: %s", daemon_pid_file_name.c_str() );
        }
        close( pid_fd );        
      }
      else 
      {
        logger->log_critical( "Error creating pid file: %s", daemon_pid_file_name.c_str() );  
      }

    }
    
    umask(0);

    if( real_daemon )
    {
      close( STDIN_FILENO );
      close( STDOUT_FILENO );
      close( STDERR_FILENO );
      
      int fd = open("/dev/null", O_RDWR );
      dup2(fd, STDIN_FILENO);
      dup2(fd, STDOUT_FILENO);
      dup2(fd, STDERR_FILENO);
    }
  }
  
  
  pid_t daemon_fork()
  {
    pid_t i = fork();
    if( i == 0 )
    {
    }
    if( i<0 )
    {
      throw std::runtime_error("fork failed");
    }
    return i;
  }
  
  void daemon_end()
  {
    if( daemon_pid_file_name.length()>0 )
    {
      unlink( daemon_pid_file_name.c_str() );
    }
    
    delete logger;
  }

  
#endif
}
