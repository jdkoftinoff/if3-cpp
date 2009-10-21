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
  
#ifdef _WIN32
  typedef SOCKET_HANDLE socket_handle_t;
#else
  typedef int socket_handle_t;
#endif
  
  void net_init();
  
  inline string_t to_string( const ::sockaddr *a, size_t a_len )
  {
    char buf1[1024] = "";
    char buf1_serv[128] = "";
    ::getnameinfo ( a, a_len, buf1, sizeof(buf1)-1, buf1_serv, sizeof(buf1_serv)-1, 0 );

    buf1[1023] = '\0';
    buf1_serv[127] = '\0';

    char buf2[128] = "";
    char buf2_serv[128] = "";
    ::getnameinfo( a, a_len, buf2, sizeof(buf2)-1, buf2_serv, sizeof(buf2_serv)-1, NI_NUMERICHOST|NI_NUMERICSERV);
    
    buf2[127] = '\0';
    buf2_serv[127] = '\0';
    
    string_t result = 
      string_t("[") + string_t(buf1) + "]:" + buf1_serv 
      + " ( [" + string_t(buf2) + "]:" + buf2_serv + " )";
    return result;
  }

  inline string_t to_string( const ::sockaddr_storage *a, size_t a_len )
  {
    return to_string( (const ::sockaddr *)a, a_len );
  }
    
  inline string_t to_string( const ::addrinfo &a )
  {
    return to_string( a.ai_addr, a.ai_addrlen );
  }
  
  inline
  std::ostream & operator << ( std::ostream& o, const ::addrinfo &v )
  {
    o << to_string(v);
    return o;
  }
  
  
  class net_address_t
  {
  public:
    net_address_t( const char *hostname, const char *port )
    {
      ::addrinfo hints;
      ::memset(&hints,'\0',sizeof(hints) );
      hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG; // | AI_NUMERICSERV;
      hints.ai_socktype = SOCK_STREAM;
      int e = ::getaddrinfo( hostname, port, &hints,&m_ai );
      if( e!=0 )
      {
        throw std::runtime_error( "getaddrinfo: " + string_t( strerror(e) ) );
      }    
    }
    
    class const_iterator
    {
    public:
      const_iterator( const const_iterator &o ) : m_v( o.m_v ) {}
      
      const_iterator( ::addrinfo *v ) : m_v(v) {}
      
      const ::addrinfo & operator * () const { return *m_v; }
      
      const ::addrinfo * operator -> () const { return m_v; }
      
      const_iterator & operator ++ ()
      {
        if( m_v != 0 )
          m_v = m_v->ai_next;
        return *this;
      }
      
      const_iterator operator ++ (int)
      {
        const_iterator prev( *this );
        if( m_v !=0 )
          m_v = m_v->ai_next;
        return prev;
      }
      
      bool operator == ( const const_iterator &o ) const
      {
        return m_v != o.m_v;
      }
      
      bool operator != ( const const_iterator &o ) const
      {
        return m_v != o.m_v;
      }
      
    private:
      ::addrinfo *m_v;
    };
    
    const_iterator begin() const { return const_iterator( m_ai ); }
    const_iterator end() const { return const_iterator(0); }
    
    ~net_address_t()
    {
      ::freeaddrinfo( m_ai );
    }
    
    inline friend std::ostream & operator << ( std::ostream& o, const net_address_t &v )
    {
      o << "{\n";
      for( net_address_t::const_iterator i= v.begin(); i!= v.end(); ++i )      
      {
        o << *i << "\n";
      }
      o << "}\n";
      return o;
    }
    
  private:
    ::addrinfo *m_ai;
  };
  
}

#endif
