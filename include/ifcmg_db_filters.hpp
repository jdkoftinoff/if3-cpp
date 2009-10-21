/*  
The IF Contextual Media Group Kernel Version 3
Source Code

Written By Jeff Koftinoff <jeffk@contextualmediagroup.com>
Copyright (c) 1995-2005
By Contextual Media Group, Inc.
http://www.contextualmediagroup.com/

ALL RIGHTS RESERVED.

*/
#ifndef IFCMG_DB_FILTERS_HPP
#define IFCMG_DB_FILTERS_HPP

#include "ifcmg_world.hpp"
#include "ifcmg_db_util.hpp"


namespace ifcmg 
{
  class db_filters_row_t 
  {
  public:
    db_filters_row_t() 
    :
    m_id(-1),
    m_name("")
    {
    }
    
    db_filters_row_t( db_filters_row_t const &o )
    :
    m_id( o.m_id ),
    m_name( o.m_name )
    {
    }
    
    db_filters_row_t( string_t const &line )
    {      
    }
    
    ~db_filters_row_t()
    {
    }
    
    db_filters_row_t & operator = ( db_filters_row_t const &o );
    
    int64_t get_id() const { return m_id; }
    string_t const & get_name() const { return m_name; }
    
  private:
    int64_t m_id;
    string_t m_name;  
  };
  
  typedef std::vector< db_filters_row_t > db_filters_table_t;
  
  void load( db_filters_table_t &t, filename_t file );  
}


#endif

