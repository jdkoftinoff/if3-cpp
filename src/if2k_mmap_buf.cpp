/*
  
The Internet Filter IF2K Version 2
Source Code

Written By Jeff Koftinoff <jeffk@jdkoftinoff.com>
Copyright (c) 1995-2005
By Turner and Sons Productions Ltd.
http://www.internetfilter.com/

ALL RIGHTS RESERVED.

*/
#include "if2k_world.h"
#include "if2k_mmap_buf.h"
#include "if2k_util.h"

#if defined(IF2K_CONFIG_WIN32)

namespace if2k
{
  mmap_buf_t::mmap_buf_t( const filename_t &filename, bool writable, bool shared )
  {
    static unsigned int unique_mapping_id=0;
    static const char * unique_mapping_prefix = "if2kmmap";
    
    unique_mapping_id++;
    string_t unique_mapping_name;
    form( unique_mapping_name, "%s%3d", unique_mapping_prefix, unique_mapping_id );
    
    data = (unsigned char *)0;
    data_len = 0;
    buf_len = 0;
    mmap_pos = 0;
    fileHandle = INVALID_HANDLE_VALUE;
    mapFileHandle = INVALID_HANDLE_VALUE;
    
    fileHandle = CreateFileA( 
      filename.c_str(), 
      writable ? (GENERIC_READ | GENERIC_WRITE) : (GENERIC_READ),
      shared ? (FILE_SHARE_READ | FILE_SHARE_WRITE) : 0,
      0,
      OPEN_EXISTING,
      0,
      0 
      );
    
    
    if( fileHandle!=INVALID_HANDLE_VALUE )
    {
      mapFileHandle = CreateFileMappingA( 
        fileHandle, 
        0, 
        writable ? PAGE_READWRITE : PAGE_READONLY, 
        0, 
        0, 
        unique_mapping_name.c_str() 
        );
      
      if( mapFileHandle !=0 )
      {
        mmap_pos = MapViewOfFile( 
          mapFileHandle, 
          writable ? (FILE_MAP_READ | FILE_MAP_WRITE) : FILE_MAP_READ, 
          0,
          0,
          0
          );
        mmap_len = GetFileSize( fileHandle, 0 );
      }
      else
      {
        DWORD e = GetLastError();
        mmap_pos=0;
        mmap_len=0;
      }
    }
    
    if( mmap_pos!=0 )
    {
      data = (unsigned char *)mmap_pos;
      data_len = mmap_len;
      buf_len = mmap_len;
    }
    else
    {
      data = (unsigned char*)calloc(4096,1); // make dummy buffer as last resort
      data_len = 4096;
      buf_len = 0;
    }
  }
  
  mmap_buf_t::~mmap_buf_t()
  {
    if( mmap_pos!=0 )
    {
      UnmapViewOfFile(mmap_pos);
    }
    if( mapFileHandle!=INVALID_HANDLE_VALUE )
    {
      CloseHandle(mapFileHandle);
    }
    if( fileHandle!=INVALID_HANDLE_VALUE )
    {
      CloseHandle(fileHandle);
    }
    
    if( data!=0 && mmap_pos==0 )
    {
      free(data);
    }
  }
}
#elif defined(IF2K_CONFIG_POSIX)
namespace if2k
{
  mmap_buf_t::mmap_buf_t( const filename_t &filename, bool writable, bool shared ) 
    : 
    file_handle(-1),
    mmap_pos(0), 
    mmap_len(0)
  {
  	
    data = (unsigned char *)0;
    data_len = 0;
    buf_len = 0;
    mmap_pos = 0;
    
    file_handle = open( filename.c_str(), ((writable && shared) ? O_RDWR : O_RDONLY) );
    if( file_handle>=0 )
    {
      struct stat sbuf;
      
      if( stat(filename.c_str(),&sbuf) == 0 )
      {      
        mmap_len = sbuf.st_size;
        mmap_pos = mmap( 
          0, 
          mmap_len, 
          PROT_READ | (writable ? PROT_WRITE : 0), 
          (shared ? MAP_SHARED : MAP_PRIVATE), 
          file_handle, 
          0 
          );
        
        if( mmap_pos != (caddr_t)(-1) && mmap_pos!=0 )
        {
          data = (unsigned char *)mmap_pos;
          data_len = mmap_len;
          buf_len = mmap_len;
        }
        else
        {
          close( file_handle );
        }
      }
      else
      {
        close(file_handle);
      }
    }
  }
  
  mmap_buf_t::~mmap_buf_t()
  {
    if( mmap_pos!=0 && mmap_pos!=(caddr_t)(-1) )
    {
      munmap( mmap_pos, mmap_len );
      close( file_handle );
    }
    else if( data )
    {
      free(data);
    }
  }
}
#else
#error unknown platform for mmap_buf_t
#endif
