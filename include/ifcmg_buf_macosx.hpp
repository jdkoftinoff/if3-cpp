/*
  
The IF Contextual Media Group Kernel Version 3
Source Code

Written By Jeff Koftinoff <jeffk@contextualmediagroup.com>
Copyright (c) 1995-2005
By Contextual Media Group, Inc.
http://www.contextualmediagroup.com/

ALL RIGHTS RESERVED.

*/
#ifndef IFCMG_BUF_MACOSX_H
#define IFCMG_BUF_MACOSX_H

#ifdef __OBJC__

#import <Cocoa/Cocoa.h>

namespace ifcmg
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
