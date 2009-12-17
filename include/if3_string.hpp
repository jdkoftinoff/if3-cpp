/*
  
The IF Contextual Media Group Kernel Version 3
Source Code

Written By Jeff Koftinoff <jeffk@contextualmediagroup.com>
Copyright (c) 1995-2005
By Contextual Media Group, Inc.
http://www.contextualmediagroup.com/

ALL RIGHTS RESERVED.

*/
#ifndef if3_STRING_HPP
#define if3_STRING_HPP

#include <string>

namespace if3
{
  typedef std::string string_t;
  typedef std::string filename_t;
  typedef std::string string_url_t;
  typedef std::string hostname_t;
  
  
  template <class T> const T & form( T &s, const char *fmt, ... )
  {
    char data[4096];
    data[sizeof(data)-1]='\0';
    data[0] ='\0';
    
    va_list l;
    va_start( l, fmt );
    
#if defined(if3_CONFIG_WIN32)	
    _vsnprintf( data, sizeof(data)-1, fmt, l );    
#else
    vsprintf( data, fmt, l );
#endif	
    s = data;
    va_end(l);
    
    return s;
  }
}

#endif
