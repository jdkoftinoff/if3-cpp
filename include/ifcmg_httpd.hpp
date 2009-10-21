/*  
 The IF Contextual Media Group Kernel Version 3
 Source Code
 
 Written By Jeff Koftinoff <jeffk@contextualmediagroup.com>
 Copyright (c) 1995-2005
 By Contextual Media Group, Inc.
 http://www.contextualmediagroup.com/
 
 ALL RIGHTS RESERVED.
 
 */
#ifndef IFCMG_HTTPD_HPP
#define IFCMG_HTTPD_HPP

#include "ifcmg_world.hpp"
#include "ifcmg_net.hpp"
#include "ifcmg_cgi.hpp"
#include "ifcmg_string.hpp"
#include "ifcmg_dynbuf.hpp"
#include "ifcmg_db.hpp"

namespace ifcmg 
{
  class httpd_session_base_t
  {
  public:
    httpd_session_base_t() {}
    virtual ~httpd_session_base_t() {}
    
    virtual void run( socket_handle_t s ) = 0;

  };
  
  class httpd_session_redirector_t : public httpd_session_base_t
  {
  public:
    httpd_session_redirector_t( db_t &db ) : m_db( db ) {}
    virtual ~httpd_session_redirector_t() {}
    
    void run( socket_handle_t s );
 
  private:
    
    db_t &m_db;
  };
  
#if defined(IFCMG_CONFIG_POSIX)
  class httpd_fork_server_t
  {
  public:
    httpd_fork_server_t( 
                        const char *bind_hostname,
                        const char *bind_port,
                        httpd_session_base_t *session_handler 
                        )
    :
    m_bind_address( bind_hostname, bind_port ),
    m_session_handler( session_handler )
    {
    }
    
    virtual ~httpd_fork_server_t()
    {
      delete m_session_handler;
    }
    
    void run();
    void run_on_address( net_address_t::const_iterator &a );
    
  private:
    net_address_t m_bind_address;
    httpd_session_base_t *m_session_handler;
  };

#endif
}

#endif

