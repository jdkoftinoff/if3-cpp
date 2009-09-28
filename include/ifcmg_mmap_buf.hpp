/*
  
The IF Contextual Media Group Kernel Version 3
Source Code

Written By Jeff Koftinoff <jeffk@contextualmediagroup.com>
Copyright (c) 1995-2005
By Contextual Media Group, Inc.
http://www.contextualmediagroup.com/

ALL RIGHTS RESERVED.

*/
#ifndef IFCMG_MMAP_BUF_H
#define IFCMG_MMAP_BUF_H

#include "ifcmg_world.hpp"
#include "ifcmg_string.hpp"
#include "ifcmg_buf.hpp"

namespace ifcmg
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
    
#if defined(IFCMG_CONFIG_POSIX)
    int file_handle;
    void *mmap_pos;
    int mmap_len;
#elif defined(IFCMG_CONFIG_WIN32)
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



