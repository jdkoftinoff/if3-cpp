#ifndef IF3_SYSLOG_HPP
#define IF3_SYSLOG_HPP

#include "if3_world.hpp"
#include "if3_string.hpp"

namespace if3
{
#if defined(IF3_CONFIG_POSIX)
  class system_logger_t
  {
  public:

    system_logger_t(
                    string_t identity,
                    bool to_stderr,
                    filename_t dest_filename = filename_t("")
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

    FILE * m_local_file_handle;
  };

  extern system_logger_t *logger;
# if defined(IF3_CONFIG_DEBUG)
#  define IF3_LOG_DEBUG(...) logger->log_debug( __VA_ARGS__ )
# else
#  define IF3_LOG_DEBUG(...)
# endif
#else
# define IF3_LOG_DEBUG(...)
#endif
}


#endif
