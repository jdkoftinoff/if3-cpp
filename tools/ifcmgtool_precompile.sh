#! /bin/bash

CMG_HOME=${CMG_HOME:-/opt/cmg}

"$CMG_HOME/bin/if3tool_compile_all" "$CMG_HOME/share/if3db" "$CMG_HOME/share/if3db-pre"
 
