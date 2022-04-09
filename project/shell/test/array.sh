#!/bin/bash

set -u
# set -x

function ArrayFunc()
{
  echo "-------$1"
  echo "+++++++$2"
  for id in $1; do
    echo "*** ${id}"
  done 

  # reg=$1
  # echo "reg= $reg "

  #reg: unbound variable,说明并不知道reg是数组,所有数组的属性均失效
  # echo "reg of id = ${!reg[*]}" 
  # echo "reg of len = ${#reg[*]}" 
  echo "999999999999999999999999"
}

function return_value()
{
  return $1
}
OUT_ID=(493 492 491)
OUT_VAL=(1 2 3)
# ArrayFunc ${OUT_ID} ${OUT_VAL}
# ArrayFunc ${OUT_ID[*]} ${OUT_VAL[*]}
ArrayFunc "${OUT_ID[*]}" "${OUT_VAL[*]}"

for(( id=0; id<${#OUT_ID[*]}; id++ )); do
  echo "id $id"
  echo "reg: ${OUT_ID[$id]}"
  echo "val: ${OUT_VAL[$id]}"
  # new=`return_value $id`
  return_value $id
done

# IN_ID_IND=(493 492 491)
# OUT_ID_IND=(494 495)
# OUT_VALUE=(1 1)
# #ID_IND=(244 245 246 247)
# GPIO_DIR=/sys/class/gpio

# for id in "${IN_ID_IND[@]}"; do 
# if [ ! -d ${GPIO_DIR}/gpio${id} ]; then
#     echo ${id} > /sys/class/gpio/export
# fi
# done

# for id in "${IN_ID_IND[@]}"; do 
# if [ -d ${GPIO_DIR}/gpio${id} ]; then
#     echo "echo 'in' > ${GPIO_DIR}/gpio${id}/direction" | sudo sh
#     echo -n "read ${id} : "
#     cat ${GPIO_DIR}/gpio${id}/value
# fi
# done

# for id in "${OUT_ID_IND[@]}"; do 
# if [ ! -d ${GPIO_DIR}/gpio${id} ]; then
#     echo ${id} > /sys/class/gpio/export
# fi
# done

# index=0
# for id in "${OUT_ID_IND[@]}"; do 
# if [ -d ${GPIO_DIR}/gpio${id} ]; then
#     echo "echo 'out' > ${GPIO_DIR}/gpio${id}/direction" | sudo sh
#     echo -n "set ${id} : "
#     echo echo" ${OUT_VALUE[index]} > ${GPIO_DIR}/gpio${id}/value" | sudo sh
# fi
# index=$((ndex+1))
# done

# OUT_ID_IND=(494 495)
# OUT_VALUE=(1 0)
# index=0
# for id in "${OUT_ID_IND[@]}"; do 
#   echo "$id : ${OUT_VALUE[index]}"
#   index=$((index+1))
#   echo "id: $index"
# done


array=(1 2 3 4)
array1[0]=5
array1[1]=5
array1[2]=5
array1[3]=5

# `hatter`是数组的0号位置，`duchess`是5号位置，`alice`是6号位置
# 没有赋值的数组元素的默认值是空字符串
names=(hatter [5]=duchess alice)  


# 定义数组的时候，可以使用通配符
mp3s=( *.mp3 )

# 先用`declare -a`命令声明一个数组，也是可以的。
declare -a ARRAYNAME


# `read -a`命令则是将用户的命令行输入，读入一个数组。
# read -a dice

# echo ${array[i]}     # i 是索引,大括号是必不可少
echo "array[0] = ${array[0]}"  
echo "array1[0] = ${array1[0]}"
echo "names[0] = ${names[0]}"
echo "mp3s[0] = ${mp3s[0]}"

# 读取所有成员
# `@`和`*`是数组的特殊索引，表示返回数组的所有成员。

foo=(aa bb cc dd ee ff)
echo "foo[@] = ${foo[@]}"
echo "foo[*] = ${foo[*]}"

for i in ${foo[@]}; do  # "${foo[@]}"必须放在双引号之中，遍历才返回正确的结果
  echo "foo : $i"
done

for i in "${foo[@]}"; do  # "${foo[@]}"必须放在双引号之中，遍历才返回正确的结果
  echo "foo : $i"
done

# 拷贝一个数组, 并添加成员

my=("${foo[@]}")
you=("${foo[@]}" gg)
echo "my[@] = ${my[@]}"
echo "you[*] = ${you[*]}"

echo "my len = ${#my[*]}"
echo "you len = ${#you[@]}"

# 提取数组序号
# `${!array[@]}`或`${!array[*]}`，可以返回数组的成员序号，即哪些位置是有值的。

arr=([5]=a [9]=b [23]=c)
echo "value = ${!arr[@]}"
#5 9 23
echo "value = ${!arr[*]}"
#5 9 23


# 提取数组成员,使用`+=`赋值运算符
food=( apples bananas cucumbers dates eggs fajitas grapes )
echo ${food[@]:1:1}
echo ${food[@]:1:3}

# 追加数组成员
foo=(a b c)
foo+=(A B C)
echo ${foo[@]}

# 删除数组
# 删除一个数组成员，使用`unset`命令。


