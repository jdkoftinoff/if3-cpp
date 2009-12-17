/*  
The Internet Filter Version 3 Kernel Version 3
Source Code

Written By Jeff Koftinoff <jeffk@internetfilter.com>
Copyright (c) 1995-2005
By Turner and Sons Productions, Inc.
http://www.internetfilter.com/

ALL RIGHTS RESERVED.

*/
#ifndef IF3_DB_NEEDLES_HPP
#define IF3_DB_NEEDLES_HPP

#include "if3_world.hpp"

#include "if3_string.hpp"
#include "if3_db_util.hpp"

namespace if3 
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
      m_id = o.m_id;
      m_needle = o.m_needle;
      m_section = o.m_section;
      m_category1 = o.m_category1;
      m_category2 = o.m_category2;
      m_category3 = o.m_category3;
      m_category4 = o.m_category4;
      m_score = o.m_score;
      m_autogen = o.m_autogen;
      return *this;
    }
    
    int64_t m_id;
    std::string m_needle;
    int64_t m_section;
    int64_t m_category1;
    int64_t m_category2;
    int64_t m_category3;
    int64_t m_category4;
    int32_t m_score;
    int32_t m_autogen;
  };

  class db_needles_table_t
  {
  public:
  };
  
  void load( db_needles_table_t &t, filename_t file );  
}


#endif

