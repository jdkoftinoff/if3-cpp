/*
  
The Internet Filter Version 3 Kernel Version 3
Source Code

Written By Jeff Koftinoff <jeffk@internetfilter.com>
Copyright (c) 1995-2005
By Turner and Sons Productions, Inc.
http://www.internetfilter.com/

ALL RIGHTS RESERVED.

*/
#ifndef IF3_TREE_HPP
#define IF3_TREE_HPP

#include "if3_world.hpp"
#include "if3_dynbuf.hpp"
#include "if3_treebase.hpp"
#include "if3_mmap_buf.hpp"
#include "if3_pattern_expander.hpp"

namespace if3
{
  template <class VALUE_T>
  struct tree_nonalphanumericpunct_ignorer_t
  {
    inline bool operator() ( const VALUE_T &a ) const
    {
      return !::isalnum(a) && !::ispunct(a);
    }
  };
  
  
  
  struct tree_traits_alphanumeric_t
  {	
    typedef char value_t;
    typedef int index_t;
    typedef short flags_t;
    typedef dynbuf_t buf_t;
    typedef treebase_case_insensitive_comparator_t<value_t> comparator_t;
    typedef tree_nonalphanumericpunct_ignorer_t<value_t> ignorer_t;
    typedef treebase_basic_leaf_t< value_t, index_t, flags_t > leaf_t;
    enum { required_prefix_flag = false };
    static inline bool test_prefix( value_t v ) { return false; }
  };
  
  
  struct tree_traits_url_t
  {
    typedef char value_t;
    typedef int index_t;
    typedef short flags_t;
    typedef dynbuf_t buf_t;
    typedef treebase_case_insensitive_comparator_t<value_t> comparator_t;
    typedef treebase_basic_ignorer_t<value_t> ignorer_t;
    typedef treebase_basic_leaf_t< value_t, index_t, flags_t > leaf_t;
    enum { required_prefix_flag = true };
    static inline bool test_prefix( value_t v ) { return ( v=='.' || v=='/' || v=='=' || v=='&'); }
  };
  
  struct tree_traits_alphanumeric_mmap_t
  {	
    typedef char value_t;
    typedef int index_t;
    typedef short flags_t;
    typedef mmap_buf_t buf_t;
    typedef treebase_case_insensitive_comparator_t<value_t> comparator_t;
    typedef tree_nonalphanumericpunct_ignorer_t<value_t> ignorer_t;
    typedef treebase_basic_leaf_t< value_t, index_t, flags_t > leaf_t;
    enum { required_prefix_flag = false };
    static inline bool test_prefix( value_t v ) { return false; }
  };
  
  
  struct tree_traits_url_mmap_t
  {
    typedef char value_t;
    typedef int index_t;
    typedef short flags_t;
    typedef mmap_buf_t buf_t;
    typedef treebase_case_insensitive_comparator_t<value_t> comparator_t;
    typedef treebase_basic_ignorer_t<value_t> ignorer_t;
    typedef treebase_basic_leaf_t< value_t, index_t, flags_t > leaf_t;
    enum { required_prefix_flag = true };
    static inline bool test_prefix( value_t v ) { return ( v=='.' || v=='/' || v=='=' || v=='&'); }
  };
  
  
  template <class T>
  class tree_event_t
  {
  public:
    typedef T tree_traits_t;	
    tree_event_t() 
    {
    }
    
    virtual ~tree_event_t()
    {
    }
    
    virtual void operator () ( 
      const treebase_t<tree_traits_t> &tree, 
      typename tree_traits_t::index_t match_item 
      )  = 0;
  };
  
  
  template <class T>
  class tree_t : public pattern_target_t
  {	
  public:
    typedef T tree_traits_t;	
    
    explicit tree_t()
      : 
      tree( 1000 )
    {
      for( int i=0; i<256; ++i )
      {
        initial_shortcuts[i]=-1;	
      }		
    }
    
    explicit tree_t( const filename_t &filename )
      : 
      tree( filename )
    {
      make_shortcuts();
    }
    
    
    virtual ~tree_t()
    {
      
    }
    
    
    void clear()
    {
      tree.clear();	
      for( int i=0; i<256; ++i )
      {
        initial_shortcuts[i]=-1;	
      }	  
    }
    
    bool load( const buf_t &buf )
    {
      bool r = tree.getbuf().copy(buf);
      make_shortcuts();
      return r;
    }	
    
    bool save( dynbuf_t &buf ) const
    {
      return buf.copy(tree.getbuf());
    }
    
    
    void add( const string_t &s, typename tree_traits_t::flags_t flags )
    {
      tree.add( s.c_str(), s.length(), flags );				
    }
    
    void remove( const string_t &s )
    {
    }
    
    
    bool find( 
      const void *buf, 
      int buf_len,
      tree_event_t<tree_traits_t> &event
      ) const
    {
      int match_count=0;
      typename tree_traits_t::flags_t flags;
      typename tree_traits_t::index_t match_item;
      
      const typename tree_traits_t::value_t *p = (const typename tree_traits_t::value_t *)buf;
      
      if( tree_traits_t::required_prefix_flag )
      {
        for( int i=0; i<buf_len; ++i, ++p )
        {
          if( tree_traits_t::test_prefix( *p ) )
          {
            int first_value = p[1];
            if( first_value<0 )
              first_value+=128;
            
            typename tree_traits_t::index_t shortcut =initial_shortcuts[first_value];
            if( shortcut != -1 )
            {
              if( tree.find_longest(p+1,buf_len-i-1,&flags,0,&match_item, shortcut, 0 ) )
              {
                // found a match!
                ++match_count;
                event(tree,match_item);
              }
            }
          }
        }
      }
      else
      {
        for( int i=0; i<buf_len; ++i, ++p )
        {		
          int first_value = *p;
          if( first_value<0 )
            first_value+=128;
          
          typename tree_traits_t::index_t shortcut =initial_shortcuts[first_value];
          if( shortcut != -1 )
          {
            if( tree.find_longest( p, buf_len-i, &flags, 0, &match_item, shortcut, 0 ) )
            {
              // found a match!
              ++match_count;
              event( tree, match_item );
            }			
          }
        }
      }
      
      return match_count!=0;
    }
    
    
    
    bool find( 
      const buf_t &buf, 
      tree_event_t<tree_traits_t> &event
      ) const
    {
      return find( buf.get_data(), buf.get_data_length(), event );
    }
    
    bool censor( 
      void *buf, 
      int buf_len,
      tree_event_t<tree_traits_t> &event,
      typename tree_traits_t::value_t replacement
      ) const
    {
      int match_count=0;
      typename tree_traits_t::flags_t flags;
      typename tree_traits_t::index_t match_item;
      
      typename tree_traits_t::value_t *p = (typename tree_traits_t::value_t *)buf;
      
      if( tree_traits_t::required_prefix_flag )
      {
        for( int i=0; i<buf_len; ++i, ++p )
        {
          if( tree_traits_t::test_prefix( *p ) )
          {
            int first_value = p[1];
            if( first_value<0 )
              first_value+=128;
            
            typename tree_traits_t::index_t shortcut = initial_shortcuts[first_value];
            if( shortcut != -1 )
            {
              if( tree.censor_longest(p+1,buf_len-i-1,replacement,&flags,0,&match_item, shortcut, 0 ) )
              {
                // found a match!
                ++match_count;
                event(tree,match_item);
              }
            }
          }
        }
      }
      else
      {
        for( int i=0; i<buf_len; ++i, ++p )
        {		
          int first_value = *p;
          if( first_value<0 )
            first_value+=128;
          
          typename tree_traits_t::index_t shortcut =initial_shortcuts[first_value];
          if( shortcut != -1 )
          {
            if( tree.censor_longest( p, buf_len-i, replacement, &flags, 0, &match_item, shortcut, 0 ) )
            {
              // found a match!
              ++match_count;
              event( tree, match_item );
            }			
          }
        }
      }
      
      return match_count!=0;
    }
    
    bool censor( 
      buf_t &buf, 
      tree_event_t<tree_traits_t> &event,
      typename tree_traits_t::value_t replacement
      ) const
    {
      return censor( buf.get_data(), buf.get_data_length(), event, replacement );
    }
    
    
    void make_shortcuts()
    {
      
      // start at first entry, go through all siblings and extract item offsets for each entry
      for( int i=0; i<256; ++i )
      {
        initial_shortcuts[i]=-1;	
      }
      
      typename tree_traits_t::index_t item=0;
      
      do
      {
        const typename tree_traits_t::leaf_t &leaf = tree.get( item );
        if( leaf.is_free() || leaf.is_end() )
          break;
        if( ::isalpha( leaf.value ) )
        {
          initial_shortcuts[ (int)::toupper(leaf.value) ] = item;			
          initial_shortcuts[ (int)::tolower(leaf.value) ] = item;						
        }
        else
        {
          initial_shortcuts[ (int)leaf.value ] = item;
        }
        item = leaf.sibling;
      } while( item>0 );		
    }
    
    int extract( 
      typename tree_traits_t::value_t *list, 
      int max_len,
      typename tree_traits_t::index_t end_leaf_index 
      ) const
    {
      return tree.extract( list, max_len, end_leaf_index );
    }
    
    int extract( 
      string_t &result, 				
      typename tree_traits_t::index_t end_leaf_index
      ) const
    {
      int maxlen=4096;
      for( int i=0; i<maxlen; ++i )
      {
        result[i]='\0';
      }
      
      return tree.extract(
        (typename tree_traits_t::value_t *)result.c_str(), 
        4096-1,
        end_leaf_index
        );
    }
    
  private:
    treebase_t<tree_traits_t> tree;
    typename tree_traits_t::index_t initial_shortcuts[256];
  };
  
  template <class tree_traits_t,class pattern_expander_t>
  inline bool tree_compile( 
    const filename_t &src,
    const filename_t &dest,
    int code_offset,
    const string_t &prefix,
    volatile int *progress=0
    )
  {
    bool r=false;
    tree_t<tree_traits_t> tree;
    pattern_expander_t expander;
    
    dynbuf_t input_buf;
    
    if(
      input_buf.append_from_file( src )
      &&  expander.load( input_buf, tree, code_offset, prefix, progress )
      )
    {  
      tree.make_shortcuts();
      
      dynbuf_t results;
      if( tree.save( results ) )
      {
        if( results.extract_to_file( dest ) )
        {
          r = true;
        }
      }
    }
    return r;
  }

}

#endif
