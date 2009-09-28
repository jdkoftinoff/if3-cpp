/*
  
The IF Contextual Media Group Kernel Version 3
Source Code

Written By Jeff Koftinoff <jeffk@contextualmediagroup.com>
Copyright (c) 1995-2005
By Contextual Media Group, Inc.
http://www.contextualmediagroup.com/

ALL RIGHTS RESERVED.

*/
#ifndef IFCMG_WORLD_H
#define IFCMG_WORLD_H

#if defined(__APPLE__)
# define IFCMG_CONFIG_MACOSX
#elif defined(__linux__)
# define IFCMG_CONFIG_LINUX
#elif defined(WIN32) || defined( _MSC_VER )
# define IFCMG_CONFIG_WIN32
#endif


// platform options
#if defined(IFCMG_CONFIG_WIN32)
# define IFCMG_CONFIG_NEW
# undef IFCMG_CONFIG_NEWNOTHROW
# undef IFCMG_CONFIG_THROW
#elif defined(IFCMG_CONFIG_MACOSX)
# define IFCMG_CONFIG_NEW
# undef IFCMG_CONFIG_NEWNOTHROW
# undef IFCMG_CONFIG_THROW
# define IFCMG_CONFIG_POSIX 
# define IFCMG_CONFIG_PTHREADS 
# define IFCMG_CONFIG_FORK 
#elif defined(IFCMG_CONFIG_LINUX)
# define IFCMG_CONFIG_NEW
# undef IFCMG_CONFIG_NEWNOTHROW
# undef IFCMG_CONFIG_THROW
# define IFCMG_CONFIG_POSIX 
# define IFCMG_CONFIG_PTHREADS 
# define IFCMG_CONFIG_FORK 
#else
# error please define one of IFCMG_CONFIG_WIN32 IFCMG_CONFIG_MACOSX IFCMG_CONFIG_LINUX
#endif


// for win32 we need windows.h and friends
#if defined(IFCMG_CONFIG_WIN32)
# include <windows.h>
# include <winsock.h>
# include <io.h>
# include <process.h>
#endif

#if defined(IFCMG_CONFIG_PTHREADS)
# ifndef __STL_PTHREADS
#  define __STL_PTHREADS
# endif
# define _PTHREADS 1
# include <pthread.h>
#endif

#if defined(IFCMG_CONFIG_POSIX)
# include <signal.h>
# include <sys/time.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <sys/mman.h>
# include <sys/stat.h>
# include <unistd.h>
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
# if defined(IFCMG_CONFIG_NEW)
#  include <new>
# endif

# include <string>
# include <iostream>
# include <sstream>
# if defined(IFCMG_CONFIG_NEWNOTHROW)
#  define newnothrow new (std::nothrow)
# else
#  define newnothrow new
# endif

# if defined(IFCMG_CONFIG_THROW)
#  define ifcmg_throw( a ) throw( a )
# elif defined(IFCMG_CONFIG_POSIX)
#  define ifcmg_throw( a ) do { fprintf(stderr,"%s",a); abort(); } while(0)
# elif defined(IFCMG_CONFIG_WIN32)
#  define ifcmg_throw( a ) do { fprintf(stderr,"%s",a); abort(); } while(0)
# endif
#endif

#endif

