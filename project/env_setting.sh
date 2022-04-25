# if undef reg will report error
set -u
# for cmd debug 
set -x  

declare -r CPU_COUNT_=`cat /proc/cpuinfo |grep "processor"|wc -l`
declare -r ubuntu_ver_=`lsb_release -r --short`
declare -r platform_=`arch`

# echo ${cp} 
# if set -u , error