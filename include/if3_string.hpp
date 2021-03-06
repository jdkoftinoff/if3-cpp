/*
  
The Internet Filter Version 3 Kernel Version 3
Source Code

Written By Jeff Koftinoff <jeffk@internetfilter.com>
Copyright (c) 1995-2005
By Turner and Sons Productions, Inc.
http://www.internetfilter.com/

ALL RIGHTS RESERVED.

*/
#ifndef IF3_STRING_HPP
#define IF3_STRING_HPP

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
    
#if defined(IF3_CONFIG_WIN32)	
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
