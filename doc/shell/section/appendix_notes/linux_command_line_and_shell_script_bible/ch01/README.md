#Chapter 01

The only way to really learn scripting is to write scripts.

## sheban起始

    #!/bin/bash

## 执行

    bash script.sh

## 改变node属性

    chmod a+x script.sh
    ./script.sh

## bashrc文件

启动`shell`时，它一开始会执行一系列的命令来定义诸如提示文本，颜色等各类设置，这组命令来自于用户主目录下的脚本文件 `~/.bashrc`

同时`Bash`还维护着`~/.bash_history`这一个历史记录文件。

    printf "%-5s %-10s %-4s\n" No Name Mark

`%s`,`%c`,`%d`,`%f`都是格式替换符`format substitution character`.

## 玩转环境变量


## 字符串长度获取

    #!/bin/bash

    msg="Hello,World!";
    echo ${#msg}


## 识别当前所用shell

    echo $SHELL
    echo $0

## UID
$UID 为 0

## SHELL 提示符
`PS1`控制默认命令行提示符的格式
`PS2`控制后续命令行提示符的格式

    $PS1


## 文件描述符

什么是文件描述符？

文件描述符是与文件输入，输出相关联的`整数`. 最常见的文件描述符是`stdin`, `stdout`, `stderr`

## stdout stderr

以`2>`的方式进行错误输出
    cat a* 2> err.txt

预留的文件描述符

    0 stdin
    1 stdout
    2 stderr

`stderr`,`stdout`同时输出到一个文件当中

    $ cmd 2>&1 output.txt
    $ cmd &> output.txt

### redirect

Redirecting from a text block enclosed within a script.Sometimes we need to redirect a block of text (multiple lines of text) as standard input. Consider a particular case where the source text is placed within the shell script. A practical usage example is writing a log file header data. It can be performed as follows:

    #!/bin/bash

    cat <<EOF>log.txt
    LOG FILE HEAD
    This is a Test log file
    Function: System statistics
    EOF


### Custom file descriptor

$ exec 3<input.txt
$ cat <&3

## tee命令

tee - read from standard input and write to standard output and files;
即可以将数据重定向到文件，还可以提供一份重定向的副本作为后续命令的`stdin`，注意，它不能读取`stderr`的内容


## 创建自定义文件描述符

open
    exec 3<input.txt
    cat<&3

write

    $ exec 4>output.txt # open file and write into
    $ echo newline >&4
    $ cat output.txt
    newline

## Arrays and associative arrays


### define Array

    array_var=(1 2 3 4 5 6)
    #
    array_var[0]=1
    array_var[1]=2
    array_var[2]=3

### Print all of the values in an array as list

    echo ${array[@]}
    #
    echo ${array[*]}


### Associative arrays

Associative arrays have been introduced to Bash from version 4.0

In an associative array, we can use any text data as an array index. However, ordinary arrays can only use integers for array indexing.
Initially, a declaration statement is required to declare a variable name as an associative array. A declaration can be made as follows:

    $ declare -A ass_array

    $ ass_array=([index1]=val1 [index2]=val2)

    $ ass_array[index]=val1

list of array index

    $ echo ${!array_var[*]}

    $ echo ${!array_var[@]}

Example

    #!/bin/bash

    declare -A ass_array;
    ass_array=([index1]='hello' [index2]='world');
    echo 'list of array index' ${!ass_array[*]};

    array_var=(1 2 3 4 5 6);
    echo "index test:" ${!array_var[@]};




## time

    $date
    2014年 05月 04日 星期日 09:22:05 CST

    $date +%s
    1399166637

## sleep

    在脚本中进行一段时间的延迟

## DEBUG

    set -x
    echo $msg
    set +x

## function

    function fName()
    {

    }

    ## or

    fName()
    {

    }

    $ fName ;

    fName arg1 arg2 ;


其中`$@` 要比　`$*`更为常用，由于`$*`被当作单个字符串，因而它很少被人所使用。



### Fork炸弹代码与解释

    :(){ :|:& };:

    :()      # 定义函数,函数名为":",即每当输入":"时就会自动调用{}内代码
    {        # ":"函数开始标识
    :    # 用递归方式调用":"函数本身
    |    # 并用管道(pipe)将其输出引至...（因为有一个管道操作符，因此会生成一个新的进程）
    :    # 另一次递归调用的":"函数
    # 综上,":|:"表示的即是每次调用函数":"的时候就会生成两份拷贝
    &    # 调用间脱钩,以使最初的":"函数被杀死后为其所调用的两个":"函数还能继续执行
    }        # ":"函数结束标识
    ;        # ":"函数定义结束后将要进行的操作...
    :        # 调用":"函数,"引爆"fork炸弹



## 管道

当命令在`shell`中执行时，不会对当前的`shell`产生影响。所改变的仅限于子`shell`内
    pwd;
    echo --------------------------;
    (cd ~/Documents; pwd);
    echo ==========================;
    pwd;
