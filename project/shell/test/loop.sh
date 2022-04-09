#!/bin/bash

set -u
# set -x

while true; do
    echo "input y/Y to remove, n/N to exit"
    read select
    case  $select  in
        y | Y)
            echo "input is yes"
            ;;
            #exit;; # exit current scripts
        n | N)
            echo "input is no"
            ;;
        *)
            echo "unknown input and exit"
            ;;
    esac
    echo "func1: BASH_LINENO is ${BASH_LINENO[0]}"
    break
done

echo "*****$LINENO*****"

function UserInput(in)
{
    echo $LINENO
    echo "func2: BASH_LINENO is ${BASH_LINENO[0]}"
    echo "func2: FUNCNAME is ${FUNCNAME[0]}"
    echo "func2: BASH_SOURCE is ${BASH_SOURCE[1]}"
# while true; do
#     echo "input y/Y to remove, n/N to exit"
#     read select
#     case  $select  in
#         y | Y)
#             echo "input is yes"
#             ;;
#             #exit;; # exit current scripts
#         n | N)
#             echo "input is no"
#             ;;
#         *)
#             echo "unknown input and exit"
#             ;;
#     esac
#     echo "func1: BASH_LINENO is ${BASH_LINENO[0]}"
#     break
# done
}
func2
echo "**********"