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
  bool real_daemon = false;
  string_t identity=argv[0];
  filename_t home_dir=".";
  filename_t pid_file=filename_t(argv[0])+".pid";
  filename_t log_file="";

  if( argc==1 )
  {
    std::cout << "Usage:\n\t" << argv[0] << " [real_daemon] [identity] [home_dir] [pid_file] [log_file]\n\n";
    std::cout << "Defaults:\n\t" << argv[0] << " '" << real_daemon << "' '" << identity << "' '" <<
        home_dir << "' '" << pid_file << "' '" << log_file << "'\n";
    exit(1);
  }

  if( argc>1 )
  {
    real_daemon = lexical_cast<bool>(argv[1]);
  }
  if( argc>2 )
  {
    identity = argv[2];
  }
  if( argc>3 )
  {
    home_dir = argv[3];
  }
  if( argc>4 )
  {
    pid_file = argv[4];
  }
  if( argc>5 )
  {
    log_file = argv[5];
  }

  if3::daemon::daemonize(
                    real_daemon,
                    identity,
                    home_dir,
                    pid_file,
                    log_file
                    );

  logger->log_info("Started");

  try
  {
    httpd::server_context_t context;
    httpd::request_handler_t handler;
    httpd::session_t session(context,handler);

    net::tcp_fork_server_t server(0, "9999", session, 1, "httpd session");

    server.run();

    if3::daemon::wait_for_children_to_finish("parent");
  }
  catch (std::exception &e)
  {
    logger->log_error("Exception caught: %s", e.what() );
  }
  IF3_LOG_DEBUG("Ending");
  if3::daemon::end();
}
