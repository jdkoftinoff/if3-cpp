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
#include "ifcmg_syslog.hpp"

namespace ifcmg
{
#if defined(IFCMG_CONFIG_POSIX)
  
  system_logger_t *logger = 0;
  
  system_logger_t::system_logger_t( 
                                   string_t identity,
                                   bool to_stderr
                                   )
  : 
  m_debug_enabled(false)
  {
    int logopt = 0;
    
      // syslog needs static buffer for identity
    static char ident_buffer[2048];
    
    std::strncpy( ident_buffer, identity.c_str(), 2047 );
    ident_buffer[2047] = '\0';
    
    if( to_stderr )
    {
      logopt |= LOG_NDELAY | LOG_PERROR | LOG_PID;
    }
    else 
    {
      logopt |= LOG_NDELAY | LOG_CONS | LOG_PID;
    }
    
    openlog( 
            ident_buffer, 
            logopt,
            LOG_DAEMON
            );            
  }
  
  system_logger_t::~system_logger_t()
  {
    closelog();
  }
  
  void system_logger_t::log_alert( const char *fmt, ... )
  {
    va_list args;    
    va_start(args, fmt);
    vsyslog(LOG_ALERT, fmt, args);
    va_end(args);    
  }
  
  void system_logger_t::log_critical( const char *fmt, ... )
  {
    va_list args;    
    va_start(args, fmt);
    vsyslog(LOG_CRIT, fmt, args);
    va_end(args);    
  }
  
  void system_logger_t::log_error( const char *fmt, ... )
  {
    va_list args;    
    va_start(args, fmt);
    vsyslog(LOG_ERR, fmt, args);
    va_end(args);    
  }
  
  void system_logger_t::log_warning( const char *fmt, ... )
  {
    va_list args;    
    va_start(args, fmt);
    vsyslog(LOG_WARNING, fmt, args);
    va_end(args);    
  }  
  
  void system_logger_t::log_notice( const char *fmt, ... )
  {
    va_list args;    
    va_start(args, fmt);
    vsyslog(LOG_NOTICE, fmt, args);
    va_end(args);    
  }
  
  void system_logger_t::log_info( const char *fmt, ... )
  {
    va_list args;    
    va_start(args, fmt);
    vsyslog(LOG_INFO, fmt, args);
    va_end(args);    
  }
  
  void system_logger_t::log_debug( const char *fmt, ... )
  {
    if( m_debug_enabled )
    {
      va_list args;    
      va_start(args, fmt);
      vsyslog(LOG_DEBUG, fmt, args);
      va_end(args);    
    }
  }
  
#endif  
}

