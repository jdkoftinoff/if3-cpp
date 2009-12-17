/*  
 The Internet Filter Version 3 Kernel Version 3
 Source Code
 
 Written By Jeff Koftinoff <jeffk@internetfilter.com>
 Copyright (c) 1995-2005
 By Turner and Sons Productions, Inc.
 http://www.internetfilter.com/
 
 ALL RIGHTS RESERVED.
 
 */
#ifndef IF3_DAEMON_HPP
#define IF3_DAEMON_HPP

#include "if3_world.hpp"
#include "if3_string.hpp"
#include "if3_util.hpp"
#include "if3_syslog.hpp"

namespace if3 
{
#if defined(IF3_CONFIG_POSIX)
  void daemon_drop_root( 
                        string_t uid_name
                        );  
  
  void daemonize(
                 bool real_daemon,
                 string_t identity,
                 string_t home_dir,
                 string_t pid_file,
                 string_t new_uid
                 );
  
  void daemon_end();
  
  
  pid_t daemon_fork();
#endif  
}

#endif
