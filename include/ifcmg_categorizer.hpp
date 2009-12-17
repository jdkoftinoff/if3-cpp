/*  
 The IF Contextual Media Group Kernel Version 3
 Source Code
 
 Written By Jeff Koftinoff <jeffk@contextualmediagroup.com>
 Copyright (c) 1995-2005
 By Contextual Media Group, Inc.
 http://www.contextualmediagroup.com/
 
 ALL RIGHTS RESERVED.
 
 */
#ifndef if3_CATEGORIZER_HPP
#define if3_CATEGORIZER_HPP

#include "if3_world.hpp"
#include "if3_html_parse.hpp"
#include "if3_db.hpp"

namespace if3 
{
  class categorizer_base_t
  {
  public:
    categorizer_base_t() {}
    virtual ~categorizer_base_t() {}    
    
  };
  
  
  class categorizer_html_target_t
  {
  public:
    categorizer_html_target_t( categorizer_base_t &categorizer )
    {
    }
    
    void content_link( string_t const &url )
    {
    }
    
    void link( string_t const &url )
    {
    }
    
    void img( string_t const &url )
    {
    }
    
    void script( string_t const &data )
    {
    }
    
    void text( string_t const &text )
    {
    }
    
  private:
  };
  
  
}

#endif
