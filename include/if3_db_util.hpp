/*  
 The Internet Filter Version 3 Kernel Version 3
 Source Code
 
 Written By Jeff Koftinoff <jeffk@internetfilter.com>
 Copyright (c) 1995-2005
 By Turner and Sons Productions, Inc.
 http://www.internetfilter.com/
 
 ALL RIGHTS RESERVED.
 
 */
#ifndef IF3_DB_UTIL_HPP
#define IF3_DB_UTIL_HPP

#include "if3_world.hpp"
#include "if3_string.hpp"

namespace if3 
{
  typedef std::list< string_t > string_list_t;
  
  string_list_t split_tabline( string_t const &line );
  
  bool decode_tbd_entry( string_t const &entry, string_t *result );
  
  void decode_tbd_entry( string_t const &entry, string_t *result, string_t const & def );
  void encode_tbd_entry( string_t const &v, string_t *result );

  bool decode_tbd_entry( string_t const &entry, int64_t *result );  
  void decode_tbd_entry( string_t const &entry, int64_t *result, int64_t def );  
  void encode_tbd_entry( int64_t v, string_t *result );

  bool decode_tbd_entry( string_t const &entry, int32_t *result );  
  void decode_tbd_entry( string_t const &entry, int32_t *result, int32_t def );  
  void encode_tbd_entry( int32_t v, string_t *result );

  bool decode_tbd_entry( string_t const &entry, float *result );  
  void decode_tbd_entry( string_t const &entry, float *result, float def );  
  void encode_tbd_entry( float v, string_t *result );
  
}

#endif

