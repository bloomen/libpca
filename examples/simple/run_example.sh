#!/bin/bash
set -e
export LD_LIBRARY_PATH=../../build:$LD_LIBRARY_PATH
./pca_example
