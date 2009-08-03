/*
  
The Internet Filter IF2K Version 2
Source Code

Written By Jeff Koftinoff <jeffk@jdkoftinoff.com>
Copyright (c) 1995-2005
By Turner and Sons Productions Ltd.
http://www.internetfilter.com/

ALL RIGHTS RESERVED.

*/
#ifndef _IF2K_STRING_H
#define _IF2K_STRING_H

#include <string>

namespace if2k
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
    
#if defined(IF2K_CONFIG_WIN32)	
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
