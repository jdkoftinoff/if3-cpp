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
#include "ifcmg_buf.h"

namespace ifcmg
{
  bool buf_t::append_from_string( const char *str )
  {
    int l=strlen(str);
    return append_from_data( (const unsigned char *)str, l );
  }
  
  
  
  bool buf_t::append_from_file(
    const char *file_name,
    int start,
    int len
    )
  {
    bool r=false;
    
    FILE *f=fopen( file_name, "rb" );
    
    if( f) 
    {
      r=append_from_file( f, start, len );
      fclose(f);
    }
    return r;
  }
  
  
  
  bool buf_t::append_from_stream( FILE *f )
  {
    
    while( !feof( f ) && !ferror(f))
    {
      if( buf_len-data_len<4096 )
      {
        if( !expand( buf_len * 3/2 ) ) // grow 150 %
          return false;
      }
      
      int len = fread( (data+data_len), 1, (buf_len-data_len), f );
      if( len>0 )
      {
        data_len+=len;
      }
    }
    return !ferror(f);
  }
  
  bool buf_t::append_from_file( 
    FILE *f, 
    int start, 
    int len,
    bool allow_partial
    ) 
  {
    int num_to_read=len;
    
    // seek only if we dont know how many bytes to read,
    
    if( num_to_read==-1 )
    {
      // save current pos
      int cur_pos = ftell(f);
      
      if( fseek( f, 0, SEEK_END ) <0 )
      {
        return false;	
      }
      
      if( start==-1 )
      {
        // calc how much from where we were 			
        num_to_read=ftell(f) - cur_pos;			
        
        // move pointer back to wherever we were
        fseek( f, cur_pos, SEEK_SET );
        
      }
      else
      {
        // calc how much from where we are supposed to be
        num_to_read=ftell(f) - start;			
      }
      
    }
    
    // if we know where to start from, then seek to that position
    
    if( start!=-1 )
    {		
      if( fseek( f, start, SEEK_SET )<0 )
      {
        return false;	
      }
    }
    
    
    // calculate how big our new buffer must be to append num_to_read bytes
    
    int new_len = data_len + num_to_read;
    
    // expand it
    if( !expand(new_len) )
    {
      // cant expand! fail!
      return false;	
    }
    
    // try read num_to_read bytes
    int num_read =fread( (data+data_len), 1, num_to_read, f );
    
    
    // fail if we didnt read all the bytes and we were supposed to
    
    if( num_read !=(int)num_to_read && !allow_partial )
    {
      return false;
    }
    
    // calculate our new data length
    
    data_len+=num_read;	
    return true;
  }
  
  
  
  bool buf_t::append_from_data( 
    const unsigned char *b, 
    int len 
    ) 
  {
    if( len+data_len > buf_len )
    {
      if( !expand(len+data_len+1024) )
      {
        return false;
      }			
    }
    
    memcpy( data+data_len, b, len );
    
    data_len+=len;
    return true;
  }
  
  
  
  bool buf_t::append_form( const char *fmt, ... )
  {
    bool ret=false;
    int added=0;
    
    // this is an ugly hack
    // 
    if( buf_len-data_len <4096 )
    {
      if( !expand(data_len+4096+1024) )
      {
        return false;
      }
    }	
    
    va_list l;
    va_start( l, fmt );
    
#if defined(IFCMG_CONFIG_WIN32)	
    added=vsprintf( (char *)data+data_len, fmt, l );
#else
    added=vsnprintf( (char *)data+data_len, buf_len-data_len, fmt, l );    
#endif	
    if( added!=-1 )
    {
      data_len += added;
      ret=true;
    }
    else
    {
      ret=false;	
    }
    va_end(l);
    
    return ret;
  }
  
  bool buf_t::append_from_buf( 
    const buf_t &other, 
    int start,
    int len
    ) 
  {
    // find how much data is available in other buf from start point.
    
    int actual_len = other.get_data_length() - start;
    
    if( actual_len < 0 )
    {
      // there is no data to copy - just return success.
      return true;
    }
    
    int copy_len = actual_len;
    
    // if he wants to copy a maximum number of bytes get the minimum of
    // actual length and len.
    
    if( len!=-1 )
    {
      if( len<actual_len )
        copy_len = len;
    }
    
    // ok, it is safe now, append it
    
    return append_from_data( other.get_data() + start, copy_len );
  } 
  
  bool buf_t::extract_to_stream( FILE *f ) const
  {
    return fwrite( data, 1, (size_t)data_len, f )==(size_t)data_len;
  }
  
  bool buf_t::extract_to_file( 
    FILE *f, 
    int start, 
    int len
    ) const 
  {
    int real_len=len;
    
    if( real_len==-1 )
    {
      real_len=data_len - start;	
    }
    
    if( real_len + start > data_len )
    {
      return false;
    }	
    
    if( real_len==0 )
    {
      return true;
    }
    if( fwrite( data+start, real_len, 1, f )!=1 )
    {
      return false;
    }
    
    return true;
  }
  
  
  
  bool buf_t::extract_to_file( 
    const char *file_name,
    int start,
    int len
    ) const
  {
    FILE *f=fopen( file_name, "wb" );
    
    if( f )
    {
      if( extract_to_file( f, start, len ) )
      {
        fclose(f);
        return true;
      }
      fclose(f);		
    }
    return false;
    
  }
  
  
  
  bool buf_t::extract_to_data( 
    unsigned char *buf,
    int start, 
    int len
    ) const 
  {
    int real_len=len;
    
    if( real_len==-1 )
    {
      real_len=data_len - start;	
    }
    
    if( real_len + start > data_len )
    {
      return false;
    }	
    
    if( real_len>0 )
    {
      memcpy( buf, data+start, real_len );	
    }
    return true;
  }
  
  
  
  int buf_t::extract_to_string( 
    char *str, 
    int start, 
    int max_len, 
    const char *separators,
    const char *ignores
    ) const
  {
    int cur_pos=start;
    int cur_len=0;
    
    if( cur_pos>=data_len )
    {
      
      // nothing else to read
      *str='\0';
      return -1;	
    }
    
    // if the string is too long for the buffer it gets truncated.
    
    while( cur_len<max_len-1 && cur_pos<data_len )
    {
      // if the character is a separator char or null, then finish the string
      // and return it.
      
      if( data[cur_pos]==0 || strchr( separators, data[cur_pos] ) )
      {
        break;
      }
      
      // is the character an ignorable character?
      
      if( !strchr( ignores, data[cur_pos]) )
      {
        // No, It isnt a separator or an ignore, so transfer the character
        
        str[cur_len]=data[cur_pos];
        ++cur_len;		    	
      }
      
      // update the indexes
      
      
      ++cur_pos;
      
    }
    
    str[cur_len]='\0';
    
    return cur_pos+1;
  }
  
  int buf_t::extract_to_string( 
    string_t &str,
    int start,  
    int max_len,
    const char *separators, 
    const char *ignores
    ) const
  {
    int cur_pos=start;
    int cur_len=0;
    str = "";
    
    if( cur_pos>=data_len )
    {     
      // nothing else to read
      return -1;	
    }
    
    // if the string is too long for the buffer it gets truncated.
    
    while( cur_len<max_len-1 && cur_pos<data_len )
    {
      // if the character is a separator char or null, then finish the string
      // and return it.
      
      if( data[cur_pos]==0 || strchr( separators, data[cur_pos] ) )
      {
        break;
      }
      
      // is the character an ignorable character?
      
      if( !strchr( ignores, data[cur_pos]) )
      {
        // No, It isnt a separator or an ignore, so transfer the character
        
        str += data[cur_pos];
        ++cur_len;		    	
      }
      
      // update the indexes          
      ++cur_pos;      
    }
    
    return cur_pos+1;
  }  
}


