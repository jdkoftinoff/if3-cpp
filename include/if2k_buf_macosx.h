/*
  
The Internet Filter IF2K Version 2
Source Code

Written By Jeff Koftinoff <jeffk@jdkoftinoff.com>
Copyright (c) 1995-2005
By Turner and Sons Productions Ltd.
http://www.internetfilter.com/

ALL RIGHTS RESERVED.

*/
#ifndef _IF2K_BUF_MACOSX_H
#define _IF2K_BUF_MACOSX_H

#ifdef __OBJC__

#import <Cocoa/Cocoa.h>

namespace if2k
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
