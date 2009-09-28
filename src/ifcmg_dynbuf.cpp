/*
  
The IF Contextual Media Group Kernel Version 3
Source Code

Written By Jeff Koftinoff <jeffk@contextualmediagroup.com>
Copyright (c) 1995-2005
By Contextual Media Group, Inc.
http://www.contextualmediagroup.com/

ALL RIGHTS RESERVED.

*/
#include "ifcmg_world.h"
#include "ifcmg_dynbuf.h"

namespace ifcmg
{
  void dynbuf_t::markup()
  {
    if( buf_len - data_len>0 )
    {
      memset( data+data_len, 0xaa, buf_len-data_len );
    }
  }
  
  bool dynbuf_t::resize( int len ) 
  {
    if( len==0 )
      len=1;
    unsigned char *new_data=new unsigned char[len];
    if( new_data )
    {
      int minlen=len;
      if( buf_len<len ) minlen=buf_len;
      
      memcpy( new_data, data, minlen );
      
      unsigned char *old_data = data;
      data=new_data;
      buf_len=len;
      delete [] old_data;		
      return true;
    }
    else
    {
      return false;	
    }	   
  }
  
  
  
  bool dynbuf_t::expand( int len ) 
  {
    if( len>buf_len )
    {
      return resize( len );
    }
    return true;
  }
  
  
  
  bool dynbuf_t::shrink( int len )
  {
    if( len<buf_len )
    {
      return resize(len);
    }
    return true;
  }
  
}

