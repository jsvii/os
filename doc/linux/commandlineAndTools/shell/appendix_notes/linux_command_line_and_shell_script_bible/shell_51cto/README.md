Shell
=====


# Commands

## type

查看命令类型

## enable

    enable -n alias
    enable -n cd
    enable cd


## 通配符

    *
    ?
    [1-9] [A-Z] [a-z]
    [1-9\-]

## set

set is a shell builtin

    set -o           # 显示所有的set

    set -o noclobber  #让‘不允许重定向’生效

    set -o noglob    # 关闭通配符功能生效

## shopt

shopt is a shell builtin

开启一些功能

    shopt -s extglob #开启扩展通配符功能

    ?(expr)  # 0-1
    *(expr)  # n
    +(expr)  # n > 1

## mktemp

创建临时文件命令

    mktemp
    mktemp -d  //directory



## $IFS

linux

    echo $IFS
     echo $IFS > test
     cat -A

shell

     OLDIFS=$IFS
     IFS=":"
     read line < /etc/passwd
     # this will not read

     read name aaa < /etc/passwd
     echo $name
     # root
