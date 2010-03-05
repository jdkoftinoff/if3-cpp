#ifndef IF3_HTTP_HPP
#define IF3_HTTP_HPP

#include "if3_world.hpp"
#include "if3_net.hpp"
#include "if3_cgi.hpp"
#include "if3_string.hpp"
#include "if3_dynbuf.hpp"
#include <map>

namespace if3
{
  namespace http
  {
    typedef string_t key_t;
    typedef string_t value_t;

    typedef std::map< key_t, value_t > items_t;

    static inline key_t key_tolower( const key_t &a )
    {
      key_t result = a;
      for( key_t::size_type i=0; i<result.length(); ++i )
      {
        result[i] = ::tolower( result[i] );
      }
      return result;
    }

    static inline bool key_equal( const key_t &a, const key_t &b )
    {
      bool r=false;
      if( a.length() == b.length() )
      {
        int sz=a.length();
        int i;
        for( i=0; i<sz; ++i )
        {
          if( a[i] != b[i] )
            break;
        }
        if( i==sz )
          r=true;
      }
      return r;
    }


    bool flatten_item( buf_t &outbuf, const key_t &key, const value_t &val );
    bool flatten_items( buf_t &outbuf, items_t const &items );

    class response_t
    {
    public:
      response_t() :
        m_items(), m_content(), m_code(404)
      {
      }
#if 0
      response_t( response_t const &o )
      : m_items( o.m_items ), m_content( o.m_content ), m_code( o.m_code )
      {
      }

      response_t const & operator = ( response_t const & o )
      {
        if( &o != this )
        {
          m_items = o.m_items;
          m_content = o.m_content;
          m_code = o.m_code;
        }
        return *this;
      }
#endif

      void clear() { erase(); }

      void erase()
      {
        m_items.clear();
        m_content.clear();
        m_code = 404;
      }

      void flatten( buf_t &outbuf )
      {
        flatten_items( outbuf, m_items );
      }

      void remove_header( key_t k )
      {
        items_t::iterator i = m_items.find( key_tolower(k) );
        if( i != m_items.end() )
        {
          m_items.erase( i );
        }
      }

      void add_header( key_t k, value_t v )
      {
        value_t cur_value;
        k=key_tolower(k);

        items_t::iterator i = m_items.find( k );
        if( i != m_items.end() )
        {
          cur_value = i->second;
          m_items.erase( i );
        }

        if( cur_value.length()>0 )
        {
          cur_value = cur_value + ", " + v;
        }

        m_items.insert( std::make_pair(k, cur_value) );
      }

      value_t header( key_t k )
      {
        items_t::iterator i = m_items.find( key_tolower(k) );
        if( i != m_items.end() )
        {
          return i->second;
        }
        else
        {
          return value_t();
        }
      }

      items_t &items() { return m_items; }
      items_t const &items() const { return m_items; }

      buf_t &content() { return m_content; }
      buf_t const &content() const { return m_content; }

      int code() { return m_code; }
      int code() const { return m_code; }
      void code( int c ) { m_code = c; }

      void fill_content_length( value_t mimetype )
      {
        add_header("Content-Type", mimetype );
        remove_header("Content-Length");
        add_header("Content-Length", lexical_cast<value_t>( m_content.get_data_length() ) );
      }

    private:
      items_t m_items;
      dynbuf_t m_content;
      int m_code;
    };

    class request_t
    {
    public:
      request_t()
      {
      }
#if 0
      request_t( request_t const &o )
      :
        m_type( o.m_type ),
        m_url( o.m_url ),
        m_http_version( o.m_http_version ),
        m_items( o.m_items ),
        m_content( o.m_content )
      {
      }

      request_t const & operator = ( request_t const &o )
      {
        if( &o != this )
        {
          m_type = o.m_type;
          m_url = o.m_url;
          m_http_version = o.m_http_version;
          m_items = o.m_items;
          m_content = o.m_content;
        }
        return *this;
      }
#endif
      bool parse( buf_t const &inbuf, int start_pos=0, int *end_pos=0 );
      bool flatten( buf_t &outbuf );

      void clear()
      {
        m_type.clear();
        m_url.clear();
        m_http_version.clear();
        m_items.clear();
        m_content.clear();
      }

      void add_header( key_t k, value_t v )
      {
        value_t cur_value;

        items_t::iterator i = m_items.find( key_tolower(k) );
        if( i != m_items.end() )
        {
          cur_value = i->second;
          m_items.erase( i );
        }

        if( cur_value.length()>0 )
        {
          cur_value = cur_value + ", " + v;
        }

        m_items.insert( std::make_pair(k, cur_value) );
      }


      void remove_header( key_t k )
      {
        items_t::iterator i = m_items.find( key_tolower(k) );
        if( i != m_items.end() )
        {
          m_items.erase( i );
        }
      }

      value_t header( key_t k )
      {
        items_t::iterator i = m_items.find( key_tolower(k) );
        if( i != m_items.end() )
        {
          return i->second;
        }
        else
        {
          return value_t();
        }
      }

      string_t type() const
      {
        return m_type;
      }
      string_t url() const
      {
        return m_url;
      }
      string_t http_version() const
      {
        return m_http_version;
      }
      items_t items() const
      {
        return m_items;
      }
      dynbuf_t const &content() const
      {
        return m_content;
      }

      dynbuf_t &content()
      {
        return m_content;
      }

    private:
      string_t m_type;
      string_t m_url;
      string_t m_http_version;
      items_t m_items;
      dynbuf_t m_content;

      typedef std::list< string_t > multiline_item_t;

      bool parse_request( const buf_t &inbuf, int start_pos, int *end_pos );
      bool parse_item( const multiline_item_t &item );
      bool parse_items( const buf_t &inbuf, int start_pos, int *end_pos );
    };
  }

}

#endif

