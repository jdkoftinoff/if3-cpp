#include "ifcmg_world.hpp"
#include "ifcmg_dynbuf.hpp"
#include "ifcmg_tree.hpp"
#include "ifcmg_compile.hpp"
#include "ifcmg_kernel.hpp"

using namespace ifcmg;

int main( int argc, char **argv )
{
  const char *cmg_home = getenv("CMG_HOME");
  if( !cmg_home )
    cmg_home = "/opt/cmg/";
  filename_t home = cmg_home;
  filename_t sharedir = home + "share/ifcmgdb-pre/";
  filename_t compiled_hostname_file = sharedir + "hostnames.pre";
  filename_t compiled_url_file = sharedir + "urls.pre";
  filename_t compiled_phrase_file = sharedir + "phrases.pre";

  const char *hostname = argv[1];
  size_t hostname_length = strlen(hostname);

  const char *link = argv[1];
  size_t link_length = strlen(link);

  bool enable_proof=true;

  kernel::kernel_t *k = new kernel::kernel_t(
                                             compiled_hostname_file,
                                             compiled_url_file,
                                             compiled_phrase_file
                                             );

  kernel::categories_enable_t categories_enable_bits;

  kernel::full_scan_request_t request;
  kernel::full_scan_results_t results;

  request.hostname( hostname, hostname_length );
  request.link( link, link_length );
  int category=-1;

  category = k->perform_scan(results,request,categories_enable_bits,enable_proof);

  std::stringstream ss;
  ss << results << "\n";

  std::cout << "category: " << category << "\n";
  std::cout << results;

  delete k;
  return 0;
}
