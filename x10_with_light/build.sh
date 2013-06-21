#!/bin/bash
cd `dirname $0`
rm -fr build-cli*
make clean
make
x=1
while [ $x -ne 0 ]
do
        make upload
        x=`echo $?`
        sleep 2
done

rm -fr build-cli*

