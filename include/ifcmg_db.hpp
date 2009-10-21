/*  
 The IF Contextual Media Group Kernel Version 3
 Source Code
 
 Written By Jeff Koftinoff <jeffk@contextualmediagroup.com>
 Copyright (c) 1995-2005
 By Contextual Media Group, Inc.
 http://www.contextualmediagroup.com/
 
 ALL RIGHTS RESERVED.
 
 */
#ifndef IFCMG_DB_HPP
#define IFCMG_DB_HPP

#include "ifcmg_world.hpp"

#include "ifcmg_db_categories.hpp"
#include "ifcmg_db_filters.hpp"
#include "ifcmg_db_filtermap.hpp"
#include "ifcmg_db_sections.hpp"
#include "ifcmg_db_needles.hpp"

#include "ifcmg_tree.hpp"

namespace ifcmg
{
  class db_t
  {
  public:
    db_t() {}
    virtual ~db_t() {}   
    
  private:
    db_categories_table_t m_categories;
    db_filters_table_t m_filters;
  };
}

#endif
