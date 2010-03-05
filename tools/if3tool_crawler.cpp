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
#include "if3_net.hpp"
#include "if3_syslog.hpp"
#include "if3_daemon.hpp"
#include "if3_crawler.hpp"

int main( int argc, char **argv )
{
  if3::net::net_init();

  if3::daemon::daemonize(
                         false,
                         if3::string_t(argv[0]),
                         if3::string_t(""),
                         if3::string_t(argv[0])+".pid",
                         if3::string_t("")
                         );

  if3::logger->log_info("Started");

  try
  {

  }
  catch (std::exception &e)
  {
    if3::logger->log_error("Exception caught: %s", e.what() );
  }
  if3::logger->log_info("Ending");
  if3::daemon::end();
}
