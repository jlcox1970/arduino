#!/bin/bash
# $Rev: 40 $
# $Author: jason $
# $Date: 2012-06-02 15:33:04 +1000 (Sat, 02 Jun 2012) $

cd `dirname $0`
rm -fr build-cli*
make clean
make
make  upload
rm -fr build-cli*

