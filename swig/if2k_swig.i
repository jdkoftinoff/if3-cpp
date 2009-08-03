%module if2kkernel
%{
#include "if2k_string.h"
#include "if2k_buf.h"
#include "if2k_mmap_buf.h"
#include "if2k_dynbuf.h"
#include "if2k_compile.h"
#include "if2k_pattern_expander.h"
#include "if2k_multiscanner.h"
%}

%include "std_string.i"
%include "std_vector.i"

%include "if2k_string.h"
%include "if2k_buf.h"
%include "if2k_mmap_buf.h"
%include "if2k_dynbuf.h"
%include "if2k_compile.h"
%include "if2k_pattern_expander.h"
%include "if2k_multiscanner.h"

