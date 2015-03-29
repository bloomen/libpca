#!/bin/bash
set -e
tarball=$1
if [ "x$tarball" = "x" ];then
    echo "No tarball provided"
    exit 1
fi

sftp bloomen,libpca@web.sourceforge.net << EOF
cd /home/frs/project/libpca
put $tarball
EOF
