/*  
 The IF Contextual Media Group Kernel Version 3
 Source Code
 
 Written By Jeff Koftinoff <jeffk@contextualmediagroup.com>
 Copyright (c) 1995-2005
 By Contextual Media Group, Inc.
 http://www.contextualmediagroup.com/
 
 ALL RIGHTS RESERVED.
 
 */
#ifndef IFCMG_NET_HPP
#define IFCMG_NET_HPP

#include "ifcmg_world.hpp"
#include "ifcmg_string.hpp"
#include "ifcmg_util.hpp"
#include "ifcmg_daemon.hpp"

namespace ifcmg 
{
  typedef int socklen_t;
  
#ifdef _WIN32
  typedef SOCKET_HANDLE socket_handle_t;
#else
  typedef int socket_handle_t;
#endif

  
  void net_init();
  
  class net_addr_t 
  {
  public:
    
    friend
    std::ostream & operator << ( std::ostream &s, net_addr_t const &v )
    {
      char buf[256];
      getnameinfo( 
                  (sockaddr *)&v.m_addr, 
                  v.m_len, 
                  buf, 
                  sizeof(buf), 
                  NULL, 
                  0, 
                  NI_NUMERICHOST
                  );
      s << buf;
      return s;
    }
    
    struct sockaddr_storage m_addr;
    socklen_t m_len;    
  };
  
  class net_addr_list_t 
  {
  public:
    net_addr_list_t( 
                    const char *host, 
                    const char *port
                    )
    {
      m_addr = 0;
      struct addrinfo hints;
      memset(&hints,'\0',sizeof(hints) );
      hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG;
      hints.ai_socktype = SOCK_STREAM;
      int e = getaddrinfo( host,port,&hints,&m_addr );
      if(e<0)
      {
        m_addr=0;
      }
    }
    
    struct addrinfo *m_addr;
  }; 
}

#endif
