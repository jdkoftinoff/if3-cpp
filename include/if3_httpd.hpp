#ifndef IF3_HTTPD_HPP
#define IF3_HTTPD_HPP

#include "if3_world.hpp"
#include "if3_net.hpp"
#include "if3_cgi.hpp"
#include "if3_string.hpp"
#include "if3_dynbuf.hpp"
#include "if3_http.hpp"

namespace if3
{
  namespace httpd
  {
    class server_context_t : public net::tcp_server_context_t
    {
    public:
    };

    class unimplemented_error : public std::runtime_error
    {
    public:
      unimplemented_error( std::string name ) : runtime_error( name )
      {
      }
    };

    class request_handler_t
    {
    public:
      request_handler_t() {}
      virtual ~request_handler_t() {}

      virtual int get(
          http::request_t const &req_header,
          http::response_t &response_header
          )
      {
        throw unimplemented_error("get");
        return 404;
      }

      virtual int put(
          http::request_t const &req_header,
          http::response_t &response_header
          )
      {
        throw unimplemented_error("put");
        return 404;
      }

      virtual int post(
          http::request_t const &req_header,
          http::response_t &response_header
          )
      {
        throw unimplemented_error("post");
        return 404;
      }

    };

    class session_t : public net::tcp_server_session_base_t
    {
    public:
      session_t( server_context_t &context, request_handler_t &top_handler )
        : m_context( context ),
          m_top_handler( top_handler )
      {
      }
      virtual ~session_t() {}

      void run(
               net::socket_handle_t s,
               struct sockaddr *addr,
               socklen_t addrlen
               );

    private:

      server_context_t &m_context;
      request_handler_t &m_top_handler;
    };

  }
}

#endif

