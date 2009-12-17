/*  
 The IF Contextual Media Group Kernel Version 3
 Source Code
 
 Written By Jeff Koftinoff <jeffk@contextualmediagroup.com>
 Copyright (c) 1995-2005
 By Contextual Media Group, Inc.
 http://www.contextualmediagroup.com/
 
 ALL RIGHTS RESERVED.
 
 */
#ifndef if3_HTML_PARSE_HPP
#define if3_HTML_PARSE_HPP

#include "if3_world.hpp"
#include "if3_string.hpp"
#include "if3_buf.hpp"
#include "if3_dynbuf.hpp"
#include "if3_cgi.hpp"

namespace if3 
{
  class html_decode_target_t
  {
  public:
    html_decode_target_t() {}
    virtual ~html_decode_target_t() {}
    
    virtual void content_link( string_t const &url ) = 0;
    virtual void link( string_t const &url ) = 0;
    virtual void img( string_t const &url ) = 0;
    virtual void script( string_t const &data ) = 0;
    virtual void text( string_t const &text ) = 0;
  };

  class html_decode_target_dumper_t
  {
  public:
    html_decode_target_dumper_t( std::ostream &o ) : m_o(o)
    {
    }

    void content_link( string_t const &url )
    {
      m_o << "content link: " << url << "\n";
    }
    
    void link( string_t const &url )
    {
      m_o << "link: " << url << "\n";
    }
    
    void img( string_t const &url )
    {
      m_o << "img: " << url << "\n";
    }
    
    void script( string_t const &data )
    {
      m_o << "script: " << data << "\n";
    }
    
    void text( string_t const &text )
    {
      m_o << "text: " << text << "\n";
    }
    
  private:
    std::ostream &m_o;
  };
  
  class html_decode_base_t
  {
  public:
    virtual ~html_decode_base_t() {}

    virtual void reset() = 0;
    virtual void process( const buf_t &data_in, html_decode_target_t &target ) =0;  
  };
  
  
  class html_decode_t : public html_decode_base_t
  {
  public:
    explicit html_decode_t();
    virtual ~html_decode_t();

    virtual void reset() = 0;
    virtual void process( const buf_t &data_in, html_decode_target_t &target );
    
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
    string_t m_script;
    string_t m_href;
    
    bool m_in_script;
    bool m_in_style;
  };
  
}

#endif
