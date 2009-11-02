#include <Python.h>
#include <unistd.h>
#include "ifcmg_world.hpp"
#include "ifcmg_tree.hpp"
#include "ifcmg_multiscanner.hpp"
#include "ifcmg_dynbuf.hpp"

static PyObject *ifcmgkernel_startup(PyObject *self, PyObject *args);
static PyObject *ifcmgkernel_run_scan(PyObject *self, PyObject *args);

static PyMethodDef IfcmgkernelMethods[] = {
  {"run_scan",  ifcmgkernel_run_scan, METH_VARARGS, "run scan on data"},
  {"startup",  ifcmgkernel_startup, METH_VARARGS, "start up the filter tables"},
  {NULL, NULL, 0, NULL}        /* Sentinel */
};

static ifcmg::multiscanner_t *multiscanner = 0;

PyMODINIT_FUNC
initifcmgkernel(void)
{
  (void) Py_InitModule("ifcmgkernel", IfcmgkernelMethods);
}

static PyObject *
ifcmgkernel_run_scan(PyObject *self, PyObject *args)
{ 
  using namespace ifcmg;

  const char *link;
  const char *text;
  const char *links;
  const char *content_links;

  int accessed = 0;
  int category = 0;
  int ad_tags_found = 0;

  if (!PyArg_ParseTuple(args, "ssss", &link, &text, &links, &content_links))
    return NULL;

  size_t text_len = strlen(text);

  multiscanner_result_t link_find_result = 
    multiscanner->find_in_data(
		     text,
		     text_len,
		     0xff, 
		     0xff, 
		     0xff,
		     0xff
		     );  
  
  multiscanner_result_t text_find_result = 
    multiscanner->find_in_data(
		     text,
		     text_len,
		     0xff, 
		     0xff, 
		     0xff,
		     0xff
		     );  
  
  multiscanner_result_t links_find_result = 
    multiscanner->find_in_data(
		     links,
		     strlen(links),
		     0xff, 
		     0xff, 
		     0xff,
		     0xff
		     );  
  
  multiscanner_result_t content_links_find_result = 
    multiscanner->find_in_data(
		     content_links,
		     strlen(content_links),
		     0xff, 
		     0xff, 
		     0xff,
		     0xff
		     );  

  if( text_len<100 )
    {
      accessed=0;
    }
  else
    {
      accessed=1;
      category=0;
      ad_tags_found=0;
      
      int total_bad = 0;
      total_bad += link_find_result.get_total_bad_url_match_count();
      total_bad += link_find_result.get_total_postbad_url_match_count();
      total_bad += link_find_result.get_total_bad_phrase_match_count();

      total_bad += text_find_result.get_total_bad_phrase_match_count();

      total_bad += content_links_find_result.get_total_bad_url_match_count();
      total_bad += content_links_find_result.get_total_postbad_url_match_count();
      total_bad += content_links_find_result.get_total_bad_phrase_match_count();
      
      if( total_bad>0 )
	{
	  category=1;
	  accessed=2;
	}
      
    }
  
  return Py_BuildValue("iii", accessed, category, ad_tags_found);
}

static PyObject *
ifcmgkernel_startup(PyObject *self, PyObject *args)
{
  using namespace ifcmg;

  const char *compiled_db_path;
  const char *uncompiled_db_path;
  if (!PyArg_ParseTuple(args, "ss", &compiled_db_path, &uncompiled_db_path ))
    return NULL;
  
  if ( multiscanner != 0 )
    delete multiscanner;
  
  multiscanner = new multiscanner_t( 
				    filename_t(compiled_db_path), 
				    filename_t(uncompiled_db_path),
				    0xff, 0xff, 0xff, 0xff 
				     );
    
  return Py_BuildValue("");
}

