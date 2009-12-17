/*  
 The Internet Filter Version 3 Kernel Version 3
 Source Code
 
 Written By Jeff Koftinoff <jeffk@internetfilter.com>
 Copyright (c) 1995-2005
 By Turner and Sons Productions, Inc.
 http://www.internetfilter.com/
 
 ALL RIGHTS RESERVED.
 
 */
#ifndef IF3_DB_HPP
#define IF3_DB_HPP

#include "if3_world.hpp"

#include "if3_db_needles.hpp"
#include "if3_db_categories.hpp"
#include "if3_db_filters.hpp"
#include "if3_db_filtermap.hpp"
#include "if3_db_sections.hpp"

#include "if3_tree.hpp"

namespace if3
{
  
  class db_t
  {
  public:
    db_t() {}
    virtual ~db_t() {}   
    
    db_needles_table_t m_needles;
    db_categories_table_t m_categories;
    db_filters_table_t m_filters;
    db_filtermap_table_t m_filtermap;
    db_sections_table_t m_sections;
  };
}

#endif
