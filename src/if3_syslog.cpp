#include "if3_world_precompiled.hpp"
#include "if3_syslog.hpp"

namespace if3
{
#if defined(IF3_CONFIG_POSIX)

  system_logger_t *logger = 0;

  system_logger_t::system_logger_t(
                                   string_t identity,
                                   bool to_stderr,
                                   filename_t dest_filename
                                   )
  :
    m_debug_enabled(false),
    m_local_file_handle(0)
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

    if( dest_filename.length()>0 )
    {
      m_local_file_handle = fopen(dest_filename.c_str(), "a");

      if( m_local_file_handle == 0 )
      {
        perror( "opening log file failed" );
        abort();
      }
    }

  }

  system_logger_t::~system_logger_t()
  {
    closelog();
    if( m_local_file_handle!=0 )
    {
      ::fclose( m_local_file_handle );
      m_local_file_handle=0;
    }
  }

  void system_logger_t::log_alert( const char *fmt, ... )
  {
    va_list args;
    va_start(args, fmt);
    vsyslog(LOG_ALERT, fmt, args);
    if( m_local_file_handle )
    {
      fprintf( m_local_file_handle, "(%d) ALERT: ", getpid() );
      vfprintf( m_local_file_handle, fmt, args );
      fprintf( m_local_file_handle, "\n" );
      fflush( m_local_file_handle );
    }
    va_end(args);
  }

  void system_logger_t::log_critical( const char *fmt, ... )
  {
    va_list args;
    va_start(args, fmt);
    vsyslog(LOG_CRIT, fmt, args);
    if( m_local_file_handle )
    {
      fprintf( m_local_file_handle, "(%d) CRITICAL: ", getpid() );
      vfprintf( m_local_file_handle, fmt, args );
      fprintf( m_local_file_handle, "\n" );
      fflush( m_local_file_handle );
    }
    va_end(args);
  }

  void system_logger_t::log_error( const char *fmt, ... )
  {
    va_list args;
    va_start(args, fmt);
    vsyslog(LOG_ERR, fmt, args);
    if( m_local_file_handle )
    {
      fprintf( m_local_file_handle, "(%d) ERROR: ", getpid() );
      vfprintf( m_local_file_handle, fmt, args );
      fprintf( m_local_file_handle, "\n" );
      fflush( m_local_file_handle );
    }

    va_end(args);
  }

  void system_logger_t::log_warning( const char *fmt, ... )
  {
    va_list args;
    va_start(args, fmt);
    vsyslog(LOG_WARNING, fmt, args);
    if( m_local_file_handle )
    {
      fprintf( m_local_file_handle, "(%d) WARNING: ", getpid() );
      vfprintf( m_local_file_handle, fmt, args );
      fprintf( m_local_file_handle, "\n" );
      fflush( m_local_file_handle );
    }
    va_end(args);
  }

  void system_logger_t::log_notice( const char *fmt, ... )
  {
    va_list args;
    va_start(args, fmt);
    vsyslog(LOG_NOTICE, fmt, args);
    if( m_local_file_handle )
    {
      fprintf( m_local_file_handle, "(%d) NOTICE: ", getpid() );
      vfprintf( m_local_file_handle, fmt, args );
      fprintf( m_local_file_handle, "\n" );
      fflush( m_local_file_handle );
    }
    va_end(args);
  }

  void system_logger_t::log_info( const char *fmt, ... )
  {
    va_list args;
    va_start(args, fmt);
    vsyslog(LOG_INFO, fmt, args);
    if( m_local_file_handle )
    {
      fprintf( m_local_file_handle, "(%d) INFO: ", getpid() );
      vfprintf( m_local_file_handle, fmt, args );
      fprintf( m_local_file_handle, "\n" );
      fflush( m_local_file_handle );
    }

    va_end(args);
  }

  void system_logger_t::log_debug( const char *fmt, ... )
  {
    if( m_debug_enabled )
    {
      va_list args;
      va_start(args, fmt);
      vsyslog(LOG_DEBUG, fmt, args);
      if( m_local_file_handle )
      {
        fprintf( m_local_file_handle, "(%d) DEBUG: ", getpid() );
        vfprintf( m_local_file_handle, fmt, args );
        fprintf( m_local_file_handle, "\n" );
        fflush( m_local_file_handle );
      }

      va_end(args);
    }
  }

#endif
}

