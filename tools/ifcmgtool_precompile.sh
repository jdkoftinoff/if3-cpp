#! /bin/bash

CMG_HOME=${CMG_HOME:-/opt/cmg}

"$CMG_HOME/bin/ifcmgtool_compile_all" "$CMG_HOME/share/ifcmgdb" "$CMG_HOME/share/ifcmgdb-pre"
 
