#!/bin/bash
# Copy to RPC Emu after build

RPCDIR=/home/alan/rpcemu/hostfs/Apps/Library/!TBX

copy_headers() {
  local unix_dir
  local riscos_dir
  local hdrs
  local dhdr

  unix_dir=$1
  riscos_dir=$2

  mkdir -p $riscos_dir/h
  hdrs=$unix_dir/*.h
  for hdr in $hdrs; do
     dhdr=$(basename $hdr | sed s#\.h\$##)
     cp -av $hdr $riscos_dir/h/$dhdr
  done
}

cp -f libtbx.a $RPCDIR

copy_headers tbx $RPCDIR/tbx

