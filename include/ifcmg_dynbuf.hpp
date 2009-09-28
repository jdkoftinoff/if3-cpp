/*
  
The IF Contextual Media Group Kernel Version 3
Source Code

Written By Jeff Koftinoff <jeffk@contextualmediagroup.com>
Copyright (c) 1995-2005
By Contextual Media Group, Inc.
http://www.contextualmediagroup.com/

ALL RIGHTS RESERVED.

*/
#ifndef IFCMG_DYNBUF_H
#define IFCMG_DYNBUF_H

#include "ifcmg_world.hpp"
#include "ifcmg_string.hpp"
#include "ifcmg_buf.hpp"

namespace ifcmg
{
  class dynbuf_t : public buf_t
  {
  public:
    
    // default constructor
    dynbuf_t()   
      :
      buf_t( newnothrow  unsigned char[64], 0, 64 )
    {
      markup();
    }
    
    
    explicit dynbuf_t( int max_sz )  
      :
      buf_t(newnothrow unsigned char[max_sz], 0, max_sz)
    {
      markup();
    }
    
    
    // copy constructor
    dynbuf_t( const dynbuf_t &a ) 
      :
      buf_t( newnothrow unsigned char[a.get_buf_length()], a.get_data_length(), a.get_buf_length() )
    {	
      memcpy( data, a.get_data(), data_len );
    }
    
    dynbuf_t( const buf_t &a ) 
      :
      buf_t( newnothrow unsigned char[a.get_buf_length()], a.get_data_length(), a.get_buf_length() )
    {	
      memcpy( data, a.get_data(), data_len );
    }
    
    
    // copy function operator
    const dynbuf_t & operator = ( const dynbuf_t &a ) 
    {
      if( &a == this )
        return *this;
      
      clear(a.get_data_length());
      markup();				
      append_from_data( a.get_data(), a.get_data_length() );
      
      return *this;
    }
    
    const dynbuf_t & operator = ( const buf_t &a ) 
    {
      if( &a == this )
        return *this;
      
      clear(a.get_data_length());
      markup();				
      append_from_data( a.get_data(), a.get_data_length() );
      
      return *this;
    }
    
    
    explicit dynbuf_t( const char *filename )   
      :
      buf_t( newnothrow unsigned char[64], 0, 64 )
    {
      markup();				
      append_from_file( filename );
    }
    
    explicit dynbuf_t( const filename_t &filename )
      :
      buf_t( newnothrow unsigned char[64], 0, 64 )	
    {
      markup();				
      append_from_file( filename );	
    }
    
    
    // destructor
    ~dynbuf_t() 
    {
      if(data) delete [] data;
    }
    
    // expand size to len
    bool expand( int len );	
    
    // shrink size to len
    bool shrink( int len );
    
    // reallocate buffer to len size	
    bool resize( int len );
    
  private:
    void markup();
  };
}

#endif
