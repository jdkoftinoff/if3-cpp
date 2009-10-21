/*
 
 The IF Contextual Media Group Kernel Version 3
 Source Code
 
 Written By Jeff Koftinoff <jeffk@contextualmediagroup.com>
 Copyright (c) 1995-2005
 By Contextual Media Group, Inc.
 http://www.contextualmediagroup.com/
 
 ALL RIGHTS RESERVED.
 
 */

#include "ifcmg_world_precompiled.hpp"
#include "ifcmg_string.hpp"
#include "ifcmg_util.hpp"
#include "ifcmg_compile.hpp"
#include "ifcmg_net.hpp"
#include "ifcmg_httpd.hpp"
#include "ifcmg_syslog.hpp"
#include "ifcmg_daemon.hpp"

using namespace ifcmg;

int main( int argc, char **argv )
{
  net_init();
  
  daemonize(
            true, 
            string_t(argv[0]), 
            string_t(""), 
            string_t(argv[0])+".pid", 
            string_t("")
            );
  
  logger->log_info("Started");

  try 
  {
    db_t db;
    httpd_fork_server_t server(0,"9999", new httpd_session_redirector_t(db) );
    server.run();
    // todo: wait for all subprocesses to exit
    
  }
  catch (std::exception &e) 
  {
    logger->log_error("Exception caught: %s", e.what() );                    
  }
  logger->log_info("Ending");
  daemon_end();
}
