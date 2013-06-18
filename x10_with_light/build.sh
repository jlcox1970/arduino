#!/bin/bash
cd `dirname $0`
rm -fr build-cli*
make clean
make
make  upload
rm -fr build-cli*

