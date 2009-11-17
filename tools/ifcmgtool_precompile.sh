#! /bin/bash

CMG_PREFIX=${CMG_PREFIX:-/usr}

$CMG_PREFIX/bin/ifcmgtool_compile_all $CMG_PREFIX/share/ifcmgdb $CMG_PREFIX/share/ifcmgdb-pre
 
