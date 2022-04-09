#!/bin/bash

set -u

source ./global.sh
source ./select.sh

RET_VAL=0

export RET_VAL

ReturnTest
echo "ret= $?; RET_VAL=$RET_VAL "