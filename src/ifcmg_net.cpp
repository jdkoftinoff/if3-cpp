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


namespace ifcmg
{
#ifdef _WIN32
  void net_init()
  {
    WSADATA wsaData;
    WORD version;
    int error;
    version = MAKEWORD( 2, 2 );
    error = WSAStartup( version, &wsaData );
    /* check for error */
    if ( error != 0 )
    {
      /* error occured */
      throw("Can't initialize Winsock");
    }
    /* check for correct version */
    if ( version != wsaData.wVersion )
    {
      /* incorrect WinSock version */
      ifcmg_throw(std::runtime_error,"Incorrect Winsock version");
    }
    /* WinSock has been initialized */    
  }
#else
  void net_init()
  {
    signal( SIGPIPE, SIG_IGN );    
  }
#endif
  
}
