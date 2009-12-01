/*
  
The IF Contextual Media Group Kernel Version 3
Source Code

Written By Jeff Koftinoff <jeffk@contextualmediagroup.com>
Copyright (c) 1995-2005
By Contextual Media Group, Inc.
http://www.contextualmediagroup.com/

ALL RIGHTS RESERVED.

 */
#ifndef IFCMG_COMPILE_HPP
#define IFCMG_COMPILE_HPP

#include "ifcmg_world.hpp"
#include "ifcmg_string.hpp"
#include "ifcmg_util.hpp"
#include "ifcmg_tree.hpp"

namespace ifcmg
{

  inline bool
  compile_url_file(
                   const filename_t &input_filename,
                   const filename_t &output_filename,
                   volatile int *progress = 0,
                   int code_offset = 0
                   )
  {
    string_t prefix;

    bool r = tree_compile<tree_traits_url_t, pattern_expander_standard_t > (
                                                                            input_filename,
                                                                            output_filename,
                                                                            code_offset,
                                                                            prefix,
                                                                            progress
                                                                            );

    return r;
  }

  inline bool
  compile_phrase_file(
                      const filename_t &input_filename,
                      const filename_t &output_filename,
                      volatile int *progress = 0,
                      int code_offset = 0
                      )
  {
    string_t prefix;

    bool r = tree_compile<tree_traits_alphanumeric_t, pattern_expander_standard_t > (
                                                                                     input_filename,
                                                                                     output_filename,
                                                                                     code_offset,
                                                                                     prefix,
                                                                                     progress
                                                                                     );

    return r;
  }

  template <class TRAITS_T, class PATTERN_EXPANDER_T>
  inline int
  compile_files(
                const filename_t &source_dir_,
                const filename_t &dest_dir_,
                const char *middle_part,
                int first_category = 0,
                int last_category = 64
                )
  {
    int success_count = 0;
    filename_t source_dir(source_dir_);
    filename_t dest_dir(dest_dir_);

    util::fix_directory_name(source_dir);
    util::fix_directory_name(dest_dir);
    mkdir(dest_dir.c_str(), 0777);
    // compile good urls
    for (int category = first_category; category < last_category; ++category)
    {
      filename_t input_filename;
      form(input_filename, "%s%03d-%s.txt", source_dir.c_str(), category + 1, middle_part);

      filename_t output_filename;
      form(output_filename, "%s%03d-%s.pre", dest_dir.c_str(), category + 1, middle_part);

      if (tree_compile<TRAITS_T, PATTERN_EXPANDER_T > (
                                                       input_filename,
                                                       output_filename,
                                                       category * 10,
                                                       ""
                                                       ))
      {
        success_count++;
      }
    }

    return success_count;
  }

  template <class TRAITS_T, class PATTERN_EXPANDER_T>
  inline int
  compile_files_into_one(
                         const filename_t &source_dir_,
                         const filename_t &dest_file_,
                         const char *middle_part,
                         int first_category = 0,
                         int last_category = 64
                         )
  {
    int success_count = 0;
    filename_t source_dir(source_dir_);
    filename_t dest_file(dest_file_);

    util::fix_directory_name(source_dir);

    tree_t<TRAITS_T> tree;
    PATTERN_EXPANDER_T expander;

    dynbuf_t input_buf;

    for (int category = first_category; category < last_category; ++category)
    {
      filename_t input_filename;
      form(input_filename, "%s%03d-%s.txt", source_dir.c_str(), category, middle_part);

      input_buf.clear();
      if (
          input_buf.append_from_file(input_filename)
          && expander.load(input_buf, tree, category, "", 0)
          )
      {
        ++success_count;
      }
    }
    tree.make_shortcuts();

    dynbuf_t compiled_data;
    if (tree.save(compiled_data))
    {
      if ( !compiled_data.extract_to_file(dest_file))
      {
        success_count = 0;
      }
    }

    return success_count;
  }


}

#endif
