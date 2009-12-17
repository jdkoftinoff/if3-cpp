/*
 
 The IF Contextual Media Group Kernel Version 3
 Source Code
 
 Written By Jeff Koftinoff <jeffk@contextualmediagroup.com>
 Copyright (c) 1995-2005
 By Contextual Media Group, Inc.
 http://www.contextualmediagroup.com/
 
 ALL RIGHTS RESERVED.
 
 */

#include "if3_world_precompiled.hpp"
#include "if3_net.hpp"
#include "if3_syslog.hpp"
#include "if3_daemon.hpp"
#include "if3_crawler.hpp"

using namespace if3;

int main( int argc, char **argv )
{
  net_init();
  
  daemonize(
            false, 
            string_t(argv[0]), 
            string_t(""), 
            string_t(argv[0])+".pid", 
            string_t("")
            );
  
  logger->log_info("Started");

  try 
  {
    
  }
  catch (std::exception &e) 
  {
    logger->log_error("Exception caught: %s", e.what() );                    
  }
  logger->log_info("Ending");
  daemon_end();
}
