#!/bin/bash

set -u
# set -x

source ./global.sh
source ./select.sh

#$$ 为当前 Shell 的进程 ID
#$0 当前 Shell 的名称
#$- 为当前 Shell 的启动参数
#$? 上一个命令的退出码，用来判断上一个命令是否执行成功
    # 返回值是 0，表示上一个命令执行成功；如果是非零，上一个命令执行失败。
 #${!#}可以直接显示脚本输入的最后一个变量
echo "\$$=$$; \$#=$#"
        # 传递给脚本的参数个数
        # 如果命令行后没有参数，那$#的值为0,
        # 此时${!#}将返回命令行中使用的脚本的名称。.
	
echo $* # 命令行各参数的值，将命令行参数提供的所有参数作为一个单词使用
echo $@ # 命令行各参数的值，将命令行参数提供的所有参数作为多个单词使用

a=z                     # 变量 a 赋值为字符串 z
b="a string"            # 变量值包含空格，就必须放在引号里面
c="a string and $b"     # 变量值可以引用其他变量的值
d="\t\ta string\n"      # 变量值可以使用转义字符
e=$(ls -l foo.txt)      # 变量值可以是命令的执行结果
f=$((5 * 7))            # 变量值可以是数学运算的结果
aa=${a}_bbb; bb=${a}_ccc



echo $a
echo "$a"
echo "${a}"
echo $b
echo $c
echo $d
echo $e
echo $f
echo $aa
echo $bb

myvar=USER  
echo ${!myvar} #$后的变量也是变量，加！用于展开变量求最终的值
declare -r WORK_DIR=current
declare -r my_dir=${WORK_DIR}_wm
echo "dir is ${my_dir}, l $LINENO"
  
echo ${!myvar} #$后的变量也是变量，加！用于展开变量求最终的值

kobe=24
number='kobe'
echo "kobe's num : $number"
#echo "kobe's num : ${${number}}" #bad substitution 不支持嵌套
echo "kobe's num : ${!number}"
eval echo '$'"${number}"

a="1 2  3"
echo $a    # 会合并空格
echo "$a"  #只有放在双引号里面读取，才能保持原来的格式
echo "\$a"

foo=''
echo "$foo"


declare -r NAME="wang ming"
echo $NAME
NAME=11 #NAME: readonly variable

# 用户创建的变量仅可用于当前 Shell，子 Shell 默认读取不到父 Shell 定义的变量。为了把变量传递给子 Shell，需要使用`export`命令


# 函数体内不仅可以声明全局变量，还可以修改全局变量
# 函数体内的变量，在函数体外可以读取
fun=world # 是否在此声明，没有关系，因为只要在函数内部使用，此处声明就会被覆盖
function variable()
{
    fun=hello
    local fun1=world  # outside of this, unbound variable "fun1"
    echo "fun= $fun, ${FUNCNAME[0]},${BASH_LINENO[0]}"
    echo "fun1= $fun1,${FUNCNAME[0]},${BASH_LINENO[0]}"
}
variable
echo "outside of func,fun= $fun,"

echo "${yn_input}"
UserSelect
echo "ret=$?"


unset foo
echo "$foo"  # unbound variable
