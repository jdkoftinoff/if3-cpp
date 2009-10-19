/*
 
 The IF Contextual Media Group Kernel Version 3
 Source Code
 
 Written By Jeff Koftinoff <jeffk@contextualmediagroup.com>
 Copyright (c) 1995-2005
 By Contextual Media Group, Inc.
 http://www.contextualmediagroup.com/
 
 ALL RIGHTS RESERVED.
 
 */
#include "ifcmg_world.hpp"
#include "ifcmg_db_util.hpp"

namespace ifcmg 
{
  string_list_t split_tabline( string_t const &line )
  {
    string_list_t result;
    
    int len = line.length();
    
    string_t cur;

    for( int i=0; i<len; ++i )
    {      
      char c=line.at(i);
      
      bool tab = (c=='\t');
      bool end = (c=='\n' || c=='\r' || i==len-1);
      
      if( tab || end  )
      {
        result.push_back( cur );
        cur.clear();
      }
      else 
      {
        cur.push_back( c );
      }
      
      if( end ) 
      {
        break;
      }        
    }
    return result;
  }

  void decode_tbd_entry( string_t const &entry, string_t *result, string_t const & def )
  {
    bool r=decode_tbd_entry( entry, result );
    if( r==false )
    {
      *result = def;
    }
  }

  bool decode_tbd_entry( string_t const &entry, string_t *result )
  {
    int len = entry.length();
    bool in_backslash=false;
    bool is_null = false;
    bool is_done = false;
    
    result->clear();
        
    for( int i=0; i<len && is_done==false; ++i )
    {            
      char c=entry.at(i);
      
      if( in_backslash )
      {
        switch (c) 
        {
          case 'N':
            is_null = true;
            is_done = true;
            break;
          case '\\':
            c = '\\';
            break;
          case 'b':
            c = '\b';
            break;
          case 'f':
            c = '\f';
            break;
          case 't':
            c = '\t';
            break;
          case 'v':
            c = '\v';
            break;
          case 'n':
            c = '\n';
            break;
          default:
            break;
        }
        if( !is_null )
        {
          result->push_back( c );        
        }
        in_backslash = false;
      }
      else 
      {
        if( c=='\\' )
        {
          in_backslash = true;
        }
        else 
        {
          result->push_back( c );
        }
      }
    }
    return !is_null;
  }
  
  void encode_tbd_entry( string_t const &v, string_t *result )
  {
    int len = v.length();
    
    result->clear();
    
    for( int i=0; i<len; ++i )
    {            
      char c=v.at(i);
      
      switch (c) 
      { 
        case '\n':
          result->push_back('\\');
          result->push_back('n');
          break;
        case '\\':
          result->push_back('\\');
          result->push_back('\\');
          break;
        case '\b':
          result->push_back('\\');
          result->push_back('b');
          break;
        case 'f':
          result->push_back('\\');
          result->push_back('f');
          break;
        case 't':
          result->push_back('\\');
          result->push_back('t');
          break;
        case 'v':
          result->push_back('\\');
          result->push_back('v');
          break;
        default:
          result->push_back(c);
          break;
      }
    }    
  }

  bool decode_tbd_entry( string_t const &entry, int64_t *result )
  {
    string_t result_as_string;
    bool r = decode_tbd_entry( entry, &result_as_string );
    if( r ) 
    {
      *result = strtoll(result_as_string.c_str(), 0, 10);
    }    
    return r;
  }
  
  void decode_tbd_entry( string_t const &entry, int64_t *result, int64_t def )
  {
    if( !decode_tbd_entry( entry, result ) )
      *result = def;
  }
  
  void encode_tbd_entry( int64_t v, string_t *result )
  {
    encode_tbd_entry( lexical_cast<string_t>( v ), result );
  }

  bool decode_tbd_entry( string_t const &entry, int32_t *result )
  {
    string_t result_as_string;
    bool r = decode_tbd_entry( entry, &result_as_string );
    if( r ) 
    {
      *result = strtol(result_as_string.c_str(), 0, 10);
    }    
    return r;
    
  }
  
  void decode_tbd_entry( string_t const &entry, int32_t *result, int32_t def )
  {
    if( !decode_tbd_entry(entry,result) )
      *result = def;
  }
  
  void encode_tbd_entry( int32_t v, string_t *result )
  {
    encode_tbd_entry( lexical_cast<string_t>( v ), result );
  }

  bool decode_tbd_entry( string_t const &entry, float *result )
  {
    string_t result_as_string;
    bool r = decode_tbd_entry( entry, &result_as_string );
    if( r ) 
    {
      *result = atof(result_as_string.c_str());
    }    
    return r;
    
  }
  
  void decode_tbd_entry( string_t const &entry, float *result, float def )
  {
    if( !decode_tbd_entry(entry,result) )
      *result = def;
  }
  
  void encode_tbd_entry( float v, string_t *result )
  {
    encode_tbd_entry( lexical_cast<string_t>( v ), result );
  }
  
}
