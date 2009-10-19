/*  
 The IF Contextual Media Group Kernel Version 3
 Source Code
 
 Written By Jeff Koftinoff <jeffk@contextualmediagroup.com>
 Copyright (c) 1995-2005
 By Contextual Media Group, Inc.
 http://www.contextualmediagroup.com/
 
 ALL RIGHTS RESERVED.
 
 */
#ifndef IFCMG_HTML_PARSE_HPP
#define IFCMG_HTML_PARSE_HPP

#include "ifcmg_world.hpp"
#include "ifcmg_string.hpp"
#include "ifcmg_buf.hpp"
#include "ifcmg_dynbuf.hpp"
#include "ifcmg_cgi.hpp"

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
    } m_state;
    string_t m_tagname;
    bool m_in_script;
    bool m_in_style;
  };
  
  
  class html_decode_to_links_t : public html_decode_base_t
  {
  public:
    explicit html_decode_to_links_t();
    virtual ~html_decode_to_links_t();
    
    void clear();
    
    virtual void process( const buf_t &in, dynbuf_t &out );
    
  protected:
    
    virtual void tag_found( const buf_t &in, size_t start_point, size_t len, dynbuf_t &out );
    virtual void content_found( const buf_t &in, size_t start_point, size_t len, dynbuf_t &out );
    
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
