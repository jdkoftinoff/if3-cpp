/*  
 The IF Contextual Media Group Kernel Version 3
 Source Code
 
 Written By Jeff Koftinoff <jeffk@contextualmediagroup.com>
 Copyright (c) 1995-2005
 By Contextual Media Group, Inc.
 http://www.contextualmediagroup.com/
 
 ALL RIGHTS RESERVED.
 
 */
#ifndef if3_SYSLOG_HPP
#define if3_SYSLOG_HPP

#include "if3_world.hpp"
#include "if3_string.hpp"

namespace if3
{
#if defined(if3_CONFIG_POSIX)
  class system_logger_t
  {
  public:

    system_logger_t( 
                    string_t identity,
                    bool to_stderr
                    );
    ~system_logger_t();
    
    void enable_debug( bool f=true )
    {
      m_debug_enabled = f;
    }
    
    bool is_debug_enabled() const
    {
      return m_debug_enabled;
    }
    
    
    void log_alert( const char *fmt, ... );
    void log_critical( const char *fmt, ... );
    void log_error( const char *fmt, ... );
    void log_warning( const char *fmt, ... );
    void log_notice( const char *fmt, ... );
    void log_info( const char *fmt, ... );
    void log_debug( const char *fmt, ... );

  private:
    void do_log_debug( string_t s );
    
    bool m_debug_enabled;
  };
  
  extern system_logger_t *logger;
#endif
}


#endif
