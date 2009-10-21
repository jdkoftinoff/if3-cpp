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
#include "ifcmg_httpd.hpp"

namespace ifcmg
{

#if defined(IFCMG_CONFIG_POSIX)
  void httpd_fork_server_t::run()
  {
    for( 
        net_address_t::const_iterator i = m_bind_address.begin();
        i!=m_bind_address.end();
        ++i
        )
    {
      logger->log_info(
                       "about to fork for binding to %s", 
                       to_string(*i).c_str() 
                       );
      
      if( daemon_fork()==0 )
      {
        run_on_address( i );
        exit(0);
      }
    }
  }
  
  void httpd_fork_server_t::run_on_address( net_address_t::const_iterator &a )
  {
    logger->log_info("Starting run on %s", to_string(*a).c_str() );
    
    socket_handle_t sock = socket(a->ai_family, a->ai_socktype, a->ai_protocol );
    if( sock==-1 )
    {
      throw std::runtime_error("unable to create socket");
    }

    const char opt = 1;
    setsockopt( sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt) );

    if( bind ( 
              sock,
              a->ai_addr, 
              a->ai_addrlen 
              ) != 0 )
    {
      if( errno != EADDRINUSE )
      {
        throw std::runtime_error( string_t("bind: ") + string_t( strerror(errno) ) );
      }
      close(sock);      
    }    
    
    if( listen( sock,  SOMAXCONN) != 0 )
    {
      throw std::runtime_error( string_t("listen: ") + string_t( strerror(errno) ) );
    }
    
    while(true)
    {
      struct sockaddr_storage rem;
      socklen_t remlen = sizeof( rem );
      socket_handle_t fd = accept( sock, 
                                  (struct sockaddr *)&rem, 
                                  (socklen_t *)&remlen
                                  );
      logger->log_info("accepted connection on socket %d", fd );
      
      m_session_handler->run( fd );
      
      close(fd);      
    }      
      
    logger->log_info("Ending run on %s", to_string(*a).c_str() );
    
  }
  
#endif
}
