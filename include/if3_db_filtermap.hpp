/*  
The Internet Filter Version 3 Kernel Version 3
Source Code

Written By Jeff Koftinoff <jeffk@internetfilter.com>
Copyright (c) 1995-2005
By Turner and Sons Productions, Inc.
http://www.internetfilter.com/

ALL RIGHTS RESERVED.

*/
#ifndef IF3_DB_FILTERMAP_HPP
#define IF3_DB_FILTERMAP_HPP

#include "if3_world.hpp"
#include "if3_db_util.hpp"


namespace if3 
{
  class db_filtermap_row_t 
  {
  public:
    db_filtermap_row_t() 
    :
    m_id(-1),
    m_name("")
    {
    }
    
    db_filtermap_row_t( db_filtermap_row_t const &o )
    :
    m_id( o.m_id ),
    m_name( o.m_name )
    {
    }
    
    db_filtermap_row_t( string_t const &line )
    {      
    }
    
    ~db_filtermap_row_t()
    {
    }
        
    db_filtermap_row_t & operator = ( db_filtermap_row_t const &o );
    
    int64_t get_id() const { return m_id; }
    string_t const & get_name() const { return m_name; }
    
  private:
    int64_t m_id;
    string_t m_name;  
  };
  
  typedef std::vector< db_filtermap_row_t > db_filtermap_table_t;
  
  void load( db_filtermap_table_t &t, filename_t file );  
}


#endif

