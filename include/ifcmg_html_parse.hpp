/*  
 The IF Contextual Media Group Kernel Version 3
 Source Code
 
 Written By Jeff Koftinoff <jeffk@contextualmediagroup.com>
 Copyright (c) 1995-2005
 By Contextual Media Group, Inc.
 http://www.contextualmediagroup.com/
 
 ALL RIGHTS RESERVED.
 
 */
#ifndef IFCMG_HTML_PARSE_H
#define IFCMG_HTML_PARSE_H

#include "ifcmg_world.hpp"
#include "ifcmg_string.hpp"
#include "ifcmg_buf.hpp"

namespace ifcmg 
{
  
  class html_decode_base_t
  {
  public:
    virtual ~html_decode_base_t() {}
    
    virtual void clear() = 0;
    virtual void process( const buf_t &in, dynbuf_t &out ) =0;  
  };
  
  
  class html_decode_t : public html_decode_base_t
  {
  public:
    explicit html_decode_t();
    virtual ~html_decode_t();
    
    void clear();
    virtual void process( const buf_t &in, dynbuf_t &out );
    
  protected:
    enum
    {
      IN_CONTENT,
      IN_CONTENT_AMP,
      IN_WHITESPACE,
      IN_LESS_THAN_FIRST,
      IN_LESS_THAN_SECOND,
      IN_LESS_THAN_THIRD,
      IN_TAG,	
      IN_TAGNAME,
      IN_COMMENT,
      IN_COMMENT_END_FIRST,
      IN_COMMENT_END_SECOND,
      IN_COMMENT_QUOTE,
      IN_TAG_QUOTE
    } state;
    jdk_str<256> tagname;
    bool in_script;
    bool in_style;
  };
  
  
  class html_decode_t_to_links : public html_decode_base_t
  {
  public:
    explicit html_decode_t_to_links();
    virtual ~html_decode_t_to_links();
    
    void clear();
    
    virtual void process( const buf_t &in, dynbuf_t &out );
    
  protected:
    
    virtual void tagfound( const buf_t &in, size_t start_point, size_t len, dynbuf_t &out );
    virtual void contentfound( const buf_t &in, size_t start_point, size_t len, dynbuf_t &out );
    
    enum
    {
      IN_CONTENT,
      IN_CONTENT_AMP,
      IN_WHITESPACE,
      IN_LESS_THAN_FIRST,
      IN_LESS_THAN_SECOND,
      IN_LESS_THAN_THIRD,
      IN_TAG,	
      IN_TAGNAME,
      IN_COMMENT,
      IN_COMMENT_END_FIRST,
      IN_COMMENT_END_SECOND,
      IN_COMMENT_QUOTE,
      IN_TAG_QUOTE,
      IN_TAG_SPACE,
      IN_TAG_PROPERTY
    } m_state;
    
    string_t m_tag_name;
    string_t m_tag_property;
    string_t m_tag_property_value;
    bool m_in_script;
    bool m_in_style;
    size_t m_tag_start_point;
  };  
}

#endif
