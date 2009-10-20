/*  
 The IF Contextual Media Group Kernel Version 3
 Source Code
 
 Written By Jeff Koftinoff <jeffk@contextualmediagroup.com>
 Copyright (c) 1995-2005
 By Contextual Media Group, Inc.
 http://www.contextualmediagroup.com/
 
 ALL RIGHTS RESERVED.
 
 */
#ifndef IFCMG_DAEMON_HPP
#define IFCMG_DAEMON_HPP

#include "ifcmg_world.hpp"
#include "ifcmg_string.hpp"
#include "ifcmg_util.hpp"
#include "ifcmg_syslog.hpp"

namespace ifcmg 
{
#if defined(IFCMG_CONFIG_POSIX)
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
