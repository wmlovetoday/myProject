#!/bin/bash

set -u
# set -x

source ./global.sh

RGMII_DELAY=0xc000
RGMII_NO_DELAY=0x0000
RGMII_1000M=0x0010
RGMII_100M=0x0001
RGMII_10M=0x0000

function condition()
{
    value=0x0
    if [ $1 -eq 1000 ] && [ $2 == "delay" ]
    then
        value=$(($RGMII_1000M + $RGMII_DELAY))
        echo -n "1000 dalay : 0x"
        printf %x $value
    elif [ $1 -eq 100 ] && [ $2 == "delay" ]
    then
        echo "100 dalay"
    elif [ $1 -eq 10 ] && [ $2 == "delay" ]
    then
        echo "10 dalay"

    elif [ $1 -eq 1000 ] && [ $2 == "nodelay" ]
    then
        echo "1000 no dalay"
    elif [ $1 -eq 100 ] && [ $2 == "nodelay" ]
    then
        echo "100 no dalay"
    elif [ $1 -eq 10 ] && [ $2 == "nodelay" ]
    then
        echo "10 no dalay"
    else
        echo "ERROR: illegal input"
    fi
}

condition 1000 delay