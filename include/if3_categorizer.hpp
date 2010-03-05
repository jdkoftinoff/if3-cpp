/*
 The Internet Filter Version 3 Kernel Version 3
 Source Code

 Written By Jeff Koftinoff <jeffk@internetfilter.com>
 Copyright (c) 1995-2005
 By Turner and Sons Productions, Inc.
 http://www.internetfilter.com/

 ALL RIGHTS RESERVED.

 */
#ifndef IF3_CATEGORIZER_HPP
#define IF3_CATEGORIZER_HPP

#include "if3_world.hpp"
#include "if3_html_parse.hpp"

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
