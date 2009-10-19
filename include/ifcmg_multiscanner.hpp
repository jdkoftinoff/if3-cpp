/*
  
The IF Contextual Media Group Kernel Version 3
Source Code

Written By Jeff Koftinoff <jeffk@contextualmediagroup.com>
Copyright (c) 1995-2005
By Contextual Media Group, Inc.
http://www.contextualmediagroup.com/

ALL RIGHTS RESERVED.

*/
#ifndef IFCMG_MULTISCANNER_HPP
#define IFCMG_MULTISCANNER_HPP

#include "ifcmg_world.hpp"
#include "ifcmg_string.hpp"
#include "ifcmg_buf.hpp"
#include "ifcmg_util.hpp"
#include "ifcmg_scanner.hpp"

namespace ifcmg
{
  
  class multiscanner_result_t
  {
  public:
    enum { MAX_CATEGORIES = 32 };
    
    multiscanner_result_t()
      :
      total_good_url_match_count(0),
      total_bad_url_match_count(0),
      total_postbad_url_match_count(0),
      total_bad_phrase_match_count(0)
    {
      for( unsigned int i=0; i<MAX_CATEGORIES; ++i )
      {
        good_url_match_count[i] = 0;
        bad_url_match_count[i] = 0;
        postbad_url_match_count[i] = 0;
        bad_phrase_match_count[i] = 0;
      }
    }
    
    multiscanner_result_t( const multiscanner_result_t &other )
      :
      total_good_url_match_count( other.total_good_url_match_count ),
      total_bad_url_match_count( other.total_bad_url_match_count ),
      total_postbad_url_match_count( other.total_postbad_url_match_count ),
      total_bad_phrase_match_count( other.total_bad_phrase_match_count )
    {
      for( unsigned int i=0; i<MAX_CATEGORIES; ++i )
      {
        good_url_match_count[i] = other.good_url_match_count[i];
        bad_url_match_count[i] = other.bad_url_match_count[i];
        postbad_url_match_count[i] = other.postbad_url_match_count[i];
        bad_phrase_match_count[i] = other.bad_phrase_match_count[i];
      }
    }

#ifndef SWIG    
    const multiscanner_result_t & operator = ( const multiscanner_result_t &other )
    {
      if( &other != this )
      {
        total_good_url_match_count = other.total_good_url_match_count;
        total_bad_url_match_count = other.total_bad_url_match_count;
        total_postbad_url_match_count = other.total_postbad_url_match_count;
        total_bad_phrase_match_count = other.total_bad_phrase_match_count;
        
        for( unsigned int i=0; i<MAX_CATEGORIES; ++i )
        {
          good_url_match_count[i] = other.good_url_match_count[i];
          bad_url_match_count[i] = other.bad_url_match_count[i];
          postbad_url_match_count[i] = other.postbad_url_match_count[i];
          bad_phrase_match_count[i] = other.bad_phrase_match_count[i];
        }        
      }
      return *this;
    }
#endif
    
    ~multiscanner_result_t()
    {
    }
    
    void clear()
    {
      total_good_url_match_count =0;
      total_bad_url_match_count =0;
      total_postbad_url_match_count =0;
      total_bad_phrase_match_count =0;
      for( unsigned int i=0; i<MAX_CATEGORIES; ++i )
      {
        good_url_match_count[i] = 0;
        bad_url_match_count[i] = 0;
        postbad_url_match_count[i] = 0;
        bad_phrase_match_count[i] = 0;
      }
    }
    
    unsigned int get_total_good_url_match_count() const
    {
      return total_good_url_match_count;
    }
    
    unsigned int get_good_url_match_count( unsigned int category ) const
    {
      return good_url_match_count[category];
    }
    
    unsigned int get_total_bad_url_match_count() const
    {
      return total_bad_url_match_count;
    }
    
    unsigned int get_bad_url_match_count( unsigned int category ) const
    {
      return bad_url_match_count[category];
    }

    unsigned int get_total_postbad_url_match_count() const
    {
      return total_postbad_url_match_count;
    }

    unsigned int get_postbad_url_match_count( unsigned int category ) const
    {
      return bad_url_match_count[category];
    }
    
    unsigned int get_total_bad_phrase_match_count() const
    {
      return total_bad_phrase_match_count;
    }
    
    unsigned int get_bad_phrase_match_count( unsigned int category ) const
    {
      return bad_phrase_match_count[category];
    }
    
    void add_good_url( unsigned int category )
    {
      total_good_url_match_count++;
      good_url_match_count[ category ]++;
    }
    
    void add_bad_url( unsigned int category )
    {
      total_bad_url_match_count++;
      bad_url_match_count[ category ]++;
    }

    void add_postbad_url( unsigned int category )
    {
      total_postbad_url_match_count++;
      postbad_url_match_count[ category ]++;
    }
    
    void add_bad_phrase( unsigned int category )
    {
      total_bad_phrase_match_count++;
      bad_phrase_match_count[ category ]++;
    }
    
    std::string to_string() const
    {
      std::stringstream os;

      os << "total_good_url_match_count = " << total_good_url_match_count << "\n";
      os << "total_bad_url_match_count = " << total_bad_url_match_count << "\n";
      os << "total_postbad_url_match_count = " << total_postbad_url_match_count << "\n";
      os << "total_bad_phrase_match_count = " << total_bad_phrase_match_count << "\n";
      for( size_t i=0; i<MAX_CATEGORIES; ++i )
      {
        if( good_url_match_count[i] !=0 )
        {
          os << "good_url_match_count category " << i << " = " <<  good_url_match_count[i] << "\n";
        }
        if( bad_url_match_count[i] !=0 )
        {
          os << "bad_url_match_count category " << i << " = " <<  bad_url_match_count[i] << "\n";
        }
        if( postbad_url_match_count[i] !=0 )
        {
          os << "postbad_url_match_count category " << i << " = " <<  postbad_url_match_count[i] << "\n";
        }
        if( bad_phrase_match_count[i] !=0 )
        {
          os << "bad_phrase_match_count category " << i << " = " <<  bad_phrase_match_count[i] << "\n";
        }
      }

      return os.str();
    }

  private:
    
    unsigned int total_good_url_match_count;
    unsigned int total_bad_url_match_count;
    unsigned int total_postbad_url_match_count;
    unsigned int total_bad_phrase_match_count;
    unsigned int good_url_match_count[MAX_CATEGORIES];
    unsigned int bad_url_match_count[MAX_CATEGORIES];
    unsigned int postbad_url_match_count[MAX_CATEGORIES];
    unsigned int bad_phrase_match_count[MAX_CATEGORIES];
  };
  
  template <class T>
  class multiscanner_good_url_event_t  : public tree_event_t<T>
  {
  public:
    typedef T tree_traits_t;
    
    multiscanner_good_url_event_t( multiscanner_result_t &result_, unsigned int current_category_ )
      :
      result( result_ ),
      current_category( current_category_ )
    {
    }
    
    virtual void operator () ( const treebase_t<tree_traits_t> &tree, typename tree_traits_t::index_t match_item )
    {
      result.add_good_url( current_category );
    }	
    
  private:
    multiscanner_result_t &result;
    int current_category;
  };
  
  template <class T>
  class multiscanner_bad_url_event_t  : public tree_event_t<T>
  {
  public:
    typedef T tree_traits_t;
    
    multiscanner_bad_url_event_t( multiscanner_result_t &result_, unsigned int current_category_ )
      :
      result( result_ ),
      current_category(current_category_)
    {
    }
    
    virtual void operator () ( const treebase_t<tree_traits_t> &tree, typename tree_traits_t::index_t match_item )
    {
      result.add_bad_url( current_category );
    }	
    
  private:
    multiscanner_result_t &result;
    int current_category;
  };

  template <class T>
  class multiscanner_postbad_url_event_t  : public tree_event_t<T>
  {
  public:
    typedef T tree_traits_t;
    
    multiscanner_postbad_url_event_t( multiscanner_result_t &result_, unsigned int current_category_ )
      :
      result( result_ ),
      current_category(current_category_)
    {
    }
    
    virtual void operator () ( const treebase_t<tree_traits_t> &tree, typename tree_traits_t::index_t match_item )
    {
      result.add_postbad_url( current_category );
    }	
    
  private:
    multiscanner_result_t &result;
    int current_category;
  };
  
  template <class T>
  class multiscanner_bad_phrase_event_t  : public tree_event_t<T>
  {
  public:
    typedef T tree_traits_t;
    
    multiscanner_bad_phrase_event_t( multiscanner_result_t &result_, int current_category_ )
      :
      result( result_ ),
      current_category( current_category_)
    {
    }
    
    virtual void operator () ( const treebase_t<tree_traits_t> &tree, typename tree_traits_t::index_t match_item )
    {
      result.add_bad_phrase( current_category );
    }	
    
  private:
    multiscanner_result_t &result;
    int current_category;
  };
  
  class multiscanner_t
  {
  public:
    enum { MAX_CATEGORIES = 32 };
    
    multiscanner_t( 
      const filename_t compiled_dir_name_,
      const filename_t non_compiled_dir_name_,
      unsigned int good_url_enable_bits,
      unsigned int bad_url_enable_bits,
      unsigned int postbad_url_enable_bits,
      unsigned int bad_phrase_enable_bits
      )
    {
      filename_t compiled_dir_name( compiled_dir_name_ );
      filename_t non_compiled_dir_name( non_compiled_dir_name_ );
      
      util::fix_directory_name( compiled_dir_name );
      util::fix_directory_name( non_compiled_dir_name );
      
      for( unsigned int i=0; i<MAX_CATEGORIES; ++i )
      {
        good_urls_precompiled[i] = 
          load_url_scanner_precompiled(
            compiled_dir_name,
            good_url_enable_bits,
            i,
            "goodurl"
            );
        
        good_urls[i] = 
          load_url_scanner(
            non_compiled_dir_name,
            good_url_enable_bits,
            i,
            "goodurl"
            );
        
        bad_urls_precompiled[i] = 
          load_url_scanner_precompiled(
            compiled_dir_name,
            bad_url_enable_bits,
            i,
            "badurl"
            );
  
        postbad_urls_precompiled[i] = 
          load_url_scanner_precompiled(
            compiled_dir_name,
            postbad_url_enable_bits,
            i,
            "postbadurl"
            );
        
        bad_urls[i] = 
          load_url_scanner(
            non_compiled_dir_name,
            bad_url_enable_bits,
            i,
            "badurl"
            );

        postbad_urls[i] = 
          load_url_scanner(
            non_compiled_dir_name,
            postbad_url_enable_bits,
            i,
            "postbadurl"
            );
        
        bad_phrases_precompiled[i] = 
          load_alphanumeric_scanner_precompiled(
            compiled_dir_name,
            bad_phrase_enable_bits,
            i,
            "badphr"
            );
        
        bad_phrases[i] = 
          load_alphanumeric_scanner(
            non_compiled_dir_name,
            bad_phrase_enable_bits,
            i,
            "badphr"
            );
      }
    }
    
    virtual ~multiscanner_t()
    {
      for( unsigned int i=0; i<MAX_CATEGORIES; ++i )
      {
        delete good_urls_precompiled[i];
        delete good_urls[i];
        delete bad_urls_precompiled[i];
        delete bad_urls[i];
        delete postbad_urls_precompiled[i];
        delete postbad_urls[i];
        delete bad_phrases_precompiled[i];
        delete bad_phrases[i];
      }
    }

    multiscanner_result_t find_in_string(
      std::string str,
      unsigned int good_url_enable_bits,
      unsigned int bad_url_enable_bits,
      unsigned int postbad_url_enable_bits,
      unsigned int bad_phrase_enable_bits
      )
    {
      return find_in_data( 
        str.c_str(), 
        str.length(), 
        good_url_enable_bits, 
        bad_url_enable_bits, 
        postbad_url_enable_bits,
        bad_phrase_enable_bits 
        );
    }
    
    multiscanner_result_t find_in_data(
      const void *buf,
      int buf_len,
      unsigned int good_url_enable_bits,
      unsigned int bad_url_enable_bits,
      unsigned int postbad_url_enable_bits,
      unsigned int bad_phrase_enable_bits
      )
    {
      multiscanner_result_t result;
      
      for( unsigned int i=0; i<MAX_CATEGORIES; ++i )
      {
        int bit = 1<<i;

        if( bad_url_enable_bits & bit )
        {
          if( bad_urls_precompiled[i] )
          {
            multiscanner_bad_url_event_t<tree_traits_url_mmap_t> event(result,i);
            bad_urls_precompiled[i]->find( buf, buf_len, event );
          }
          
          if( bad_urls[i] )
          {
            multiscanner_bad_url_event_t<tree_traits_url_t> event(result,i);
            bad_urls[i]->find( buf, buf_len, event );
          }
        }

        if( good_url_enable_bits & bit )
        {
          if( good_urls_precompiled[i] )
          {
            multiscanner_good_url_event_t<tree_traits_url_mmap_t> event(result,i);
            good_urls_precompiled[i]->find( buf, buf_len, event );
          }
          
          if( good_urls[i] )
          {
            multiscanner_good_url_event_t<tree_traits_url_t> event(result,i);
            good_urls[i]->find( buf, buf_len, event );
          }
        }
        
        if( postbad_url_enable_bits & bit )
        {
          if( postbad_urls_precompiled[i] )
          {
            multiscanner_postbad_url_event_t<tree_traits_url_mmap_t> event(result,i);
            postbad_urls_precompiled[i]->find( buf, buf_len, event );
          }
          
          if( postbad_urls[i] )
          {
            multiscanner_postbad_url_event_t<tree_traits_url_t> event(result,i);
            postbad_urls[i]->find( buf, buf_len, event );
          }
        }
        
        if( bad_phrase_enable_bits & bit )
        {
          if( bad_phrases_precompiled[i] )
          {
            multiscanner_bad_phrase_event_t<tree_traits_alphanumeric_mmap_t> event(result,i);
            bad_phrases_precompiled[i]->find( buf, buf_len, event );
          }
          
          if( bad_phrases[i] )
          {
            multiscanner_bad_phrase_event_t<tree_traits_alphanumeric_t> event(result,i);
            bad_phrases[i]->find( buf, buf_len, event );
          }
        }        
      }
      return result;
    }


    std::string censor_in_string(
      std::string str,
      char censor_char,
      unsigned int bad_url_enable_bits,
      unsigned int postbad_url_enable_bits,
      unsigned int bad_phrase_enable_bits
      )
    {      
      censor_in_data( 
        const_cast<char *>(str.c_str()),
        str.length(), 
        censor_char,
        bad_url_enable_bits,
        postbad_url_enable_bits,
        bad_phrase_enable_bits
        );
      return str;
    }
    
    multiscanner_result_t censor_in_data(
      void *buf,
      int buf_len,
      char censor_char,
      unsigned int bad_url_enable_bits,
      unsigned int postbad_url_enable_bits,
      unsigned int bad_phrase_enable_bits
      )
    {
      multiscanner_result_t result;
      
      for( unsigned int i=0; i<MAX_CATEGORIES; ++i )
      {
        int bit = 1<<i;
        
        if( bad_url_enable_bits & bit )
        {
          if( bad_urls_precompiled[i] )
          {
            multiscanner_bad_url_event_t<tree_traits_url_mmap_t> event(result,i);
            bad_urls_precompiled[i]->censor( buf, buf_len, event, censor_char );
          }
          
          if( bad_urls[i] )
          {
            multiscanner_bad_url_event_t<tree_traits_url_t> event(result,i);          
            bad_urls[i]->censor( buf, buf_len, event, censor_char );
          }
        }

        if( postbad_url_enable_bits & bit )
        {
          if( postbad_urls_precompiled[i] )
          {
            multiscanner_postbad_url_event_t<tree_traits_url_mmap_t> event(result,i);
            postbad_urls_precompiled[i]->censor( buf, buf_len, event, censor_char );
          }
          
          if( postbad_urls[i] )
          {
            multiscanner_postbad_url_event_t<tree_traits_url_t> event(result,i);          
            postbad_urls[i]->censor( buf, buf_len, event, censor_char );
          }
        }
        
        if( bad_phrase_enable_bits & bit )
        {
          if( bad_phrases_precompiled[i] )
          {
            multiscanner_bad_phrase_event_t<tree_traits_alphanumeric_mmap_t> event(result,i);
            bad_phrases_precompiled[i]->censor( buf, buf_len, event, censor_char );
          }
          
          if( bad_phrases[i] )
          {
            multiscanner_bad_phrase_event_t<tree_traits_alphanumeric_t> event(result,i);          
            bad_phrases[i]->censor( buf, buf_len, event, censor_char );
          }
        }        
      }
      return result;
    }
    
  private:
    
    url_scanner_precompiled_t *load_url_scanner_precompiled(
      filename_t compiled_dir_name,
      unsigned int enable_bits,
      int category,
      const char *middle_part
      )
    {
      url_scanner_precompiled_t *scanner = 0;
      
      if( enable_bits && (1<<category) )
      {
        filename_t input_filename;      
        form( input_filename, "%s%d%s.pre", compiled_dir_name.c_str(), category+1, middle_part );
        
        if( util::file_exists( input_filename ) )
        {
          scanner = newnothrow url_scanner_precompiled_t( input_filename );
          if( !scanner )
          {
            ifcmg_throw( "new url_scanner_precompiled_t" );
          }
        }
      }
      return scanner;
    }
    
    url_scanner_t *load_url_scanner(
      filename_t non_compiled_dir_name,
      unsigned int enable_bits,
      int category,
      const char *middle_part
      )
    {
      url_scanner_t *scanner = 0;
      
      if( enable_bits && (1<<category) )
      {
        filename_t input_filename;      
        form( input_filename, "%s%d%s.txt", non_compiled_dir_name.c_str(), category+1, middle_part );
        
        if( util::file_exists( input_filename ) )
        {
          scanner = newnothrow url_scanner_t( input_filename );
          if( !scanner )
          {
            ifcmg_throw( "new url_scanner_t" );
          }
        }
      }
      return scanner;
    }
    
    alphanumeric_scanner_precompiled_t *load_alphanumeric_scanner_precompiled(
      filename_t compiled_dir_name,
      unsigned int enable_bits,
      int category,
      const char *middle_part
      )
    {
      alphanumeric_scanner_precompiled_t *scanner = 0;
      
      if( enable_bits && (1<<category) )
      {
        filename_t input_filename;      
        form( input_filename, "%s%d%s.pre", compiled_dir_name.c_str(), category+1, middle_part );
        
        if( util::file_exists( input_filename ) )
        {
          scanner = newnothrow alphanumeric_scanner_precompiled_t( input_filename );
          if( !scanner )
          {
            ifcmg_throw( "new alphanumeric_scanner_precompiled_t" );
          }
        }
      }
      return scanner;
    }
    
    alphanumeric_scanner_t *load_alphanumeric_scanner(
      filename_t non_compiled_dir_name,
      unsigned int enable_bits,
      int category,
      const char *middle_part
      )
    {
      alphanumeric_scanner_t *scanner = 0;
      
      if( enable_bits && (1<<category) )
      {
        filename_t input_filename;      
        form( input_filename, "%s%d%s.txt", non_compiled_dir_name.c_str(), category+1, middle_part );
        
        if( util::file_exists( input_filename ) )
        {
          scanner = newnothrow alphanumeric_scanner_t( input_filename );
          if( !scanner )
          {
            ifcmg_throw( "alphanumeric_scanner_t" );
          }
        }
      }
      return scanner;
    }
    
    url_scanner_precompiled_t *good_urls_precompiled[MAX_CATEGORIES];
    url_scanner_t *good_urls[MAX_CATEGORIES];
    
    url_scanner_precompiled_t *bad_urls_precompiled[MAX_CATEGORIES];
    url_scanner_t *bad_urls[MAX_CATEGORIES];

    url_scanner_precompiled_t *postbad_urls_precompiled[MAX_CATEGORIES];
    url_scanner_t *postbad_urls[MAX_CATEGORIES];
    
    alphanumeric_scanner_precompiled_t *bad_phrases_precompiled[MAX_CATEGORIES];
    alphanumeric_scanner_t *bad_phrases[MAX_CATEGORIES];
    
    unsigned int enable_bits;
  };
}

#endif
