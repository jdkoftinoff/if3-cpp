#include "if3_world_precompiled.hpp"
#include "if3_daemon.hpp"
#include "if3_syslog.hpp"

namespace if3
{
  namespace daemon
  {
#if defined(IF3_CONFIG_POSIX)
    children_list_t running_children;

    static string_t daemon_pid_file_name;

    volatile int32_t num_sigterm = 0;
    volatile int32_t num_sighup = 0;
    volatile int32_t num_sigint = 0;
    volatile int32_t num_sigchld = 0;
    volatile int32_t num_sigquit = 0;
    volatile int32_t num_sigalrm = 0;


    void drop_root(
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
                   filename_t home_dir,
                   filename_t pid_file,
                   string_t new_uid,
                   filename_t log_file
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
            _exit(0);
            break;
        };
      }
      logger = new system_logger_t( identity, !real_daemon, log_file );

      if( new_uid.length()>0 )
      {
        drop_root(new_uid);
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
      prepare_child_start();
    }

    void handle_sigterm( int )
    {
      num_sigterm++;
    }

    void handle_sigint( int )
    {
      num_sigint++;
    }

    void handle_sighup( int )
    {
      num_sighup++;
    }

    void handle_sigchld( int )
    {
      num_sigchld++;
    }

    void handle_sigquit( int )
    {
      num_sigquit++;
    }

    void handle_sigalrm( int )
    {
      num_sigalrm++;
    }

    void setup_sigterm()
    {
      struct sigaction act;

      num_sigterm = 0;
      act.sa_handler = handle_sigterm;
      sigemptyset(&act.sa_mask);
      if( sigaction(SIGTERM,&act,NULL)<0)
      {
        throw( std::runtime_error("sigaction failed for sigterm") );
      }
      siginterrupt(SIGTERM,1);
    }

    void setup_sigchld()
    {
      struct sigaction act;

      num_sigchld = 0;

      act.sa_handler = handle_sigchld;
      sigemptyset(&act.sa_mask);
      act.sa_flags|=SA_NOCLDSTOP | SA_RESTART ;
      if( sigaction(SIGCHLD,&act,NULL)<0)
      {
        throw( std::runtime_error("sigaction failed for sigchld") );
      }
      siginterrupt(SIGCHLD,1);
    }


    void setup_sigint()
    {
      struct sigaction act;

      num_sigint = 0;
      act.sa_handler = handle_sigint;
      sigemptyset(&act.sa_mask);
      if( sigaction(SIGINT,&act,NULL)<0)
      {
        throw( std::runtime_error("sigaction failed for sigint") );
      }
      siginterrupt(SIGINT,1);
    }

    void setup_sighup()
    {
      struct sigaction act;

      num_sighup = 0;
      act.sa_handler = handle_sighup;
      sigemptyset(&act.sa_mask);
      if( sigaction(SIGHUP,&act,NULL)<0)
      {
        throw( std::runtime_error("sigaction failed for sighup") );
      }
      siginterrupt(SIGHUP,1);
    }

    void setup_sigquit()
    {
      struct sigaction act;

      num_sigquit = 0;
      act.sa_handler = handle_sigquit;
      sigemptyset(&act.sa_mask);
      if( sigaction(SIGQUIT,&act,NULL)<0)
      {
        throw( std::runtime_error("sigaction failed for sigquit") );
      }
      siginterrupt(SIGQUIT,1);
    }

    void setup_sigalrm()
    {
      struct sigaction act;

      num_sighup = 0;
      act.sa_handler = handle_sigalrm;
      sigemptyset(&act.sa_mask);
      if( sigaction(SIGALRM,&act,NULL)<0)
      {
        throw( std::runtime_error("sigaction failed for sigalrm") );
      }
      siginterrupt(SIGALRM,1);
    }

    void register_pid( pid_t pid )
    {
      running_children.push_back( pid );
    }

    void unregister_pid( pid_t pid )
    {
      bool did_it=false;
      for(
          children_list_t::iterator i = running_children.begin();
          i != running_children.end();
          ++i
           )
      {
        if( *i == pid )
        {
          running_children.erase(i);
          logger->log_info( "pid %d unregistered", (int)pid );
          did_it=true;
          break;
        }
      }
      if( !did_it )
      {
        logger->log_error( "tried to unregister pid %d but was unrecognized", (int)pid );
      }
    }

    void process_sigchlds()
    {
      int status;
      pid_t pid;

      while( (pid=waitpid(-1,&status,WNOHANG))>0 )
      {
        unregister_pid(pid);
      }
    }

    void send_signal_to_children( int sig )
    {
      for(
          children_list_t::const_iterator i = running_children.begin();
          i != running_children.end();
          ++i
           )
      {
        pid_t pid = *i;
        if( pid>0 )
        {
          if( kill( pid, sig) == 0 )
          {
            logger->log_info("Sent signal %d to pid %d", sig, pid );
          }
          else
          {
            logger->log_error(
                              "Error sending signal %d to pid %d: %s",
                              sig,
                              pid,
                              strerror(errno)
                              );
          }
        }
      }
    }

    void prepare_child_start()
    {
      running_children.clear();
      setup_sigterm();
      setup_sigchld();
      setup_sighup();
      setup_sigint();
      setup_sigquit();
      setup_sigalrm();
    }

    pid_t do_fork()
    {
      pid_t i = fork();
      if( i>0 )
      {
        register_pid(i);
      }
      if( i == 0 )
      {
        // in child, clean up some things
        prepare_child_start();
      }
      if( i<0 )
      {
        throw std::runtime_error("fork failed");
      }
      return i;
    }

    void end()
    {
      if( daemon_pid_file_name.length()>0 )
      {
        if( unlink( daemon_pid_file_name.c_str() )<0 )
        {
          if( logger )
          {
            logger->log_error("remove pid file failed: %s", strerror(errno));
          }
        }
      }

      if( logger )
      {
        delete logger;
        logger = 0;
      }
    }

    void default_on_sigterm()
    {
      send_signal_to_children(SIGTERM);
    }

    void default_on_sigchld()
    {
    }

    void default_on_sigint()
    {
      send_signal_to_children(SIGINT);
    }

    void default_on_sighup()
    {
      send_signal_to_children(SIGHUP);
    }

    void default_on_sigquit()
    {
      send_signal_to_children(SIGQUIT);
    }

    void default_on_sigalrm()
    {
      send_signal_to_children(SIGALRM);
    }


    void wait_for_children_to_finish(
                                     const char *title,
                                     signals_observer_t &signals_observer
                                     )
    {

      while( !running_children.empty() )
      {
        sleep(1);

        if( signals_observer.poll() )
        {
#if 1
          logger->log_info(
              "%s: num_children=%d sigchld=%d sighup=%d sigint=%d sigalrm=%d",
              title,
              running_children.size(),
              num_sigchld, num_sighup, num_sigint, num_sigalrm
          );

          for( children_list_t::const_iterator i = running_children.begin();
              i!=running_children.end();
              i++ )
          {
            logger->log_info( "%s: child running %d", title, (int)*i );
          }
#endif
        }
      }

      logger->log_info( "%s: all sub processes ended", title );
    }

#endif
  }
}
