/*
  
The IF Contextual Media Group Kernel Version 3
Source Code

Written By Jeff Koftinoff <jeffk@contextualmediagroup.com>
Copyright (c) 1995-2005
By Contextual Media Group, Inc.
http://www.contextualmediagroup.com/

ALL RIGHTS RESERVED.

*/
#ifndef IFCMG_BUF_HPP
#define IFCMG_BUF_HPP

#include "ifcmg_world.hpp"
#include "ifcmg_string.hpp"

namespace ifcmg
{  
  class buf_t
  {
  protected:
    buf_t() {} // non initializing constructor only for subclasses
  private:
    const buf_t & operator = ( const buf_t &a ); // not allowed	
    buf_t( const buf_t &); // not allowed
    
  public:
    
    buf_t( void *data_, int data_len_, int buf_len_ )
      : 
      data_len( data_len_ ),
      buf_len( buf_len_ ),
      data( (unsigned char *)data_ )	
    {		
    }
    
    // destructor
    virtual ~buf_t() 
    {
    }
    
    bool copy( const buf_t &o )
    {
      bool r=false;
      clear();
      if( resize( o.get_data_length()+1 ) )
      {
        r=append_from_data( o.get_data(), o.get_data_length() );
      }
      return r;
    }
    
    // clear but dont resize
    virtual void clear() 
    {
      data_len=0;
    }
    
		
    // clear and resize to len
    virtual void clear( int len ) 
    {
      data_len=0;
      shrink(1);
    }
    
		
    // expand size to len
    virtual bool expand( int len )
    {
      return false;	
    }
    
    
    // shrink size to len
    virtual bool shrink( int len )
    {
      return false;	
    }
    
   	
    // reallocate buffer to len size	
    virtual bool resize( int len )
    {
      return false;	
    }
    
    
    virtual void fill()
    {
	    data_len=buf_len;    
    }
		
    // get raw ptr to data
    unsigned char * get_data() 
    {	
      return data;
    }
    
    const unsigned char * get_data() const 
    {
      return data;
    }
    
    // get the current size of the buffer
    int get_buf_length() const
    {
      return buf_len;
    }
		
    // get length of data
    int get_data_length() const 
    {
      return data_len;
    }
    
    // manually set the data length - only for use by functions dealing with the buf directly
    void set_data_length( int len )
    {
      data_len = len;
    }
    
		
    // append data
    virtual bool append_from_data( const unsigned char *b, int len );
    virtual bool append_from_data( const char *b, int len ) 
    {
      return append_from_data( (const unsigned char *)b, len );
    }
    
    virtual bool append_from_file( const char *file_name, int start=0, int len=-1 );
    virtual bool append_from_file( const filename_t &file_name, int start=0, int len=-1 )
    {
	    return append_from_file( file_name.c_str(), start, len );
    }
    virtual bool append_from_file( FILE *f, int start=0, int len=-1, bool allow_partial=false );	
    
    virtual bool append_from_stream( FILE *f );
    
    virtual bool append_from_string( const char *str );
    virtual bool append_from_string( const string_t &s )
    {
	    return append_from_string( s.c_str() );
    }
    virtual bool append_from_buf( const buf_t &other, int start=0, int len=-1 );
    virtual bool append_form( const char *fmt, ... );
    
    
    // extract data
    virtual bool extract_to_file( FILE *f, int start=0, int len=-1 ) const;
    virtual bool extract_to_stream( FILE *f ) const;
    virtual bool extract_to_file( const char *file_name, int start=0, int len=-1 ) const;	
    virtual bool extract_to_file( const filename_t &file_name, int start=0, int len=-1 ) const
    {
	    return extract_to_file( file_name.c_str(), start, len );
    }
    
    virtual bool extract_to_data( unsigned char *b, int start, int len ) const;
    virtual bool extract_to_data( char *b, int start, int len ) const
    {
      return extract_to_data( (unsigned char *)b, start, len );
    }
    
    virtual int extract_to_string( 
      char *str, 
      int start, 
      int max_len, 
      const char *separators="\n", 
      const char *ignores="\r"
      ) const;
    
    virtual int extract_to_string( 
      string_t &str,
      int start,  
      int max_len,
      const char *separators="\n", 
      const char *ignores="\r"
      ) const;
    
  protected:	
    
    int data_len;
    int buf_len;
    unsigned char *data;	
  };
  
  template <int SZ>
  class staticbuf_t : public buf_t
  {
  public:
    inline staticbuf_t() : buf_t( staticbuf, 0, SZ )
    {
    }
    
  private:
    unsigned char staticbuf[ SZ ];
  };
  
}

#endif
