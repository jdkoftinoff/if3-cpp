/*  
 The Internet Filter Version 3 Kernel Version 3
 Source Code
 
 Written By Jeff Koftinoff <jeffk@internetfilter.com>
 Copyright (c) 1995-2005
 By Turner and Sons Productions, Inc.
 http://www.internetfilter.com/
 
 ALL RIGHTS RESERVED.
 
 */
#ifndef IF3_CGI_HPP
#define IF3_CGI_HPP

#include "if3_world.hpp"
#include "if3_string.hpp"

namespace if3 
{
  inline bool cgi_is_escapable( char c )
  {
    bool r=true;
    
    if( ::isalnum(c) )
    {
      r=false;
    }
    else 
    {
      if( std::strchr( ";/?:@&=+!*'(),$-_.%<>~", c )!=0 )
        r=false;
    }
    return r;
  }
  
  inline bool cgi_is_escapable_char_with_amp( char c )
  {
    bool r=true;
    
    if( ::isalnum(c) )
    {
      r=false;
    }
    else
    {
      if( std::strchr( ";/?:@=+!*'(),$-_.%<>~", c )!=0 )
        r=false;
    }
    
    return r;	  
  }
  
  
  inline bool cgi_is_escapable_char_for_var( char c ) 
  {
    bool r=true;
    
    if( ::isalnum(c) )
    {
      r=false;
    }
    else
    {
      if( std::strchr( ";/?:@&=+!*'(),$-_.%<>~", c )!=0 )
        r=true;
    }
    
    return r;	  
  } 
  
  inline bool cgi_is_escapable_char_with_amp_for_var( char c )
  {
    bool r=true;
    
    if( ::isalnum(c) )
    {
      r=false;
    }
    else
    {
      if( std::strchr( ";/?:@=+!*'(),$-_.%<>~", c )!=0 )
        r=true;
    }
    
    return r;	  
  }
  
  bool cgi_unescape( string_t const &inval, string_t &outval );  
  bool cgi_escape( string_t const &inval, string_t &outval );  
  bool cgi_unescape_with_amp( string_t const &inval, string_t &outval );  
  bool cgi_escape_with_amp( string_t const &inval, string_t &outval );  
  bool cgi_extract_pair( string_t const &item, string_t &out_name, string_t &out_val );
  bool cgi_encode_pair( string_t const &out_name, string_t const &out_val, string_t &result );
}

#endif

