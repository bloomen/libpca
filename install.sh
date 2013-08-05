#!/bin/bash
set -e
version=$(head -n 1 VERSION.txt)

dir=$1

if [ "x$dir" = "x" ]; then
    dir=/usr/local
fi

echo "Compiling ... "

cd build
make clean >/dev/null
rm -f libpca.so*
make
mv libpca.so libpca.so.$version
ln -s libpca.so.$version libpca.so
cd ..

echo
echo "Installing ..."

inc_dir=$dir/include
lib_dir=$dir/lib

mkdir -p $dir
mkdir -p $inc_dir
mkdir -p $lib_dir

cp -v include/pca.h $inc_dir
cp -v build/libpca.so.$version $lib_dir
rm -f $lib_dir/libpca.so
ln -sv $lib_dir/libpca.so.$version $lib_dir/libpca.so

echo
echo "libpca successfully installed!"
