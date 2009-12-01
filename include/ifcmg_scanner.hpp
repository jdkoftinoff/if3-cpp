/*
  
The IF Contextual Media Group Kernel Version 3
Source Code

Written By Jeff Koftinoff <jeffk@contextualmediagroup.com>
Copyright (c) 1995-2005
By Contextual Media Group, Inc.
http://www.contextualmediagroup.com/

ALL RIGHTS RESERVED.

*/
#ifndef IFCMG_SCANNER_HPP
#define IFCMG_SCANNER_HPP

#include "ifcmg_pattern_expander.hpp"
#include "ifcmg_tree.hpp"
#include "ifcmg_dynbuf.hpp"

namespace ifcmg
{
  
  template <class tree_traits_t>
  class scanner_precompiled_t : public tree_t<tree_traits_t>
  {
  public:
    
    scanner_precompiled_t( 
      const filename_t &filename
      ) 
      :    
      tree_t<tree_traits_t>( filename )
    {
    }
    
    scanner_precompiled_t( 
      const filename_t &filename,
      const string_t &tree_type_,
      const string_t &tree_category_
      ) 
      :    
      tree_t<tree_traits_t>( filename, tree_type_, tree_category_ )
    {
    }
    
    
    virtual ~scanner_precompiled_t()
    {
    }
    
  protected:
  };
  
  
  
  template <class TREE_TRAITS_T, class PATTERN_EXPANDER_T>
  class scanner_t : public tree_t<TREE_TRAITS_T>
  {
  public:
    typedef PATTERN_EXPANDER_T pattern_expander_t;
    typedef TREE_TRAITS_T tree_traits_t;
    
    scanner_t( 
      const filename_t &filename_,
      int flags_=0
      ) 
      :
      tree_t<tree_traits_t>(),
      filename( filename_ ),
      prefix( "" ),    
      flags(flags_)
    {
      load();
    }
    
    scanner_t( 
      const filename_t &filename_,
      const string_t &prefix_,
      int flags_
      ) 
      :
      tree_t<tree_traits_t>(),
      filename( filename_ ),
      prefix( prefix_ ),    
      flags(flags_)
    {
      load();
    }
    
    void load()
    {
      dynbuf_t data( filename );
      
      tree_t<tree_traits_t>::clear();
      pattern_expander_t expander;
      expander.load( 
        data,
        *this,
        flags,
        prefix
        );           
      
      tree_t<tree_traits_t>::make_shortcuts();
    }    
    
    virtual ~scanner_t()
    {
    }
    
    
  protected:
    filename_t filename;
    const string_t prefix;
    int flags;
  };
  
  
  typedef scanner_t<tree_traits_url_t,pattern_expander_standard_t> url_scanner_t;
  typedef scanner_t<tree_traits_alphanumeric_t,pattern_expander_standard_t> alphanumeric_scanner_t;
  
  typedef scanner_precompiled_t<tree_traits_url_mmap_t> url_scanner_precompiled_t;
  typedef scanner_precompiled_t<tree_traits_alphanumeric_mmap_t> alphanumeric_scanner_precompiled_t;

}

#endif
