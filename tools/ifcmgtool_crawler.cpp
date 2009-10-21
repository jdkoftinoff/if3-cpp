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
#include "ifcmg_net.hpp"
#include "ifcmg_syslog.hpp"
#include "ifcmg_daemon.hpp"
#include "ifcmg_crawler.hpp"

using namespace ifcmg;

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
