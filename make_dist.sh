#!/bin/bash
set -e
version=$(head -n 1 VERSION.txt)

dir="libpca-"$version
rm -rf $dir
mkdir $dir

shfiles='install.sh'
for f in $shfiles;do
	cp $f $dir
done

txtfiles='COPYING.txt README.txt CHANGES.txt VERSION.txt'
for f in $txtfiles;do
	cp $f $dir
done

folders='build examples include src test'
for f in $folders;do
	cp -r $f $dir
done

cleans='build test examples/simple'
for f in $cleans;do
	cd $dir/$f
	make clean > /dev/null
	cd - > /dev/null
done

tar cfz $dir.tar.gz $dir
mkdir -p dist
mv $dir.tar.gz dist

rm -rf $dir
