/*

 The Internet Filter Version 3 Kernel Version 3
 Source Code

 Written By Jeff Koftinoff <jeffk@internetfilter.com>
 Copyright (c) 1995-2005
 By Turner and Sons Productions, Inc.
 http://www.internetfilter.com/

 ALL RIGHTS RESERVED.

 */
#ifndef IF3_WORLD_PRECOMPILED_HPP
#define IF3_WORLD_PRECOMPILED_HPP

#if defined(__APPLE__)
# define IF3_CONFIG_MACOSX
#elif defined(__linux__)
# define IF3_CONFIG_LINUX
#elif defined(WIN32) || defined( _MSC_VER )
# define IF3_CONFIG_WIN32
#endif


  // platform options
#if defined(IF3_CONFIG_WIN32)
# define IF3_CONFIG_NEW
# undef IF3_CONFIG_NEWNOTHROW
# undef IF3_CONFIG_THROW
#elif defined(IF3_CONFIG_MACOSX)
# define IF3_CONFIG_NEW
# undef IF3_CONFIG_NEWNOTHROW
# undef IF3_CONFIG_THROW
# define IF3_CONFIG_POSIX
# define IF3_CONFIG_PTHREADS
# define IF3_CONFIG_FORK
#elif defined(IF3_CONFIG_LINUX)
# define IF3_CONFIG_NEW
# undef IF3_CONFIG_NEWNOTHROW
# undef IF3_CONFIG_THROW
# define IF3_CONFIG_POSIX
# define IF3_CONFIG_PTHREADS
# define IF3_CONFIG_FORK
#else
# error please define one of IF3_CONFIG_WIN32 IF3_CONFIG_MACOSX IF3_CONFIG_LINUX
#endif


  // for win32 we need windows.h and friends
#if defined(IF3_CONFIG_WIN32)

#ifndef _CRT_SECURE_NO_DEPRECATE
# define _CRT_SECURE_NO_DEPRECATE 1
#endif

#ifndef _WIN32_WINNT
# define _WIN32_WINNT 0x0500
# undef WINVER
# define WINVER _WIN32_WINNT
#endif

# include <winsock2.h>
# include <windows.h>
# include <ws2tcpip.h>
# include <io.h>
# include <process.h>

typedef long ssize_t;
typedef long long int64_t;
typedef unsigned long long uint64_t;
typedef int int32_t;
typedef unsigned int uint32_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef char int8_t;
typedef unsigned char uint8_t;

#define strtoll(p, e, b) _strtoi64(p, e, b) 

#endif

#if defined(IF3_CONFIG_PTHREADS)
# ifndef __STL_PTHREADS
#  define __STL_PTHREADS
# endif
# define _PTHREADS 1
# include <pthread.h>
#endif

#if defined(IF3_CONFIG_POSIX)
# include <signal.h>
# include <sys/time.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <sys/mman.h>
# include <sys/stat.h>
# include <netdb.h>
# include <unistd.h>
# include <netinet/in.h>
# include <sys/poll.h>
# include <unistd.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <net/if.h>
# include <sys/resource.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <sys/types.h>
# include <syslog.h>
# include <pwd.h>
#endif

  // include the normal standard c library stuff
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <stdarg.h>


#ifdef __cplusplus
# if defined(IF3_CONFIG_NEW)
#  include <new>
# endif

# include <string>
# include <iostream>
# include <sstream>
# include <list>
# include <valarray>
# include <vector>
# include <algorithm>
# include <exception>
# include <stdexcept>
# include <cstring>
# include <cstdlib>

# if defined(IF3_CONFIG_NEWNOTHROW)
#  define newnothrow new (std::nothrow)
# else
#  define newnothrow new
# endif

# if defined(IF3_CONFIG_THROW)
#  define if3_throw( a, b ) throw( a(b) )
# elif defined(IF3_CONFIG_POSIX)
#  define if3_throw( a, b ) do { fprintf(stderr,"%s(\"%s\")",#a,b); abort(); } while(0)
# elif defined(IF3_CONFIG_WIN32)
#  define if3_throw( a, b ) do { fprintf(stderr,"%s(\"%s\")",#a,b); abort(); } while(0)
# endif
#endif

namespace if3
{
  template< typename T1, typename T2 > inline T1 lexical_cast ( const T2 &v )
  {
    T1 newval;
    std::stringstream os;

    if ( ! ( os << v ) || ! ( os >> newval ) )
    {
      if3_throw( std::invalid_argument, "lexical_cast" );
    }

    return newval;
  }


}

#endif

