#!/bin/bash

set -u

source ./global.sh
source ./select.sh

# -t 用于指定超时
# -a 读取数组，从零号位置开始。
# -p 提示
# -n 参数指定只读取若干个字符作为变量值，而不是整行读取。
# -e 参数允许用户输入的时候，使用`readline`库提供的快捷键,如　tab
# -s 使得用户的输入不显示在屏幕上，这常常用于输入密码或保密信息
# -u fd 使用文件描述符`fd`作为输入。


read -t 1 ack
ack=${ack:-5} # `-`` 不可省略，如果不想写 `-`,使用下面的语法
# ${ack:5}
echo "ack= $ack"

# 这个是环境变量，一旦设置，对所有read都生效，且不用写 -t 选项．
#TMOUT=3  
TIMOUT=3
echo -n "输入一些文本 in $TIMOUT seconds > "
if read -t $TIMOUT response; then
  echo "用户已经输入了 : $response"
else
  echo "用户没有输入"
fi

# 如果用户的输入项少于`read`命令给出的变量数目，那么额外的变量值为空。
# 如果用户的输入项多于定义的变量，那么多余的输入项会包含到最后一个变量中。
# echo Please, enter your firstname and lastname
read -p "echo Please, enter your firstname and lastname" FN LN
echo "Hi! $LN, $FN !"

# 如果`read`命令之后没有定义变量名，那么环境变量`REPLY`会包含所有的输入
echo -n "Enter one or more values > "
read
echo "REPLY = '$REPLY'"

# 读取一个文件的内容。`done`命令后面的定向符`<`，将文件内容导向`read`命令，每次读取一行，存入变量`myline`，直到文件读取完毕。
filename='./test.sh'
while read myline
do
  echo "$myline"
done < $filename


# IFS 变量,自定义读取间隔符，默认是空格

FILE=/etc/passwd

read -p "Enter a username > " user_name
file_info="$(grep "^$user_name:" $FILE)"

if [ -n "$file_info" ]; then
  IFS=":" read user pw uid gid name home shell <<< "$file_info"
  echo "User = '$user'"
  echo "UID = '$uid'"
  echo "GID = '$gid'"
  echo "Full Name = '$name'"
  echo "Home Dir. = '$home'"
  echo "Shell = '$shell'"
else
  echo "No such user '$user_name'" >&2
  exit 1
fi


