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
  size_t link_length;

  const char *text;
  size_t text_length;

  const char *links;
  size_t links_length;

  const char *content_links;
  size_t content_links_length;

  int accessed = 0;
  int category = 0;
  int ad_tags_found = 0;

  if (!PyArg_ParseTuple(args, "s#s#s#s#", 
			&link, &link_length, &text, &text_length, 
			&links, &links_length, 
			&content_links, &content_links_length ))
    return NULL;
  
  multiscanner_result_t link_find_result = 
    multiscanner->find_in_data(
		     link,
		     link_length,
		     0xff, 
		     0xff, 
		     0xff,
		     0xff
		     );  
  
  multiscanner_result_t text_find_result = 
    multiscanner->find_in_data(
		     text,
		     text_length,
		     0xff, 
		     0xff, 
		     0xff,
		     0xff
		     );  
  
  multiscanner_result_t links_find_result = 
    multiscanner->find_in_data(
		     links,
		     links_length,
		     0xff, 
		     0xff, 
		     0xff,
		     0xff
		     );  
  
  multiscanner_result_t content_links_find_result = 
    multiscanner->find_in_data(
		     content_links,
		     content_links_length,
		     0xff, 
		     0xff, 
		     0xff,
		     0xff
		     );  

  if( text_length<100 )
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

