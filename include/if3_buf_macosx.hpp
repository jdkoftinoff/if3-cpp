/*
  
The Internet Filter Version 3 Kernel Version 3
Source Code

Written By Jeff Koftinoff <jeffk@internetfilter.com>
Copyright (c) 1995-2005
By Turner and Sons Productions, Inc.
http://www.internetfilter.com/

ALL RIGHTS RESERVED.

*/
#ifndef IF3_BUF_MACOSX_HPP
#define IF3_BUF_MACOSX_HPP

#ifdef __OBJC__

#import <Cocoa/Cocoa.h>

namespace if3
{
  
  inline void buf_from_NSString( buf_t &buf, NSString *s )
  {
    buf.clear();
    buf.append_from_string( [s lossyCString] );
  }
  
  inline NSString * buf_to_NSString( buf_t &buf )
  {
    return [NSString stringWithCString: (const char*)buf.get_data() 
            length: buf.get_data_length() ];
  }
}

#endif
#endif
