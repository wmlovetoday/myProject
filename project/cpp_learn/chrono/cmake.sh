#!/bin/bash

build_dir=`pwd`
PRO_NAME_=chrono

rm -rf build
# rm -rf ${PRO_NAME_}

mkdir build
cd build
cmake -DPRO_NAME=${PRO_NAME_} -DPRO_DIR=${build_dir} -DENABLE_ZLOG=ON ../../..
# cmake -DENABLE_ZLOG=ON -DCMAKE_BUILD_TYPE=ON -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DSRC_EXAM=${build_dir} ../../..
make
make install
cd -
# mv build/${PRO_NAME_} ./
rm build -rf
# ulimit -c unlimited