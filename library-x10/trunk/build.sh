#!/bin/bash
# This file will install the lib under the correct path on a Fedora computer   
cd `dirname $0`
mkdir -p /usr/share/arduino/libraries/x10
cp -frv * /usr/share/arduino/libraries/x10
