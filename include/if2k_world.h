/*
  
The Internet Filter IF2K Version 2
Source Code

Written By Jeff Koftinoff <jeffk@jdkoftinoff.com>
Copyright (c) 1995-2005
By Turner and Sons Productions Ltd.
http://www.internetfilter.com/

ALL RIGHTS RESERVED.

*/
#ifndef _IF2K_WORLD_H
#define _IF2K_WORLD_H

#if defined(__APPLE__)
# define IF2K_CONFIG_MACOSX
#elif defined(__linux__)
# define IF2K_CONFIG_LINUX
#elif defined(WIN32) || defined( _MSC_VER )
# define IF2K_CONFIG_WIN32
#endif


// platform options
#if defined(IF2K_CONFIG_WIN32)
# define IF2K_CONFIG_NEW
# undef IF2K_CONFIG_NEWNOTHROW
# undef IF2K_CONFIG_THROW
#elif defined(IF2K_CONFIG_MACOSX)
# define IF2K_CONFIG_NEW
# undef IF2K_CONFIG_NEWNOTHROW
# undef IF2K_CONFIG_THROW
# define IF2K_CONFIG_POSIX 
# define IF2K_CONFIG_PTHREADS 
# define IF2K_CONFIG_FORK 
#elif defined(IF2K_CONFIG_LINUX)
# define IF2K_CONFIG_NEW
# undef IF2K_CONFIG_NEWNOTHROW
# undef IF2K_CONFIG_THROW
# define IF2K_CONFIG_POSIX 
# define IF2K_CONFIG_PTHREADS 
# define IF2K_CONFIG_FORK 
#else
# error please define one of IF2K_CONFIG_WIN32 IF2K_CONFIG_MACOSX IF2K_CONFIG_LINUX
#endif


// for win32 we need windows.h and friends
#if defined(IF2K_CONFIG_WIN32)
# include <windows.h>
# include <winsock.h>
# include <io.h>
# include <process.h>
#endif

#if defined(IF2K_CONFIG_PTHREADS)
# ifndef __STL_PTHREADS
#  define __STL_PTHREADS
# endif
# define _PTHREADS 1
# include <pthread.h>
#endif

#if defined(IF2K_CONFIG_POSIX)
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
# if defined(IF2K_CONFIG_NEW)
#  include <new>
# endif

# include <string>
# include <iostream>
# include <sstream>
# if defined(IF2K_CONFIG_NEWNOTHROW)
#  define newnothrow new (std::nothrow)
# else
#  define newnothrow new
# endif

# if defined(IF2K_CONFIG_THROW)
#  define if2k_throw( a ) throw( a )
# elif defined(IF2K_CONFIG_POSIX)
#  define if2k_throw( a ) do { fprintf(stderr,"%s",a); abort(); } while(0)
# elif defined(IF2K_CONFIG_WIN32)
#  define if2k_throw( a ) do { fprintf(stderr,"%s",a); abort(); } while(0)
# endif
#endif

#endif

