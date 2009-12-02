#include <Python.h>
#include <unistd.h>
#include "ifcmg_world.hpp"
#include "ifcmg_dynbuf.hpp"
#include "ifcmg_tree.hpp"
#include "ifcmg_compile.hpp"
#include "ifcmg_kernel.hpp"


static PyObject *ifcmgkernel_startup(PyObject *self, PyObject *args);
static PyObject *ifcmgkernel_shutdown(PyObject *self, PyObject *args);
static PyObject *ifcmgkernel_compile_url_files(PyObject *self, PyObject *args);
static PyObject *ifcmgkernel_compile_phrase_files(PyObject *self, PyObject *args);
static PyObject *ifcmgkernel_scan_data(PyObject *self, PyObject *args);
static PyObject *ifcmgkernel_scan_url(PyObject *self, PyObject *args);

static PyMethodDef IfcmgkernelMethods[] = {
  {"startup", ifcmgkernel_startup, METH_VARARGS, "start up the filter tables"},
  {"shutdown", ifcmgkernel_shutdown, METH_VARARGS, "shutdown the filter"},
  {"scan_data", ifcmgkernel_scan_data, METH_VARARGS, "run scan on data"},
  {"scan_url", ifcmgkernel_scan_url, METH_VARARGS, "run scan on url"},
  {"compile_url_files", ifcmgkernel_compile_url_files, METH_VARARGS, "compile set of filter files that contain URLs"},
  {"compile_phrase_files", ifcmgkernel_compile_phrase_files, METH_VARARGS, "compile set of filter files that contain phrases"},
  {NULL, NULL, 0, NULL} /* Sentinel */
};

static ifcmg::kernel::kernel_t *kernel=0;


PyMODINIT_FUNC
initifcmgkernel(void)
{
  (void) Py_InitModule("ifcmgkernel", IfcmgkernelMethods);
}


static PyObject *
ifcmgkernel_startup(PyObject *self, PyObject *args)
{
  const char *compiled_hostname_filename;
  const char *compiled_url_filename;
  const char *compiled_alphanumeric_filename;

  if (!PyArg_ParseTuple(args, "sss",
                        &compiled_hostname_filename,
                        &compiled_url_filename,
                        &compiled_alphanumeric_filename ) )
  {
    return NULL;
  }

  if (kernel != 0)
    delete kernel;

  kernel = new ifcmg::kernel::kernel_t(
                                    ifcmg::filename_t(compiled_hostname_filename),
                                    ifcmg::filename_t(compiled_url_filename),
                                    ifcmg::filename_t(compiled_alphanumeric_filename)
                                    );

  return Py_BuildValue("");
}

static PyObject *
ifcmgkernel_shutdown(PyObject *self, PyObject *args)
{
  if (kernel != 0)
    delete kernel;

  return Py_BuildValue("");
}

static PyObject *
ifcmgkernel_compile_url_files(PyObject *self, PyObject *args)
{
  const char *source_dir;
  const char *dest_file;
  const char *middle_part;
  int first_category;
  int last_category;

  if (!PyArg_ParseTuple(args, "sssii",
                        &source_dir,
                        &dest_file,
                        &middle_part,
                        &first_category,
                        &last_category) )
  {
    return NULL;
  }

  int count = ifcmg::compile_url_files_into_one(
                                                ifcmg::filename_t(source_dir),
                                                ifcmg::filename_t(dest_file),
                                                middle_part,
                                                first_category,
                                                last_category
                                                );

  return Py_BuildValue("i",count);
}

static PyObject *
ifcmgkernel_compile_phrase_files(PyObject *self, PyObject *args)
{
  const char *source_dir;
  const char *dest_file;
  const char *middle_part;
  int first_category;
  int last_category;

  if (!PyArg_ParseTuple(args, "sssii",
                        &source_dir,
                        &dest_file,
                        &middle_part,
                        &first_category,
                        &last_category) )
  {
    return NULL;
  }

  int count = ifcmg::compile_phrase_files_into_one(
                                                   ifcmg::filename_t(source_dir),
                                                   ifcmg::filename_t(dest_file),
                                                   middle_part,
                                                   first_category,
                                                   last_category
                                                   );

  return Py_BuildValue("i",count);
}


static PyObject *
ifcmgkernel_scan_data(PyObject *self, PyObject *args)
{
  int enable_proof_int;
  bool enable_proof;

  const char *hostname;
  size_t hostname_length;

  const char *link;
  size_t link_length;

  const char *text;
  size_t text_length;

  const char *links;
  size_t links_length;

  const char *content_links;
  size_t content_links_length;

  const char *categories_enable = 0;

  int accessed = 0;
  int category = 0;

  if (!PyArg_ParseTuple(args, "is#s#s#s#s#|s",
                        &enable_proof_int,
                        &hostname, &hostname_length,
                        &link, &link_length, &text, &text_length,
                        &links, &links_length,
                        &content_links, &content_links_length,
                        categories_enable
                        ))
    return NULL;

  enable_proof = (enable_proof_int!=0);

  ifcmg::kernel::categories_enable_t categories_enable_bits;

  if (categories_enable)
  {
    categories_enable_bits.set_from_string(std::string(categories_enable));
  }

  ifcmg::kernel::full_scan_request_t request;
  ifcmg::kernel::full_scan_results_t results;

  request.hostname( hostname, hostname_length );
  request.link( link, link_length );
  request.text( text, text_length );
  request.links( links, links_length );
  request.content_links( content_links, content_links_length );

  category = kernel->perform_scan(results,request,categories_enable_bits,enable_proof);

  if( text_length + links_length + content_links_length > 1000 )
  {
    accessed=1;
  }

  std::stringstream ss;
  ss << results << "\n";

  return Py_BuildValue("iis#", accessed, category+1, ss.str().c_str(), ss.str().length() );
}


static PyObject *
ifcmgkernel_scan_url(PyObject *self, PyObject *args)
{
  int enable_proof_int;
  bool enable_proof;

  const char *hostname;
  size_t hostname_length;

  const char *link;
  size_t link_length;

  const char *categories_enable = 0;

  int category = 0;

  if (!PyArg_ParseTuple(args, "is#s#|s",
                        &enable_proof_int,
                        &hostname, &hostname_length,
                        &link, &link_length,
                        categories_enable
                        ))
    return NULL;

  enable_proof = (enable_proof_int !=0);

  ifcmg::kernel::categories_enable_t categories_enable_bits;

  if (categories_enable)
  {
    categories_enable_bits.set_from_string(std::string(categories_enable));
  }

  ifcmg::kernel::full_scan_request_t request;
  ifcmg::kernel::full_scan_results_t results;

  request.hostname( hostname, hostname_length );
  request.link( link, link_length );

  category = kernel->perform_scan(results,request,categories_enable_bits,enable_proof);

  std::stringstream ss;
  ss << results << "\n";

  return Py_BuildValue("is#", category+1, ss.str().c_str(), ss.str().length() );
}

