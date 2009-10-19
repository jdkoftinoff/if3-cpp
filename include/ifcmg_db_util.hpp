/*  
 The IF Contextual Media Group Kernel Version 3
 Source Code
 
 Written By Jeff Koftinoff <jeffk@contextualmediagroup.com>
 Copyright (c) 1995-2005
 By Contextual Media Group, Inc.
 http://www.contextualmediagroup.com/
 
 ALL RIGHTS RESERVED.
 
 */
#ifndef IFCMG_DB_UTIL_H
#define IFCMG_DB_UTIL_H

#include "ifcmg_world.hpp"
#include "ifcmg_string.hpp"

namespace ifcmg 
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

