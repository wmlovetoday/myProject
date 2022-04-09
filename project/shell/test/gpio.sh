#!/bin/bash

set -u
# set -x

#ID_IND=(244 245 246 247)
GPIO_DIR=/sys/class/gpio

# operate array or variable
function ExportGpio()
{
  for id in $1; do 
  if [ ! -d ${GPIO_DIR}/gpio${id} ]; then
      echo ${id} > /sys/class/gpio/export
  fi
  done  
}

# operate array or variable
# $1 addr
# $2 in/out
function SetGpioDir()
{
  for id in $1; do 
  if [ -d ${GPIO_DIR}/gpio${id} ]; then
      echo "echo '$2' > ${GPIO_DIR}/gpio${id}/direction" | sudo sh
  fi
  done  
}
# operate variable
# $1 addr
function ReadGpio()
{
  declare local val
  VALUE=`cat ${GPIO_DIR}/gpio$1/value`
}

# operate variable
# $1 addr
# $2 value
function WriteGpio()
{
  echo "echo $2 > ${GPIO_DIR}/gpio$1/value" | sudo sh
}

spi1_gpio_in=(493 492 491)
spi1_gpio_out=(494 495)
spi1_gpio_out_val=(0 1)

ExportGpio "${spi1_gpio_in[*]}"
SetGpioDir "${spi1_gpio_in[*]}" in
ExportGpio "${spi1_gpio_out[*]}"
SetGpioDir "${spi1_gpio_out[*]}" out

for(( id=0; id<${#spi1_gpio_in[*]}; id++ )) do 
  ReadGpio ${spi1_gpio_in[$id]}
  echo "get ${spi1_gpio_in[$id]} : $VALUE"
done

for(( id=0; id<${#spi1_gpio_out[*]}; id++ )) do 
  WriteGpio ${spi1_gpio_out[$id]} ${spi1_gpio_out_val[$id]}
  echo "write ${spi1_gpio_out[$id]} : ${spi1_gpio_out_val[$id]}"
done

spi2_gpio_in=(244 245 246 247)

ExportGpio "${spi2_gpio_in[*]}"
SetGpioDir "${spi2_gpio_in[*]}" in

for(( id=0; id<${#spi2_gpio_in[*]}; id++ )) do 
  ReadGpio ${spi2_gpio_in[$id]}
  echo "get ${spi2_gpio_in[$id]} : $VALUE"
done

# gpio 40pin spi1 serve as gpio
# IN_ID_IND=(493 492 491)
# OUT_ID_IND=(494 495)
# OUT_VALUE=(0 1)
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
#     echo "set ${id} : ${OUT_VALUE[index]}"
#     echo "echo ${OUT_VALUE[index]} > ${GPIO_DIR}/gpio${id}/value" | sudo sh
# fi
# index=$((index+1))
# done
