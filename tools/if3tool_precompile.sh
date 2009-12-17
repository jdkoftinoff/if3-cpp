#! /bin/bash

IF3_HOME=${IF3_HOME:-/opt/tasi}

"$IF3_HOME/bin/if3tool_compile_all" "$IF3_HOME/share/if3db" "$IF3_HOME/share/if3db-pre"
 
