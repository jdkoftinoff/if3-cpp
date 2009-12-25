/*
 
 The Internet Filter Version 3 Kernel Version 3
 Source Code
 
 Written By Jeff Koftinoff <jeffk@internetfilter.com>
 Copyright (c) 1995-2005
 By Turner and Sons Productions, Inc.
 http://www.internetfilter.com/
 
 ALL RIGHTS RESERVED.
 
 */
#include "if3_world_precompiled.hpp"
#include "if3_html_parse.hpp"
#include "if3_util.hpp"

namespace if3
{

  html_decode_t::html_decode_t()
  {
    reset();
  }

  html_decode_t::~html_decode_t()
  {

  }

  void html_decode_t::reset()
  {
    this->m_href.clear();
    this->m_in_script = false;
    this->m_in_style = false;
    this->m_tagname.clear();
    this->m_text.clear();
    this->m_state = html_decode_t::IN_CONTENT;
  }

  void html_decode_t::process( const buf_t &data_in, html_decode_target_t &target )
  {
    int len = data_in.get_data_length();
    const char * buf = (const char *)data_in.get_data();

    for( int i=0; i<len; ++i )
    {

      char c=buf[i];
      if ( c=='\n' || c=='\r' )
        c=' ';

      switch ( m_state )
      {
        case IN_CONTENT:
        {
          if ( c=='<' )
          {
            m_state=IN_LESS_THAN_FIRST;
          }
          else if ( !m_in_script && !m_in_style && isspace ( c ) )
          {
            m_text.append(&c,1);
            m_state=IN_WHITESPACE;
          }
          else if ( !m_in_script && !m_in_style && c=='&' )
          {
            m_state=IN_CONTENT_AMP;
          }
          else if ( !m_in_script && !m_in_style )
          {
            m_text.append(&c,1);
          }
          break;
        }
        case IN_CONTENT_AMP:
        {
          if ( c==';' )
          {
            m_state=IN_CONTENT;
          }
          else if ( c=='<' )
          {
            m_state=IN_LESS_THAN_FIRST;
          }
          else if ( isspace(c) )
          {
            m_state=IN_CONTENT;
          }
          break;
        }
        case IN_WHITESPACE:
        {
          if ( !isspace ( c ) )
          {
            if ( c=='<' )
            {
              m_state=IN_LESS_THAN_FIRST;
            }
            else if ( c=='&' )
            {
              m_state=IN_CONTENT_AMP;
            }
            else
            {
              m_text.append(&c,1);
              m_state=IN_CONTENT;
            }
          }
          break;
        }
        case IN_LESS_THAN_FIRST:
        {
          if ( c=='"' )
          {
            m_state=IN_TAG_QUOTE;
          }
          else if ( c=='\\' )
          {
            m_state=IN_LESS_THAN_FIRST;
          }
          else if ( c=='!' )
          {
            m_state=IN_LESS_THAN_SECOND;
          }
          else if ( c=='>' )
          {
            m_state=IN_CONTENT;
          }
          else
          {
            m_state=IN_TAGNAME;
            m_tagname.append(&c,1);
          }
          break;
        }
        case IN_LESS_THAN_SECOND:
        {
          if ( c=='"' )
          {
            m_state=IN_TAG_QUOTE;
          }
          else if ( c=='-' )
          {
            m_state=IN_LESS_THAN_THIRD;
          }
          else if ( c=='>' )
          {
            m_state=IN_CONTENT;
          }
          else
          {
            m_state=IN_TAG;
          }
          break;
        }
        case IN_LESS_THAN_THIRD:
        {
          if ( c=='"' )
          {
            m_state=IN_TAG_QUOTE;
          }
          else
            if ( c=='-' )
            {
              m_state=IN_COMMENT;
            }
            else if ( c=='>' )
            {
              m_state=IN_CONTENT;
            }
            else
            {
              m_state=IN_TAG;
            }
          break;
        }
        case IN_TAGNAME:
        {
          if ( c=='"' )
          {
            m_state=IN_TAG_QUOTE;
          }
          else
            if ( c=='>' )
            {
              m_state=IN_CONTENT;
              if ( util::compare_case_insensitive( m_tagname, "SCRIPT" ) ==0 )
              {
                m_in_script=true;
              }
              else if ( util::compare_case_insensitive( m_tagname, "/SCRIPT" ) ==0 )
              {
                m_in_script=false;
              }
              else if ( util::compare_case_insensitive( m_tagname, "STYLE" ) ==0 )
              {
                m_in_style=true;
              }
              else if ( util::compare_case_insensitive( m_tagname, "/STYLE" ) ==0 )
              {
                m_in_style=false;
              }
              m_tagname.clear();
            }
            else if ( isspace ( c ) )
            {
              m_state=IN_TAG;
            }
            else
            {
              m_tagname.append( &c, 1 );
            }

          break;
        }

        case IN_TAG:
        {
          if ( c=='"' )
          {
            m_state=IN_TAG_QUOTE;
          }
          else
            if ( c=='>' )
            {
              m_state=IN_CONTENT;
              if ( util::compare_case_insensitive( m_tagname, "SCRIPT" ) ==0 )
              {
                m_in_script=true;
              }
              if ( util::compare_case_insensitive( m_tagname, "/SCRIPT" ) ==0 )
              {
                m_in_script=false;
              }
              else if ( util::compare_case_insensitive( m_tagname, "STYLE" ) ==0 )
              {
                m_in_style=true;
              }
              else if ( util::compare_case_insensitive( m_tagname, "/STYLE" ) ==0 )
              {
                m_in_style=false;
              }
              m_tagname.clear();
            }
          break;
        }
        case IN_COMMENT:
        {
          if ( c=='"' )
          {
            m_state=IN_COMMENT_QUOTE;
          }
          else
            if ( c=='-' )
            {
              m_state=IN_COMMENT_END_FIRST;
            }
          break;
        }
        case IN_COMMENT_END_FIRST:
        {
          if ( c=='"' )
          {
            m_state=IN_COMMENT_QUOTE;
          }
          else
            if ( c=='-' )
            {
              m_state=IN_COMMENT_END_SECOND;
            }
            else
            {
              m_state=IN_COMMENT;
            }
          break;
        }
        case IN_COMMENT_END_SECOND:
        {
          if ( c=='"' )
          {
            m_state=IN_COMMENT_QUOTE;
          }
          else
            if ( c=='>' )
            {
              m_state=IN_CONTENT;
            }
            else
            {
              m_state=IN_COMMENT;
            }
          break;
        }
        case IN_TAG_QUOTE:
        {
          if ( c=='"' )
          {
            m_state=IN_TAG;
          }
          break;
        }
        case IN_COMMENT_QUOTE:
        {
          if ( c=='"' )
          {
            m_state=IN_COMMENT;
          }
          break;
        }
      }
    }
  }
}
