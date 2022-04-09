#!/bin/bash

set -u
# set -x

declare -r yn_input="please input y or n"

function ReturnTest()
{
    RET_VAL=10
    local val=100
    return $val
}
