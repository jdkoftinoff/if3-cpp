/*
  
The IF Contextual Media Group Kernel Version 3
Source Code

Written By Jeff Koftinoff <jeffk@contextualmediagroup.com>
Copyright (c) 1995-2005
By Contextual Media Group, Inc.
http://www.contextualmediagroup.com/

ALL RIGHTS RESERVED.

*/
#ifndef IFCMG_TREEBASE_HPP
#define IFCMG_TREEBASE_HPP

#include "ifcmg_world.hpp"
#include "ifcmg_util.hpp"

namespace ifcmg
{  
  template <class VALUE_T>
  struct treebase_basic_ignorer_t
  {
    bool operator() ( const VALUE_T &a ) const
    {
      return false;
    }
  };
  
  template <class VALUE_T>
  struct treebase_whitespace_ignorer_t
  {
    bool operator() ( const VALUE_T &a ) const
    {
      return util::isspace(a);
    }
  };
  
  template <class VALUE_T>
  struct treebase_nonalphanumeric_ignorer_t
  {
    bool operator() ( const VALUE_T &a ) const
    {
      return !util::isalnum(a);
    }
  };
  
  template <class VALUE_T>
  struct treebase_basic_comparator_t
  {
    int operator() ( const VALUE_T &a, const VALUE_T &b ) const
    {
      int result;
      
      if( a==b )
        result=0;
      else if( a<b )
        result=-1;
      else
        result=1;	    
      return result;
    }
  };
  
  
  template <class VALUE_T>
  struct treebase_case_insensitive_comparator_t
  {
    int operator() ( const VALUE_T &a_, const VALUE_T &b_ ) const
    {
      int result;
      VALUE_T a( util::toupper(a_) );
      VALUE_T b( util::toupper(b_) );
      
      if( a==b )
        result=0;
      else if( a<b )
        result=-1;
      else
        result=1;	    
      return result;
    }
  };
  
  
  
  template <class VALUE_T, class INDEX_T, class FLAGS_T>
  struct treebase_basic_leaf_t
  {
    treebase_basic_leaf_t()
      : sibling(0), child(0), parent(0), flags(0), value(0), in_use(0)
    {
    }
    
    void clear()
    {
      sibling=0;
      child=0;
      parent=0;
      flags=0;
      value=0;
      in_use=0;
    }
    
    bool is_free() const { return (in_use&1)==0; }
    void release() { in_use=0; }
    void allocate() { in_use|=1; }
    
    bool is_end() const { return bool((in_use&2) !=0); }	
    void set_end() { in_use|=2; }
    void unset_end() { in_use=in_use&1; }
		
    INDEX_T sibling;
    INDEX_T child;
    INDEX_T parent;
    FLAGS_T flags;
    VALUE_T value;	
    unsigned char in_use;    
  };
  
  
  template <typename TRAITS_T>
  class treebase_t
  {
  public:
    typedef TRAITS_T tree_traits_t;
    
  protected:
    typename tree_traits_t::buf_t buf;	
    typename tree_traits_t::index_t first_free;
    
  public:
    
    explicit treebase_t( int max_items=100 ) : 
      buf( sizeof( typename tree_traits_t::leaf_t ) * max_items),
      first_free(0)
    {
      buf.fill();
			clear(0,max_items);
    }
    
    explicit treebase_t( const filename_t &filename ) :
      buf( filename ),
      first_free(0)
    {
      buf.fill();
    }
    
    explicit treebase_t( const typename tree_traits_t::buf_t &otherbuf ) :
      buf( otherbuf ),
      first_free(0)
    {
      buf.fill();	    
    }
    
    const treebase_t<tree_traits_t> & operator = ( const treebase_t<tree_traits_t> &o )
    {
	    if( &o != this )
	    {
        buf = o.buf;
        buf.fill();		
		    first_free = o.first_free;
	    }
	    return *this;
    }
    
    void clear()
    {
      buf.fill();		
      first_free=0;
    }
    
    void clear( typename tree_traits_t::index_t start, typename tree_traits_t::index_t end )
    {
	    typename tree_traits_t::leaf_t *casted_buf = (typename tree_traits_t::leaf_t *)buf.get_data();
      
      for( typename tree_traits_t::index_t i=start; i<end; ++i )
      {
        casted_buf[i].clear();	
      }		
    }
    
    int count() const
    {
	    return buf.get_data_length() / sizeof( typename tree_traits_t::leaf_t );
    }
    
    int count_items_used() const
    {
	    int cnt=count();
	    int last=0;
	    typename tree_traits_t::leaf_t *casted_buf = (typename tree_traits_t::leaf_t *)buf.get_data();
	    
	    for( typename tree_traits_t::index_t i=0; i<cnt; ++i )
	    {
        if( !casted_buf[i].is_free() )
          last=i;
	    }
	    return last+1;
    }
    
    size_t calc_memory_used() const
    {
      int cnt=count_items_used();
      return cnt * sizeof( typename tree_traits_t::leaf_t );
    }
    
    
    const typename tree_traits_t::leaf_t &get( typename tree_traits_t::index_t i ) const
    {
	    typename tree_traits_t::leaf_t *casted_buf = (typename tree_traits_t::leaf_t *)buf.get_data();
	    
	    return casted_buf[i];
    }
    
    typename tree_traits_t::leaf_t &get( typename tree_traits_t::index_t i )	
    {
	    typename tree_traits_t::leaf_t *casted_buf = (typename tree_traits_t::leaf_t *)buf.get_data();	
	    return casted_buf[i];
    }
    
    
    
    typename tree_traits_t::index_t add_child( 
      typename tree_traits_t::index_t parent_item, 
      const typename tree_traits_t::value_t &value, 
      const typename tree_traits_t::flags_t &flags 
      )
    {
	    int item=find_next_free();
	    typename tree_traits_t::leaf_t *casted_buf = (typename tree_traits_t::leaf_t *)buf.get_data();
      
	    // put data into found space, no further siblings, no children
      casted_buf[item].sibling = 0;
	    casted_buf[item].child = 0;
	    casted_buf[item].value = value;
	    casted_buf[item].flags = flags;
	    casted_buf[item].parent = parent_item;
	    casted_buf[item].allocate();
	    
      
	    // find last child
	    if( parent_item!=item && parent_item!=-1 ) // but only if there are any parents at all!
	    {
        int last_child = casted_buf[parent_item].child;
        if( last_child )
        {
          while( casted_buf[last_child].sibling )
          {
            last_child = casted_buf[last_child].sibling;
          }
          
          // found the last child of the parent. Tell him he has a brother
          casted_buf[last_child].sibling = item;		
        }
        else
        {
          // parent has no children yet. add our new child as a child of parent
          if( parent_item!=-1 )
            casted_buf[parent_item].child = item;
        }
	    }
	    return item;	    
    }
    
    typename tree_traits_t::index_t add_sibling( 
      typename tree_traits_t::index_t last_sibling_item, 
      const typename tree_traits_t::value_t &value, 
      const typename tree_traits_t::flags_t &flags 
      )
    {
	    int item=find_next_free();
	    typename tree_traits_t::leaf_t *casted_buf = (typename tree_traits_t::leaf_t *)buf.get_data();
      
	    // put data into found space, no further siblings, no children
      casted_buf[item].sibling = 0;
	    casted_buf[item].child = 0;
	    casted_buf[item].value = value;
	    casted_buf[item].flags = flags;
	    
      casted_buf[item].parent = casted_buf[last_sibling_item].parent;    // same parent as my older brother
	    casted_buf[item].allocate();
      
      casted_buf[last_sibling_item].sibling=item;
	    return item;	    
    }
    
    
    void remove( typename tree_traits_t::index_t i )
    {
	    typename tree_traits_t::leaf_t *casted_buf = (typename tree_traits_t::leaf_t *)buf.get_data();
      casted_buf[i].release();	    
	    if( i < first_free )
        first_free=i;
    }
    
    const typename tree_traits_t::buf_t &getbuf() const
    {
	    return buf;
    }
    
    typename tree_traits_t::buf_t &getbuf() 
    {
	    return buf;
    }			
    
    typename tree_traits_t::index_t find_next_free()
    {
	    typename tree_traits_t::index_t num = 
        (typename tree_traits_t::index_t)(buf.get_data_length() / sizeof(typename tree_traits_t::leaf_t));
	    
	    for( ;first_free<num; ++first_free )
	    {
        if( get(first_free).is_free() )
		    {
          break;
		    }
	    }
	    if( first_free==num )
	    {
        if( !buf.expand( buf.get_data_length()*3/2 ) )
        {
          ifcmg_throw("Unable to expand tree buffer");		
		    }
        buf.fill();
        clear( first_free, (buf.get_data_length() / sizeof(typename tree_traits_t::leaf_t)) );
	    }
	    return first_free;
    }
    
    bool find_sibling( 
      typename tree_traits_t::index_t first_item, 
      const typename tree_traits_t::value_t &v, 
      typename tree_traits_t::index_t *item 
      ) const
    {
	    typename tree_traits_t::leaf_t *casted_buf = (typename tree_traits_t::leaf_t *)buf.get_data();
	    typename tree_traits_t::comparator_t comparator;
      
	    while( !casted_buf[ first_item ].is_free() )
	    {
        // does it match?
        
        if( !casted_buf[first_item].is_end() && comparator( casted_buf[first_item].value, v )==0 )
		    {
          // yup! return the value
			    *item = first_item;
			    return true;
		    }
		    else
		    {
          // nope! check his brother
			    if( casted_buf[ first_item ].sibling==0 )
			    {
            // no more siblings, so return the last sibling and FALSE
            *item = first_item;
            return false;
			    }
          else
          {
            first_item = casted_buf[ first_item ].sibling;			    
          }
		    }
	    }
      // the caller must see if the item does match. If it doesn't,
	    // it is the index of the last sibling.
	    
	    return false;
    }
    
    void add( 
      const typename tree_traits_t::value_t *list, 
      int list_len, 
      const typename tree_traits_t::flags_t flags 
      )
    {
	    // is there a top family?
      
	    typename tree_traits_t::ignorer_t ignore;
	    int list_pos=0;
      
	    // skip any initial items that are needed to ignore	    
	    while( list_pos<list_len && ignore(list[list_pos]) )
        ++list_pos;
	    
	    if( get(0).is_free() )
	    {
        // nope, this is our first ever add
        typename tree_traits_t::index_t i=-1;
        
        while( list_pos<list_len )
        {
			    // add only the items that are not to be ignored
			    if( !ignore(list[list_pos]) )
			    {
    				i=add_child(i,list[list_pos],flags);
			    }
          list_pos++;
        }
        typename tree_traits_t::value_t zero(0);
        i=add_child(i,zero,flags);
        get(i).set_end();
        
        // and we are done!
        return;		    
	    }
	    else
	    {
        // find a matching char
        typename tree_traits_t::index_t i=0;
		    
		    while(1)
		    {				            
          // skip any items that are needed to ignore	    
          while( list_pos<list_len && ignore(list[list_pos]) )
            ++list_pos;
          
			    if( list_pos==list_len )
          {
            typename tree_traits_t::value_t zero(0);
            i=add_child(get(i).parent,zero,flags);
            get(i).set_end();
            
            return;    // we are done!
          }
          
          
          if( find_sibling(i,list[list_pos],&i) )
			    {
            ++list_pos;
            
            // We already have it
				    // so traverse to the child of this sibling.
				    if( get(i).child ==0 )
				    {
              // no child, so add the rest of the children here
					    while( list_pos<list_len )
					    {
                if( !ignore(list[list_pos]) )
                  i=add_child(i,list[list_pos],flags);
						    list_pos++;
              }
              typename tree_traits_t::value_t zero(0);
              i=add_child(i,zero,flags);
              
              get(i).set_end();
              
              // and we are done!
              return;
				    }
				    else
				    {
              i=get(i).child;
				    }
			    }
			    else   
			    {
            // no sibling available
            
				    // i points to last sibling of the family.
				    i = add_sibling( i, list[list_pos++], flags );
				    
				    while( list_pos<list_len )
				    {
					    if( !ignore(list[list_pos]) )
                i=add_child(i,list[list_pos],flags);
              list_pos++;
				    }
            typename tree_traits_t::value_t zero(0);
            i=add_child(i,zero,flags);
            get(i).set_end();
            
				    // and we are done!
				    return;
			    }
        }
	    }
    }
    
    bool find_shortest( 
      const typename tree_traits_t::value_t *list, 
      int list_len, 
      typename tree_traits_t::flags_t *flags=0, 
      int *match_len=0, 
      typename tree_traits_t::index_t *match_item=0
      ) const
    {
	    typename tree_traits_t::leaf_t *casted_buf = (typename tree_traits_t::leaf_t *)buf.get_data();
	    typename tree_traits_t::index_t i=0;
	    typename tree_traits_t::index_t list_pos=0;
	    typename tree_traits_t::ignorer_t ignore;
      
	    // skip any initial items that are needed to ignore	    
	    while( list_pos<list_len && ignore(list[list_pos]) )
        ++list_pos;
      
      while( list_pos<list_len && !casted_buf[i].is_free() )
      {
		    if( !find_sibling(i,list[list_pos],&i ) )
		    {
          // no match, return false
          return false;
		    }
		    
		    // is there a child?
		    if( casted_buf[i].child )
		    {				
          // follow the child.
			    i = casted_buf[i].child;
          
          // if any of the children are an end point, then we have a match
          
          for( typename tree_traits_t::index_t j=i; j!=0; j=casted_buf[j].sibling )
          {
            if( casted_buf[j].is_end() )
            {	
              // found a match!
              
					    if( flags )
                *flags = casted_buf[j].flags;
              if( match_len )
						    *match_len = list_pos+1;
              if( match_item )
						    *match_item = j;
              return true;						
            }					
          }
          
		    }
		    else
		    {
          // no, so we have a match.
			    if( flags )
            *flags = casted_buf[i].flags;
          if( match_len )
				    *match_len = list_pos+1;
          if( match_item )
				    *match_item = i;
          return true;
		    }
        
		    // move to the next non ignored item in the list
        while( list_pos<list_len && ignore(list[++list_pos]) );            
      }
      
      // no match
      return false;
    }
    
    
    bool find_longest(
      const typename tree_traits_t::value_t *list, 
      int list_len, 
      typename tree_traits_t::flags_t *flags=0, 
      int *match_len=0, 
      typename tree_traits_t::index_t *match_item=0,
      typename tree_traits_t::index_t initial_leaf_pos=0,
      typename tree_traits_t::index_t initial_list_pos=0
      ) const
    {
	    typename tree_traits_t::leaf_t *casted_buf = (typename tree_traits_t::leaf_t *)buf.get_data();
	    typename tree_traits_t::index_t i=initial_leaf_pos;
	    typename tree_traits_t::index_t list_pos=initial_list_pos;
	    typename tree_traits_t::ignorer_t ignore;
      
	    // skip any initial items that are needed to ignore	    
	    while( list_pos<list_len && ignore(list[list_pos]) )
        ++list_pos;
      
      while( list_pos<list_len && !casted_buf[i].is_free() )
      {
		    if( !find_sibling(i,list[list_pos],&i ) )
		    {
          // no match, return false
          return false;
		    }
		    
		    // is there a child?
		    if( casted_buf[i].child )
		    {				
          // follow the child.
			    i = casted_buf[i].child;
          
          // if any of the children are an end point, then we have a match
          
          for( typename tree_traits_t::index_t j=i; j!=0; j=casted_buf[j].sibling )
          {
            if( casted_buf[j].is_end() )
            {	
              // found a match!
              
              // go through all children again and see if we can find a longer match
              
					    //typename typename tree_traits_t::comparator_t comparator;
              
              for( typename tree_traits_t::index_t k=i; k!=0; k=casted_buf[k].sibling )
              {
                if( find_longest(
                      list, 
                      list_len, 
                      flags, 
                      match_len, 
                      match_item,
                      k,
                      list_pos+1
                      ) )
                {
                  // by definition it is longer than the one we have.
                  return true;
                }
              }
              
              // our match is the longest
					    if( flags )
                *flags = casted_buf[j].flags;
              if( match_len )
						    *match_len = list_pos+1;
              if( match_item )
						    *match_item = j;
              return true;						
            }					
          }
          
		    }
		    else
		    {
          // no, so we have a match.
			    if( flags )
            *flags = casted_buf[i].flags;
          if( match_len )
				    *match_len = list_pos+1;
          if( match_item )
				    *match_item = i;
          return true;
		    }
        
		    // move to the next non ignored item in the list
        while( list_pos<list_len && ignore(list[++list_pos]) );            
      }
      
      // no match
      return false;
    }
    
    void perform_censor(
      typename tree_traits_t::value_t *list,
      int list_len,
      typename tree_traits_t::value_t replacement,
      int match_pos,
      int match_end_pos
      ) const
    {
	    typename tree_traits_t::index_t list_pos=match_pos;
	    typename tree_traits_t::ignorer_t ignore;
      
	    // skip any initial items that are needed to ignore	    
	    while( list_pos<list_len && list_pos<=match_end_pos )
      {
        if( !ignore(list[list_pos] ) )
        {
          list[list_pos] = replacement;
        }
        ++list_pos;
      }
    }
    
    bool censor_longest(
      typename tree_traits_t::value_t *list, 
      int list_len, 
      typename tree_traits_t::value_t replacement,
      typename tree_traits_t::flags_t *flags=0, 
      int *match_len=0, 
      typename tree_traits_t::index_t *match_item=0,
      typename tree_traits_t::index_t initial_leaf_pos=0,
      typename tree_traits_t::index_t initial_list_pos=0
      ) const
    {
	    typename tree_traits_t::leaf_t *casted_buf = (typename tree_traits_t::leaf_t *)buf.get_data();
	    typename tree_traits_t::index_t i=initial_leaf_pos;
	    typename tree_traits_t::index_t list_pos=initial_list_pos;
	    typename tree_traits_t::ignorer_t ignore;
      
	    // skip any initial items that are needed to ignore	    
	    while( list_pos<list_len && ignore(list[list_pos]) )
        ++list_pos;
      
      while( list_pos<list_len && !casted_buf[i].is_free() )
      {
		    if( !find_sibling(i,list[list_pos],&i ) )
		    {
          // no match, return false
          return false;
		    }
		    
		    // is there a child?
		    if( casted_buf[i].child )
		    {				
          // follow the child.
			    i = casted_buf[i].child;
          
          // if any of the children are an end point, then we have a match
          
          for( typename tree_traits_t::index_t j=i; j!=0; j=casted_buf[j].sibling )
          {
            if( casted_buf[j].is_end() )
            {	
              // found a match!
              
              // go through all children again and see if we can find a longer match
              
					    //typename typename tree_traits_t::comparator_t comparator;
              
              for( typename tree_traits_t::index_t k=i; k!=0; k=casted_buf[k].sibling )
              {
                if( censor_longest(
                      list, 
                      list_len, 
                      replacement,
                      flags, 
                      match_len, 
                      match_item,
                      k,
                      list_pos+1
                      ) )
                {
                  // by definition it is longer than the one we have.
                  perform_censor( list, list_len, replacement, initial_list_pos, list_pos );
                  return true;
                }
              }
              
              // our match is the longest
					    if( flags )
                *flags = casted_buf[j].flags;
              if( match_len )
						    *match_len = list_pos+1;
              if( match_item )
						    *match_item = j;
              perform_censor( list, list_len, replacement, initial_list_pos, list_pos );
              
              return true;						
            }					
          }
          
		    }
		    else
		    {
          // no, so we have a match.
			    if( flags )
            *flags = casted_buf[i].flags;
          if( match_len )
				    *match_len = list_pos+1;
          if( match_item )
				    *match_item = i;
          perform_censor( list, list_len, replacement, initial_list_pos, list_pos );
          return true;
		    }
        
		    // move to the next non ignored item in the list
        while( list_pos<list_len && ignore(list[++list_pos]) );            
      }
      
      // no match
      return false;
    }
    
    
    int extract( 
      typename tree_traits_t::value_t *list, 
      int max_len, 
      typename tree_traits_t::index_t end_leaf_index 
      ) const
    {
	    typename tree_traits_t::leaf_t *casted_buf = (typename tree_traits_t::leaf_t *)buf.get_data();    
      int len=0;
	    typename tree_traits_t::index_t cur_item=end_leaf_index;
	    typename tree_traits_t::value_t *p = list;		
	    
	    if( end_leaf_index<0 )
	    {
        return 0;
	    }
	    
      if( casted_buf[cur_item].is_end() )
      {
        cur_item = casted_buf[cur_item].parent;	
      }
      
      if( cur_item<0 )
      {			
        return 0;
      }		
      
	    while( len<max_len )
	    {
        *p++ = casted_buf[ cur_item ].value;
		    len++;
		    
		    if( casted_buf[cur_item].parent==-1 )
		    {
          // found top of tree
			    // now reverse the order
			    if( len>1 )
			    {
            for( int i=0; i<len/2; ++i )
            {
              typename tree_traits_t::value_t tmp( list[i] );    			    
              list[i]=list[len-i-1];
              list[len-i-1]=tmp;					    
            }
			    }
          return len;
        }
        cur_item = casted_buf[ cur_item ].parent;
	    }
	    return 0;
    }
    
  };
  
}

#endif
