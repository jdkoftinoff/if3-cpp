/*
  
The IF Contextual Media Group Kernel Version 3
Source Code

Written By Jeff Koftinoff <jeffk@contextualmediagroup.com>
Copyright (c) 1995-2005
By Contextual Media Group, Inc.
http://www.contextualmediagroup.com/

ALL RIGHTS RESERVED.

*/
#include "ifcmg_world.h"
#include "ifcmg_pattern_expander.h"

namespace ifcmg
{
/// Manages the recursive expansion of phrase patterns
  struct expstring_list_t
  {
  private:
    explicit expstring_list_t( const expstring_list_t & );
    const expstring_list_t & operator = ( const expstring_list_t & );
  public:
    ///
    expstring_list_t()
      : num(0), next(0)
    {
    }
    
    
    ///
    ~expstring_list_t() { delete next; }
    
    
    
    ///
    void add_string( const string_t &s )
    {
      if( num<16 )
      {
        strings[num++]=s;
      }
    }
    
    
    /**
       Adds a single string as an optional word for this phrase section. 
    */
    
    ///
    void exp_strings( const string_t &pre, pattern_target_t &dest, short flags, bool remove=false )	   
      /**
         Recursively forms all combinations of phrases from this point
         in the phrase.  Adds the phrases to the specified WordList object.
         All the phrases will be prefixed by the 'pre' string.
      */
    {
      for( int i=0; i<num; ++i )
      {
        string_t new_pre;
        
        if( next )
        {
          new_pre = pre;
          new_pre += strings[i];
          
          next->exp_strings( new_pre, dest, flags );
        }
        else
        {
          new_pre = pre;
          new_pre += strings[i];
          
          if( remove )
            dest.remove( new_pre );
          else
            dest.add( new_pre, flags );
        }
      }
      if( num==0 )
      {
        if( next )
        {
          next->exp_strings( pre, dest, flags );
        }
      }
    }
    
    
    /// number of strings in this section
    int num;
    /// Points to the next string list
    expstring_list_t *next;
    
    /// space for the actual strings for this section.
    string_t strings[16];
  };
  
  
  bool pattern_expander_standard_t::expand(
    const string_t &in_,
    pattern_target_t &target,
    short flags,
    bool remove,
    const string_t &prefix
    )
  {
    string_t in( prefix );
    
    in += in_;
    
    // if the phrase is not bracketed, then
    // there is only one 'word' - no optional phrase 
    // variations
    // 
    if( in[0]!='[' && in[0]!='{' )
    {
      target.add( in, flags );
      return true;
    }
    
    // find the length of the string including null
    // 
    int l=in.length()+1;
    if( l>4095 )
      return false;
    
    // p is what we use to mark each possible end-of subphrase marker "," and "]" and "}"
    // with a null
    // 
    char raw_line[4096];
    strcpy( raw_line, in.c_str() );
    
    char *p=raw_line;
    
    // w is what we use to keep track of the beginning 
    // of each 'word'
    // 
    char *w=0;
    
    expstring_list_t top;
    expstring_list_t *cur=&top;
    bool in_bracket_section=false;
    
    for( int a=0; a<l; ++a, ++p )
    {			   
      if( *p=='\0' )
      {
        // is it a string end without an ending bracket?
        if( in_bracket_section==true )
        {          
          // yup, assume ending bracket
          if( w )
          {
            cur->add_string(string_t(w));
            w=0;
          }				
        }
        
        break;
      }
      
      // is it the beginning of a subphrase section?
      // 
      if( *p=='[' || *p=='{' )
      {
        in_bracket_section=true;
        // yes, kill this character with a null
        // 	
        *p='\0';
        
        // if we have a subphrase before this then add it.
        // 
        if( w )
        {
          cur->add_string( string_t(w) );
          w=0;
        }
        
        // remember the next character is the beginning of the next subphrase
        // 
        w=p+1;
        
        // allocate a newnothrow string list object for this section
        cur->next=newnothrow expstring_list_t;
        cur=cur->next;
        continue;		
      }
      else 
        // is it the end of a subphrase section?
        if( *p==']' || *p=='}' )
        {
          in_bracket_section=false;
          // yes, kill this character with a null
          *p='\0';
          
          // if this characters ends the last subphrase then add it to the current list
          // 
          if( w )
          {
            cur->add_string( string_t(w) );
            w=0;
          }
          continue;
        }
        else
          // is is the end of a single subphrase?
          // 
          if( *p==',' && in_bracket_section )
          {
            // yes, kill this character with a null
            *p='\0';
            
            // if this character ends the last subphrase then add it to the current list
            if( w )
            {
              cur->add_string( string_t(w) );
            }
            
            // remember the next character is the beginning of the next subphrase
            // 
            w=p+1;
            
            continue;
          }
          else if( in_bracket_section==false )
          {
            // assume open bracket if it is missing
            in_bracket_section=true;
            w=p;
            cur->next=newnothrow expstring_list_t;
            cur=cur->next;
          }		
      
    }
    
    top.exp_strings(string_t(""),target,flags,remove);
    return true;
  }
  
}
