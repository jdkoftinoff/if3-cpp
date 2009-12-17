/*
  
The Internet Filter Version 3 Kernel Version 3
Source Code

Written By Jeff Koftinoff <jeffk@internetfilter.com>
Copyright (c) 1995-2005
By Turner and Sons Productions, Inc.
http://www.internetfilter.com/

ALL RIGHTS RESERVED.

*/
#ifndef IF3_PATTERN_EXPANDER_HPP
#define IF3_PATTERN_EXPANDER_HPP

#include "if3_buf.hpp"
#include "if3_string.hpp"
#include "if3_util.hpp"

namespace if3
{
  class pattern_target_t
  {
  public:
    pattern_target_t() {}
    virtual ~pattern_target_t() {}
    
    virtual void add( const string_t &s, short flags ) = 0;
    virtual void remove( const string_t &s ) = 0;    
  };
  
  class pattern_expander_t
  {
  public:
    explicit pattern_expander_t()
    {
    }
    
    virtual ~pattern_expander_t()
    {
    }
    
    virtual bool expand( 
      const string_t &in, 
      pattern_target_t &target,				    
      short flags,
      bool remove,
      const string_t &prefix
      ) = 0;
    
    virtual bool load_remove( 
      const buf_t &buf, 
      pattern_target_t &target,
      const string_t &prefix
      )
    {
      int pos=0;
      string_t str;
      
      while( (pos=buf.extract_to_string(str,pos,4096))>=0 )
      {
        if( str.length()>4 )
        {
          if( !expand( str, target, 0, true, prefix ) )
          {
            return false;
          }				  
        }			 
      }
      return true;    
    }				 
    
    virtual bool load( 
      const buf_t &buf,
      pattern_target_t &target,				  
      short flags,
      const string_t &prefix,
      volatile int *progress = 0
      )
    {
      int pos=0;
      string_t str;
      progress=0;
      while( (pos=buf.extract_to_string(str,pos,4096))>=0 )
      {
        progress++;
        if( str.length()>4 )
        {
          if( !expand( str, target, flags, false, prefix ) )
          {
            return false;
          }				  
        }			 
      }
      return true;
    }
    
    
    virtual bool load(
      const filename_t &fname,
      pattern_target_t &target,				 
      short flags,
      const string_t &prefix,
      volatile int *progress=0
      )
    {
      string_t str;
      progress=0;
      FILE *f=fopen(fname.c_str(),"rt");
      if( f )		
      {
        while( util::read_line( str, f ) )
        {
          progress++;
          if( str.length()>4 )
          {			
            if( !expand( str, target, flags, false, prefix ) )
            {
              fclose(f);
              return false;
            }				  
          }
        }			 
        fclose(f);
        return true;
      }
      else
      {			 
        return false;
      }
    }
    
    
    virtual bool load_remove(
      const filename_t &fname,
      pattern_target_t &target,
      const string_t &prefix
      )
    {
      string_t str;
      FILE *f=fopen(fname.c_str(),"rt");
      if( f )		
      {
        while( util::read_line( str, f ) )
        {
          if( !expand( str, target, 0, true, prefix ) )
          {
            fclose(f);
            return false;
          }				  
          
        }			 
        fclose(f);
        return true;
      }
      else
      {			 
        return false;
      }
      
    }
    
    
  };
  
  class pattern_expander_standard_t : public pattern_expander_t
  {
  public:
    explicit pattern_expander_standard_t()
    {
    }

    ~pattern_expander_standard_t();
    
    bool expand(
      const string_t &in, 
      pattern_target_t &target,			
      short flags,
      bool remove,
      const string_t &prefix
			);
    
    
  };
  
}

#endif
