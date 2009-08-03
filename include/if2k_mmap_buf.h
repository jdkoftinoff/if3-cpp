/*
  
The Internet Filter IF2K Version 2
Source Code

Written By Jeff Koftinoff <jeffk@jdkoftinoff.com>
Copyright (c) 1995-2005
By Turner and Sons Productions Ltd.
http://www.internetfilter.com/

ALL RIGHTS RESERVED.

*/
#ifndef _IF2K_MMAP_BUF_H
#define _IF2K_MMAP_BUF_H

#include "if2k_world.h"
#include "if2k_string.h"
#include "if2k_buf.h"

namespace if2k
{
  class mmap_buf_t : public buf_t
  {
  private:
    mmap_buf_t(); // not allowed.
    mmap_buf_t( const mmap_buf_t &other ); // not allowed
    const mmap_buf_t & operator = ( const mmap_buf_t &other ); // not allowed
  public:
    mmap_buf_t( const filename_t &filename, bool writable=false, bool shared=false );
    
    virtual ~mmap_buf_t();
    
    bool is_valid() const { return mmap_pos!=0; }
    
  private:
    
#if defined(IF2K_CONFIG_POSIX)
    int file_handle;
    void *mmap_pos;
    int mmap_len;
#elif defined(IF2K_CONFIG_WIN32)
    HANDLE fileHandle;
    HANDLE mapFileHandle;
    void *mmap_pos;
    int mmap_len;
#else
#error unknown mmap method
#endif
  };  
}

#endif



