#!/bin/bash

set -u
# set -x

function UserSelect()
{
local select=''
local ret=0  # yes:1, no:2, other:0
while true; do
    read select
    case  $select  in
        y | Y)
            echo "input is yes"
            ret=1
            return $ret
            ;;
            #exit;; # exit current scripts
        n | N)
            echo "input is no"
            ret=2
            return $ret
            ;;
        *)
            echo "unknown input, must input y/Y/n/N"
            ret=0
            ;;
    esac
    # break
done
}