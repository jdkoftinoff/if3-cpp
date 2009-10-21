/*  
The IF Contextual Media Group Kernel Version 3
Source Code

Written By Jeff Koftinoff <jeffk@contextualmediagroup.com>
Copyright (c) 1995-2005
By Contextual Media Group, Inc.
http://www.contextualmediagroup.com/

ALL RIGHTS RESERVED.

*/
#ifndef IFCMG_DB_NEEDLES_HPP
#define IFCMG_DB_NEEDLES_HPP

#include "ifcmg_world.hpp"

#include "ifcmg_string.hpp"
#include "ifcmg_db_util.hpp"

namespace ifcmg 
{
 
  class db_needles_row_t 
  {
  public:
    db_needles_row_t() 
    :
    m_id(-1)
    {
    }
    
    db_needles_row_t( db_needles_row_t const &o )
    :
    m_id( o.m_id )
    {
    }
    
    db_needles_row_t( string_t const &line )
    {      
    }
    
    ~db_needles_row_t() 
    {
    }
    
    db_needles_row_t & operator = ( db_needles_row_t const &o )
    {
      return *this;
    }
    
    int64_t m_id;
    char m_needle[128];
    int64_t m_section;
    int64_t m_category1;
    int64_t m_category2;
    int64_t m_category3;
    int64_t m_category4;
    int32_t m_score;
    int32_t m_autogen;   
  };
    
  typedef std::vector< db_needles_row_t > db_needles_table_t;
  
  void load( db_needles_table_t &t, filename_t file );  

}


#endif

