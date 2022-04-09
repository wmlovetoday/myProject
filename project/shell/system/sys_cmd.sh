#!/bin/bash

set -u
# set -x

declare -r WORK_DIR=`pwd`

declare -r SCRIPTS_LOC=`cd "\`dirname "$0"\`"; pwd`
declare -r CPU_COUNT=`cat /proc/cpuinfo |grep "processor"|wc -l`
echo "WORK_DIR : ${WORK_DIR}"
echo "SCRIPTS_LOC : ${SCRIPTS_LOC}"
echo "CPU_COUNT : ${CPU_COUNT}"
