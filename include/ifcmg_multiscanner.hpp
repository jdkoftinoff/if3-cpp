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
#include "ifcmg_compile.hpp"

namespace ifcmg
{
  namespace multiscanner
  {
    enum
    {
      MAX_CATEGORIES=64,
      MAX_MATCH_LEN=2048
    };

  class categories_enable_t
  {
  public:
    categories_enable_t()
    {
      for( int i=0; i<MAX_CATEGORIES; ++i )
      {
        enabled[i]=true;
      }
    }

    categories_enable_t( string_t cats )
    {
      for( int i=0; i<MAX_CATEGORIES; ++i )
      {
        enabled[i]=false;
      }
      set_from_string( cats );
    }

    void clear()
    {
      for( int i=0; i<MAX_CATEGORIES; ++i )
      {
        enabled[i]=false;
      }
    }

    void set_from_string( string_t cats )
    {
      std::vector<string_t> items = util::split( cats, ',' );
      for( std::vector<string_t>::iterator i=items.begin(); i!=items.end(); i++ )
      {
        int value = strtol( i->c_str(), 0, 10 );
        if( value>0 && value<=MAX_CATEGORIES )
        {
          enabled[value-1] = true;
        }
      }
    }

    bool is_enabled( int c ) const { return enabled[c]; }

  private:
    bool enabled[MAX_CATEGORIES];
  };

  class result_t
  {
  public:
    typedef std::list< std::pair< unsigned int, string_t > > match_container_t;

    result_t()
      :
      good_url_matches(),
      bad_url_matches(),
      postbad_url_matches(),
      bad_phrase_matches(),
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

    result_t( const result_t &other )
      :
      good_url_matches( other.good_url_matches),
      bad_url_matches( other.bad_url_matches ),
      postbad_url_matches( other.postbad_url_matches ),
      bad_phrase_matches( other.bad_phrase_matches ),
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

    const result_t & operator = ( const result_t &other )
    {
      good_url_matches = other.good_url_matches;
      bad_url_matches = other.bad_url_matches;
      postbad_url_matches = other.postbad_url_matches;
      bad_phrase_matches = other.bad_phrase_matches;
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
      return *this;
    }

    ~result_t()
    {
    }

    void clear()
    {
      good_url_matches.clear();
      bad_url_matches.clear();
      postbad_url_matches.clear();
      bad_phrase_matches.clear();
      
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

    void add_good_url( unsigned int category, const char *match_text=0 )
    {
      total_good_url_match_count++;
      good_url_match_count[ category ]++;
      if( match_text )
      {
        good_url_matches.push_back(
          std::make_pair( category, string_t(match_text) )
          );
      }
    }

    void add_bad_url( unsigned int category, const char *match_text=0 )
    {
      total_bad_url_match_count++;
      bad_url_match_count[ category ]++;
      if( match_text )
      {
        bad_url_matches.push_back(
          std::make_pair( category, string_t(match_text) )
          );
      }
    }

    void add_postbad_url( unsigned int category, const char *match_text=0 )
    {
      total_postbad_url_match_count++;
      postbad_url_match_count[ category ]++;
      if( match_text )
      {
        postbad_url_matches.push_back(
          std::make_pair( category, string_t(match_text) )
          );
      }
    }

    void add_bad_phrase( unsigned int category, const char *match_text=0 )
    {
      total_bad_phrase_match_count++;
      bad_phrase_match_count[ category ]++;
      if( match_text )
      {
        bad_phrase_matches.push_back(
          std::make_pair( category, string_t(match_text) )
          );
      }

    }

    void print_matches( std::ostream &os, const char *title, const match_container_t &matches ) const
    {
      os << title << ":\n";
      for(match_container_t::const_iterator i = matches.begin(); i!=matches.end(); ++i )
      {
        os << "\t" << i->first << "\t" << i->second << "\n";
      }
    }


    string_t to_string() const
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

      print_matches( os, "good url matches", get_good_url_matches() );
      print_matches( os, "bad url matches", get_bad_url_matches() );
      print_matches( os, "postbad url matches", get_postbad_url_matches() );
      print_matches( os, "bad url matches", get_postbad_url_matches() );
      
      return os.str();
    }

    const match_container_t &get_good_url_matches() const
    {
      return good_url_matches;
    }

    const match_container_t &get_bad_url_matches() const
    {
      return bad_url_matches;
    }

    const match_container_t &get_postbad_url_matches() const
    {
      return postbad_url_matches;
    }

    const match_container_t &get_bad_phrase_matches() const
    {
      return bad_phrase_matches;
    }

    friend
    std::ostream & operator << ( std::ostream &os, const result_t &v )
    {
      os << v.to_string();
      return os;
    }

  private:

    match_container_t good_url_matches;
    match_container_t bad_url_matches;
    match_container_t postbad_url_matches;
    match_container_t bad_phrase_matches;

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
  class good_url_event_t  : public tree_event_t<T>
  {
  public:
    typedef T tree_traits_t;

    good_url_event_t( result_t &result_, unsigned int current_category_ )
      :
      result( result_ ),
      current_category( current_category_ )
    {
    }

    virtual void operator () (
      const treebase_t<tree_traits_t> &tree,
      typename tree_traits_t::index_t match_item
    )
    {
      char match_text[MAX_MATCH_LEN]="";
      tree.extract(match_text,MAX_MATCH_LEN,match_item);
      match_text[MAX_MATCH_LEN-1] = '\0';

      result.add_good_url( current_category, match_text );
    }

  private:
    result_t &result;
    int current_category;
  };

  template <class T>
  class bad_url_event_t  : public tree_event_t<T>
  {
  public:
    typedef T tree_traits_t;

    bad_url_event_t( result_t &result_, unsigned int current_category_ )
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
    result_t &result;
    int current_category;
  };

  template <class T>
  class postbad_url_event_t  : public tree_event_t<T>
  {
  public:
    typedef T tree_traits_t;

    postbad_url_event_t( result_t &result_, unsigned int current_category_ )
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
    result_t &result;
    int current_category;
  };

  template <class T>
  class bad_phrase_event_t  : public tree_event_t<T>
  {
  public:
    typedef T tree_traits_t;

    bad_phrase_event_t( result_t &result_, int current_category_ )
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
    result_t &result;
    int current_category;
  };

  class multiscanner_t
  {
  public:

    multiscanner_t(
      const filename_t compiled_dir_name_,
      const filename_t non_compiled_dir_name_
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
            i,
            "goodurl"
            );

        good_urls[i] =
          load_url_scanner(
            non_compiled_dir_name,
            i,
            "goodurl"
            );

        bad_urls_precompiled[i] =
          load_url_scanner_precompiled(
            compiled_dir_name,
            i,
            "badurl"
            );

        postbad_urls_precompiled[i] =
          load_url_scanner_precompiled(
            compiled_dir_name,
            i,
            "postbadurl"
            );

        bad_urls[i] =
          load_url_scanner(
            non_compiled_dir_name,
            i,
            "badurl"
            );

        postbad_urls[i] =
          load_url_scanner(
            non_compiled_dir_name,
            i,
            "postbadurl"
            );

        bad_phrases_precompiled[i] =
          load_alphanumeric_scanner_precompiled(
            compiled_dir_name,
            i,
            "badphr"
            );

        bad_phrases[i] =
          load_alphanumeric_scanner(
            non_compiled_dir_name,
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

    result_t find_in_string(
      string_t str,
      categories_enable_t const &good_url_enable_bits,
      categories_enable_t const &bad_url_enable_bits,
      categories_enable_t const &postbad_url_enable_bits,
      categories_enable_t const &bad_phrase_enable_bits
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

    result_t find_in_data(
      const void *buf,
      int buf_len,
      categories_enable_t const &good_url_enable_bits,
      categories_enable_t const &bad_url_enable_bits,
      categories_enable_t const &postbad_url_enable_bits,
      categories_enable_t const &bad_phrase_enable_bits
      )
    {
      result_t result;

      for( unsigned int i=0; i<MAX_CATEGORIES; ++i )
      {
        if( bad_url_enable_bits.is_enabled(i))
        {
          if( bad_urls_precompiled[i] )
          {
            bad_url_event_t<tree_traits_url_mmap_t> event(result,i);
            bad_urls_precompiled[i]->find( buf, buf_len, event );
          }

          if( bad_urls[i] )
          {
            bad_url_event_t<tree_traits_url_t> event(result,i);
            bad_urls[i]->find( buf, buf_len, event );
          }
        }

        if( good_url_enable_bits.is_enabled(i) )
        {
          if( good_urls_precompiled[i] )
          {
            good_url_event_t<tree_traits_url_mmap_t> event(result,i);
            good_urls_precompiled[i]->find( buf, buf_len, event );
          }

          if( good_urls[i] )
          {
            good_url_event_t<tree_traits_url_t> event(result,i);
            good_urls[i]->find( buf, buf_len, event );
          }
        }

        if( postbad_url_enable_bits.is_enabled(i) )
        {
          if( postbad_urls_precompiled[i] )
          {
            postbad_url_event_t<tree_traits_url_mmap_t> event(result,i);
            postbad_urls_precompiled[i]->find( buf, buf_len, event );
          }

          if( postbad_urls[i] )
          {
            postbad_url_event_t<tree_traits_url_t> event(result,i);
            postbad_urls[i]->find( buf, buf_len, event );
          }
        }

        if( bad_phrase_enable_bits.is_enabled(i) )
        {
          if( bad_phrases_precompiled[i] )
          {
            bad_phrase_event_t<tree_traits_alphanumeric_mmap_t> event(result,i);
            bad_phrases_precompiled[i]->find( buf, buf_len, event );
          }

          if( bad_phrases[i] )
          {
            bad_phrase_event_t<tree_traits_alphanumeric_t> event(result,i);
            bad_phrases[i]->find( buf, buf_len, event );
          }
        }
      }
      return result;
    }


    string_t censor_in_string(
      string_t str,
      char censor_char,
      categories_enable_t const &bad_url_enable_bits,
      categories_enable_t const &postbad_url_enable_bits,
      categories_enable_t const &bad_phrase_enable_bits
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

    result_t censor_in_data(
      void *buf,
      int buf_len,
      char censor_char,
      categories_enable_t const &bad_url_enable_bits,
      categories_enable_t const &postbad_url_enable_bits,
      categories_enable_t const &bad_phrase_enable_bits
      )
    {
      result_t result;

      for( unsigned int i=0; i<MAX_CATEGORIES; ++i )
      {

        if( bad_url_enable_bits.is_enabled(i))
        {
          if( bad_urls_precompiled[i] )
          {
            bad_url_event_t<tree_traits_url_mmap_t> event(result,i);
            bad_urls_precompiled[i]->censor( buf, buf_len, event, censor_char );
          }

          if( bad_urls[i] )
          {
            bad_url_event_t<tree_traits_url_t> event(result,i);
            bad_urls[i]->censor( buf, buf_len, event, censor_char );
          }
        }

        if( postbad_url_enable_bits.is_enabled(i))
        {
          if( postbad_urls_precompiled[i] )
          {
            postbad_url_event_t<tree_traits_url_mmap_t> event(result,i);
            postbad_urls_precompiled[i]->censor( buf, buf_len, event, censor_char );
          }

          if( postbad_urls[i] )
          {
            postbad_url_event_t<tree_traits_url_t> event(result,i);
            postbad_urls[i]->censor( buf, buf_len, event, censor_char );
          }
        }

        if( bad_phrase_enable_bits.is_enabled(i) )
        {
          if( bad_phrases_precompiled[i] )
          {
            bad_phrase_event_t<tree_traits_alphanumeric_mmap_t> event(result,i);
            bad_phrases_precompiled[i]->censor( buf, buf_len, event, censor_char );
          }

          if( bad_phrases[i] )
          {
            bad_phrase_event_t<tree_traits_alphanumeric_t> event(result,i);
            bad_phrases[i]->censor( buf, buf_len, event, censor_char );
          }
        }
      }
      return result;
    }

  private:

    url_scanner_precompiled_t *load_url_scanner_precompiled(
      filename_t compiled_dir_name,
      int category,
      const char *middle_part
      )
    {
      url_scanner_precompiled_t *scanner = 0;

      filename_t input_filename;
      form(input_filename, "%s%03d-%s.pre", compiled_dir_name.c_str(), category + 1, middle_part);
      if (util::file_exists(input_filename)) {
        scanner = newnothrow url_scanner_precompiled_t(input_filename);
        if (!scanner) {
          ifcmg_throw(std::runtime_exception, "new url_scanner_precompiled_t");
        }
      }
      return scanner;
    }

    url_scanner_t *load_url_scanner(
      filename_t non_compiled_dir_name,
      int category,
      const char *middle_part
      )
    {
      url_scanner_t *scanner = 0;

      filename_t input_filename;
      form(input_filename, "%s%03d-%s.txt", non_compiled_dir_name.c_str(), category + 1, middle_part);

      if (util::file_exists(input_filename)) {
        scanner = newnothrow url_scanner_t(input_filename);
        if (!scanner) {
          ifcmg_throw(std::runtime_exception, "new url_scanner_t");
        }
      }
      return scanner;
    }

    alphanumeric_scanner_precompiled_t *load_alphanumeric_scanner_precompiled(
      filename_t compiled_dir_name,
      int category,
      const char *middle_part
      )
    {
      alphanumeric_scanner_precompiled_t *scanner = 0;

      filename_t input_filename;
      form(input_filename, "%s%03d-%s.pre", compiled_dir_name.c_str(), category + 1, middle_part);

      if (util::file_exists(input_filename)) {
        scanner = newnothrow alphanumeric_scanner_precompiled_t(input_filename);
        if (!scanner) {
          ifcmg_throw(std::runtime_exception, "new alphanumeric_scanner_precompiled_t");
        }
      }
      return scanner;
    }

    alphanumeric_scanner_t *load_alphanumeric_scanner(
      filename_t non_compiled_dir_name,
      int category,
      const char *middle_part
      )
    {
      alphanumeric_scanner_t *scanner = 0;

      filename_t input_filename;
      form(input_filename, "%s%03d-%s.txt", non_compiled_dir_name.c_str(), category + 1, middle_part);

      if (util::file_exists(input_filename)) {
        scanner = newnothrow alphanumeric_scanner_t(input_filename);
        if (!scanner) {
          ifcmg_throw(std::runtime_exception, "alphanumeric_scanner_t");
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

  };

  struct category_with_strength_t
  {
    category_with_strength_t( int c = -1, int s =0 ) :
      category( c ),
      strength( s )
    {
    }

    int category;
    int strength;
  };


  class results_histogram_t
  {
  public:
    results_histogram_t()
    {
      for( int i=0; i< MAX_CATEGORIES; ++i )
      {
        m_category_sums[i] = 0;
      }
    }

    void apply_good_results( const result_t &results )
    {
      for( int i=0; i< MAX_CATEGORIES; ++i )
      {
        m_category_sums[i] += results.get_good_url_match_count(i);
      }
    }

    void apply_bad_results( const result_t &results )
    {
      for( int i=0; i< MAX_CATEGORIES; ++i )
      {
        m_category_sums[i] += results.get_bad_phrase_match_count(i);
        m_category_sums[i] += results.get_bad_url_match_count(i);
      }
    }

    void apply_postbad_results( const result_t &results )
    {
      for( int i=0; i< MAX_CATEGORIES; ++i )
      {
        m_category_sums[i] += results.get_postbad_url_match_count(i);
      }
    }

    category_with_strength_t
    calc_largest_category() const
    {
      int category = -1;
      int value = 0;

      for( int i=0; i<MAX_CATEGORIES; ++i )
      {
        if (m_category_sums[i] >= value && m_category_sums[i]>0)
        {
          category = i;
          value = m_category_sums[i];
        }
      }
      return category_with_strength_t(category,value);
    }

    category_with_strength_t
    calc_smallest_category() const
    {
      int category = -1;
      int value = 0;

      for( int i=MAX_CATEGORIES-1; i>=0; --i )
      {
        if (m_category_sums[i] >= value && m_category_sums[i]>0)
        {
          category = i;
          value = m_category_sums[i];
        }
      }
      return category_with_strength_t(category,value);
    }

  private:
    int m_category_sums[ MAX_CATEGORIES ];
  };

  class ifcmgkernel_t
  {
  public:

    ifcmgkernel_t(
                  const filename_t compiled_db_path,
                  const filename_t uncompiled_db_path,
                  bool do_compiling,
                  int safe_category,
                  int unknown_category
                  )
      :
      m_compiled_db_path( compiled_db_path ),
      m_uncompiled_db_path( uncompiled_db_path ),
      m_multiscanner(0),
      m_safe_category( safe_category ),
      m_unknown_category( unknown_category )
    {
      if( do_compiling )
      {
        compile_all();
      }
      m_multiscanner = new multiscanner_t(
                                          m_compiled_db_path,
                                          m_uncompiled_db_path
                                          );
    }

    virtual ~ifcmgkernel_t()
    {
      delete m_multiscanner;
    }

    category_with_strength_t
    validate_hostname(
                      string_t hostname,
                      string_t good_categories_enable,
                      string_t bad_categories_enable
                      )
    {
      string_t fixup = string_t("http://") + hostname;

      return validate_data( fixup.c_str(), fixup.length(), good_categories_enable, bad_categories_enable );
    }

    category_with_strength_t
    validate_link(
                      string_t url,
                      string_t good_categories_enable,
                      string_t bad_categories_enable
                      )
    {
      return validate_data( url.c_str(), url.length(), good_categories_enable, bad_categories_enable );
    }

    category_with_strength_t
    validate_text(
                      const char *data, int data_len,
                      string_t good_categories_enable,
                      string_t bad_categories_enable
                      )
    {
      return validate_data( data, data_len, good_categories_enable, bad_categories_enable );
    }

    category_with_strength_t
    validate_links(
                   const char *data, int data_len,
                   string_t good_categories_enable,
                   string_t bad_categories_enable
                   )
    {
      return validate_data( data, data_len, good_categories_enable, bad_categories_enable );
    }

    category_with_strength_t
    validate_content_links(
                           const char *data, int data_len,
                           string_t good_categories_enable,
                           string_t bad_categories_enable
                           )
    {
      return validate_data( data, data_len, good_categories_enable, bad_categories_enable );
    }


    category_with_strength_t
    validate_data(
                  const char *data,
                  int data_len,
                  string_t good_categories_enable,
                  string_t bad_categories_enable
                  )
    {
      categories_enable_t good_categories_enable_bits( good_categories_enable );
      categories_enable_t bad_categories_enable_bits( bad_categories_enable );
      result_t result;

      result = m_multiscanner->find_in_data(
                                            data, data_len,
                                            good_categories_enable,
                                            bad_categories_enable,
                                            bad_categories_enable,
                                            bad_categories_enable
                                            );

      results_histogram_t good_histogram;
      good_histogram.apply_good_results( result );
      category_with_strength_t good_category = good_histogram.calc_largest_category();

      results_histogram_t bad_histogram;
      bad_histogram.apply_bad_results( result );
      category_with_strength_t bad_category = bad_histogram.calc_largest_category();

      results_histogram_t postbad_histogram;
      postbad_histogram.apply_postbad_results( result );
      category_with_strength_t postbad_category = postbad_histogram.calc_largest_category();

      // default to unknown
      category_with_strength_t final(m_unknown_category,1);

      // if it matches postbad, then it is bad
      if( postbad_category.category != -1 )
      {
        final = postbad_category;
      }
      // otherwise if it matches good, then it is good
      else if( good_category.category != -1 )
      {
        final = good_category;
      }
      // otherwise if it matches bad, then it is bad
      else if( bad_category.category!=-1 )
      {
        final = bad_category;
      }

      return final;
    }

    struct allscans_t
    {
      category_with_strength_t hostname;
      category_with_strength_t link;
      category_with_strength_t text;
      category_with_strength_t links;
      category_with_strength_t content_links;
      category_with_strength_t final;
    };

    allscans_t validate(
                        string_t hostname,
                        string_t link,
                        const char *text, int text_len,
                        const char *links, int links_len,
                        const char *content_links, int content_links_len,
                        string_t good_categories_enable_for_hostname,
                        string_t good_categories_enable_for_link,
                        string_t good_categories_enable_for_content,
                        string_t bad_categories_enable
                        )
    {
      allscans_t result;
      result.hostname = validate_hostname(
                                          hostname,
                                          good_categories_enable_for_hostname,
                                          bad_categories_enable
                                           );

      result.link = validate_link(
                                  link,
                                  good_categories_enable_for_link,
                                  bad_categories_enable
                                   );

      result.text = validate_text(
                                  text, text_len,
                                  good_categories_enable_for_content,
                                  bad_categories_enable
                                   );

      result.links = validate_links(
                                    links, links_len,
                                    good_categories_enable_for_content,
                                    bad_categories_enable
                                     );

      result.content_links = validate_links(
                                            content_links,
                                            content_links_len,
                                            good_categories_enable_for_content,
                                            bad_categories_enable
                                             );

      // default the final result to be based on the hostname
      result.final = result.hostname;

      // if it was unknown, then look at the link response
      if( result.final.category == m_unknown_category )
        result.final = result.link;

      // if that is still unknown, then look at the largest score of the rest of the content
      if( result.final.category == m_unknown_category )
      {
        result.final = result.text;
        if( result.links.strength >  result.final.strength )
          result.final = result.links;
        if( result.content_links.strength > result.final.strength )
          result.final = result.content_links;
      }

      return result;
    }

  private:
    void compile_all()
    {
      int count=0;

      count+=compile_files<tree_traits_url_t,pattern_expander_standard_t>(
                                                                          m_uncompiled_db_path,
                                                                          m_compiled_db_path,
                                                                          "goodurl",
                                                                          MAX_CATEGORIES
                                                                          );

      count+=compile_files<tree_traits_url_t,pattern_expander_standard_t>(
                                                                          m_uncompiled_db_path,
                                                                          m_compiled_db_path,
                                                                          "badurl",
                                                                          MAX_CATEGORIES
                                                                          );

      count+=compile_files<tree_traits_url_t,pattern_expander_standard_t>(
                                                                          m_uncompiled_db_path,
                                                                          m_compiled_db_path,
                                                                          "postbadurl",
                                                                          MAX_CATEGORIES
                                                                          );

      count+=compile_files<tree_traits_alphanumeric_t,pattern_expander_standard_t>(
                                                                                   m_uncompiled_db_path,
                                                                                   m_compiled_db_path,
                                                                                   "badphr",
                                                                                   MAX_CATEGORIES
                                                                                   );
    }

    filename_t m_compiled_db_path;
    filename_t m_uncompiled_db_path;
    multiscanner_t * m_multiscanner;
    int m_safe_category;
    int m_unknown_category;
  };
  }
}

#endif
