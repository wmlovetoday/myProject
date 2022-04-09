#!/bin/bash

declare -r CPU_COUNT=`cat /proc/cpuinfo |grep "processor"|wc -l`
build_dir=`pwd`
PRO_NAME_=regex

rm -rf build
rm -rf ${PRO_NAME_}

mkdir build
cd build
cmake -DPRO_NAME=${PRO_NAME_} -DPRO_DIR=${build_dir} -DENABLE_ZLOG=ON ../../..
# cmake -DENABLE_ZLOG=ON -DCMAKE_BUILD_TYPE=ON -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DSRC_EXAM=${build_dir} ../../..
make -j${CPU_COUNT}
make install
cd -
# mv build/${PRO_NAME_} ./
rm build -rf
# ulimit -c unlimited