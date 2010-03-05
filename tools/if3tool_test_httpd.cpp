#include "if3_world_precompiled.hpp"
#include "if3_string.hpp"
#include "if3_util.hpp"
#include "if3_compile.hpp"
#include "if3_net.hpp"
#include "if3_httpd.hpp"
#include "if3_syslog.hpp"
#include "if3_daemon.hpp"

using namespace if3;

int main( int argc, char **argv )
{
  net::net_init();

  if3::daemon::daemonize(
            false,
            string_t(argv[0]),
            string_t(""),
            string_t(argv[0])+".pid",
            string_t("")
            );

  logger->log_info("Started");

  try
  {
    httpd::server_context_t context;
    httpd::request_handler_t handler;
    httpd::session_t session(context,handler);

    session.run( STDIN_FILENO, 0, 0 );
  }
  catch (std::exception &e)
  {
    logger->log_error("Exception caught: %s", e.what() );
  }
  logger->log_info("Ending");
  if3::daemon::end();
}
