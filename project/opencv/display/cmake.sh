#!/bin/bash

WORK_DIR=`cd "\`dirname "$0"\`"; pwd`
CMAKE_DIR=${WORK_DIR}/../..

echo "WORK_DIR = ${WORK_DIR} but cur_dir = `pwd`"
source ${CMAKE_DIR}/env_setting.sh
PRO_NAME_=display

if [ $# -ge 1 ] && [ $1 == "clean" ]
then
    rm -rf build
    rm -rf ${PRO_NAME_}
    exit 0
fi

rm -rf ${WORK_DIR}/build
rm -rf ${WORK_DIR}/${PRO_NAME_}
mkdir ${WORK_DIR}/build
cd ${WORK_DIR}/build

com_build=-DPRO_NAME=${PRO_NAME_}\ -DPRO_DIR=${WORK_DIR}\ -DPLATFORM=${platform_}\ -DVERSION=${ubuntu_ver_}
opt_build=-DENABLE_ZLOG=ON\ -DENABLE_ZMQ_DDS=OFF\ -DENABLE_OPENCV=ON\ -DENABLE_SYS=0FF
cmake -DENABLE_RELEASE=OFF ${com_build} ${opt_build}  ${CMAKE_DIR}

make -j ${CPU_COUNT_}
make install
cd ${WORK_DIR}
rm -rf build

# ulimit -c unlimited