#include "if3_world_precompiled.hpp"
#include "if3_httpd.hpp"

namespace if3
{
  namespace httpd
  {
    void session_t::run(net::socket_handle_t s, struct sockaddr *addr, socklen_t addrlen)
    {
      dynbuf_t xmit_buf;
      net::set_socket_nonblocking( s );
      try
      {
        http::request_t request;
        bool got_header = false;

        dynbuf_t receive_header_buf;
        got_header = net::read_until_pattern(receive_header_buf, request.content(), s, string_t("\r\n\r\n"));

        if (got_header)
        {
          IF3_LOG_DEBUG("about to parse request");
#if IF3_CONFIG_DEBUG
          fprintf(stdout,"receive_header_buf len:%d", receive_header_buf.get_data_length() );
          fwrite(receive_header_buf.get_data(), receive_header_buf.get_data_length(), 1, stdout );
          fprintf(stdout,"receive_content_buf len:%d", request.content().get_data_length() );
          fwrite(request.content().get_data(), request.content().get_data_length(), 1, stdout );
#endif
          if (request.parse(receive_header_buf))
          {
            IF3_LOG_DEBUG( "Request parsed: '%s' '%s' '%s'", request.type().c_str(), request.url().c_str(), request.http_version().c_str() );
            IF3_LOG_DEBUG( "Request Host, Port: '%s' '%s'", request.header("host").c_str(), request.header("port").c_str() );

            // TODO: read the rest of the incoming content for post or put requests
            http::response_t response;

            try
            {
              if (request.type() == "GET")
              {
                response.code(m_top_handler.get(request, response));
              }
              else if (request.type() == "POST")
              {
                response.code(m_top_handler.post(request, response));
              }
              else if (request.type() == "PUT")
              {
                response.code(m_top_handler.put(request, response));
              }
              else
              {
                throw( httpd::unimplemented_error("unknown request type") );
              }
            }
            catch ( httpd::unimplemented_error &e )
            {
              response.content().clear();
              response.content().append_from_string( "<html><body>404: Not Found</body></html>\n" );
              response.code( 404 );
              response.fill_content_length("text/html");
            }
            catch ( std::runtime_error &e )
            {
              logger->log_error( "top handler generated exception: %s", e.what() );
              response.content().clear();
              response.content().append_form(
                  "<html><body><p>404: Not Found</p><p>Runtime Error:</p><ul>%s</ul></body></html>\n",
                  e.what() // TODO: Encode as html
                  );
              response.code( 404 );
              response.fill_content_length("text/html");
            }

            dynbuf_t outgoing_header;
            response.flatten( outgoing_header );
            if( net::write_block(outgoing_header,s,2048) )
            {
              net::write_block( response.content(),s,2048 );
            }
          }
        }
      }
      catch( std::exception &e )
      {
        logger->log_alert("Exception: %s", e.what());
      }
      close(s);
    }
  }
}
