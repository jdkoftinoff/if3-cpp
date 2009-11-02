#include <Python.h>
#include <unistd.h>
#include "ifcmg_world.hpp"
#include "ifcmg_tree.hpp"
#include "ifcmg_multiscanner.hpp"
#include "ifcmg_dynbuf.hpp"

static PyObject *ifcmgkernel_startup(PyObject *self, PyObject *args);
static PyObject *
ifcmgkernel_run_scan(PyObject *self, PyObject *args);

static PyMethodDef IfcmgkernelMethods[] = {
  {"run_scan", ifcmgkernel_run_scan, METH_VARARGS, "run scan on data"},
  {"startup", ifcmgkernel_startup, METH_VARARGS, "start up the filter tables"},
  {NULL, NULL, 0, NULL} /* Sentinel */
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

  const char *bad_categories_enable = 0;

  int accessed = 0;
  int category = 0;
  int ad_tags_found = 0;

  if (!PyArg_ParseTuple(args, "s#s#s#s#|s",
                        &link, &link_length, &text, &text_length,
                        &links, &links_length,
                        &content_links, &content_links_length,
                        bad_categories_enable
                        ))
    return NULL;

  multiscanner_categories_enable_t good_url_enable_bits;
  multiscanner_categories_enable_t bad_url_enable_bits;
  multiscanner_categories_enable_t postbad_url_enable_bits;
  multiscanner_categories_enable_t bad_phrase_enable_bits;

  if (bad_categories_enable)
  {
    bad_url_enable_bits.set_from_string(std::string(bad_categories_enable));
    postbad_url_enable_bits.set_from_string(std::string(bad_categories_enable));
    bad_phrase_enable_bits.set_from_string(std::string(bad_categories_enable));
  }

  multiscanner_result_t link_find_result =
          multiscanner->find_in_data(
                                     link,
                                     link_length,
                                     good_url_enable_bits,
                                     bad_url_enable_bits,
                                     postbad_url_enable_bits,
                                     bad_phrase_enable_bits
                                     );

  multiscanner_result_t text_find_result =
          multiscanner->find_in_data(
                                     text,
                                     text_length,
                                     good_url_enable_bits,
                                     bad_url_enable_bits,
                                     postbad_url_enable_bits,
                                     bad_phrase_enable_bits
                                     );

  multiscanner_result_t links_find_result =
          multiscanner->find_in_data(
                                     links,
                                     links_length,
                                     good_url_enable_bits,
                                     bad_url_enable_bits,
                                     postbad_url_enable_bits,
                                     bad_phrase_enable_bits
                                     );

  multiscanner_result_t content_links_find_result =
          multiscanner->find_in_data(
                                     content_links,
                                     content_links_length,
                                     good_url_enable_bits,
                                     bad_url_enable_bits,
                                     postbad_url_enable_bits,
                                     bad_phrase_enable_bits
                                     );

  if (text_length < 100)
  {
    accessed = 0;
  }
  else
  {
    accessed = 1;
    category = 0;
    ad_tags_found = 0;

    int total_bad = 0;
    total_bad += link_find_result.get_total_bad_url_match_count();
    total_bad += link_find_result.get_total_postbad_url_match_count();
    total_bad += link_find_result.get_total_bad_phrase_match_count();

    total_bad += text_find_result.get_total_bad_phrase_match_count();

    total_bad += content_links_find_result.get_total_bad_url_match_count();
    total_bad += content_links_find_result.get_total_postbad_url_match_count();
    total_bad += content_links_find_result.get_total_bad_phrase_match_count();

    // find largest category

    int category_sums[64];

    for (int i = 0; i < 64; ++i)
    {
      category_sums[i] = 0;
      category_sums[i] += link_find_result.get_bad_phrase_match_count(i);
      category_sums[i] += link_find_result.get_bad_url_match_count(i);
      category_sums[i] += text_find_result.get_bad_phrase_match_count(i);
      category_sums[i] += text_find_result.get_bad_url_match_count(i);
      category_sums[i] += content_links_find_result.get_bad_phrase_match_count(i);
      category_sums[i] += content_links_find_result.get_bad_url_match_count(i);
    }

    int largest_category = -1;
    int largest_category_value = 0;

    for (int i = 0; i < 64; ++i)
    {
      if (category_sums[i] >= largest_category_value && category_sums[i]>0)
      {
        largest_category = i;
        largest_category_value = category_sums[i];
      }
    }

    if (largest_category != -1)
    {
      category = largest_category;

      if (total_bad > 0)
      {
        accessed = 2;
      }

    }

  }

  return Py_BuildValue("iii", accessed, category+1, ad_tags_found);
}

static PyObject *
ifcmgkernel_startup(PyObject *self, PyObject *args)
{
  using namespace ifcmg;

  const char *compiled_db_path;
  const char *uncompiled_db_path;
  if (!PyArg_ParseTuple(args, "ss", &compiled_db_path, &uncompiled_db_path))
    return NULL;

  if (multiscanner != 0)
    delete multiscanner;

  multiscanner = new multiscanner_t(
                                    filename_t(compiled_db_path),
                                    filename_t(uncompiled_db_path)
                                    );

  return Py_BuildValue("");
}

