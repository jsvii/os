linux Commands
==============



### 执行

```
`commands`
```


### 逻辑运行符

#### && ||

    if [ condition1 ] && [ condition2 ]


#### (())

Math
    (( val1 = 20 * 20 ))

    (( $val1 > 10 ))


### shell脚本参数相关

#### 获取结果

```
$?
```
#### 获取命令

```
$0
```

#### 获取参数

```
$1 $2 $3 $4
```

#### 获取参数长度

```
$#
```

#### 获取所有参数

```
$*  single
$@  array
```

#### 获取最后一个参数

这是强调一下，`${$#}` 不能得到是后一个参数，而是要用 `${!#}`


## test


### 数字操作

    n1 -eq n2  // equal
    n1 -ge n2  // >= great equal
    n1 -gt n2  // great than
    n1 -le n2  // <= less equal
    n1 -lt n2  // less than
    n1 -ne n2  // not equal

### 字符串操作

    =    // equal
    !=   // not equal
    <    // str order

    -n  str  // the string's length is not 0
    -z  str  //  the string's length is 0


becare `a` is `>` `A`, which is different form the `sort` command



### 文件操作

    -d   // is there a directory
    -e   // file exist
    -f   // file exist and type if file
    -s   // file exist and not empy SIZE greater than 0
    -S   // file exist and is a socket
    -O   // am I the ower
    -G   // am I in the group of the file

    -r   // readable
    -w
    -x

    -nt
    file1 -nt file2  // file1 is newer than file2
    -ot
    file1 -ot file2  // file1 is older than file2

## getopt

`getopt` 是`shell`脚本的参数获取的关键工具

```
getopt options optstring parameters
```

案例:
```
getopt ab:cd -a -b test1 -cd test2 test3
```

上面定义了需要有 `-a`, `-b`, `-c`, `-d` 这些参数,其中,`-b`还必须指定参数,不然就不正确

### -q 参数

`-q` 是用于忽略错误信息的,比如上面的例子,`-b`是必须有参数的,但却没有指定,那么就会有错误信息

```
getopt: option requires an argument -- 'b'
```



## 环境变量

很多程序和脚本都是通过环境变量来获取系统信息,存储临时数据和配置信息, 在`linux`的很多地方都可以设置环境变量.你必须了解到这些变量存储在哪里,怎样使用,以及自己如何来创建环境变量.

### 全局环境变量

#### 设置全局环境变量

```
export TERM=xterm-256color
```
#### 删除全局环境变量

```
unset TERM
```

$ printenv

#### 定位系统环境变量

`bash shell`会按照以下的顺序进行逐一执行

- /etc/profile

- $HOME/.bash_profile

- $HOME/.bash_login

- $HOME/.profile




## expr


       STRING : REGEXP
              anchored pattern match of REGEXP in STRING

       match STRING REGEXP
              same as STRING : REGEXP

       substr STRING POS LENGTH
              substring of STRING, POS counted from 1

       index STRING CHARS
              index in STRING where any CHARS is found, or 0

       length STRING
              length of STRING

       + TOKEN
              interpret TOKEN as a string, even if it is a

              keyword like 'match' or an operator like '/'

       ( EXPRESSION )
              value of EXPRESSION


## basename











## gawk



## linux command

difference between `-` and `--`



## man

    man man

show the usage of man

The table below shows the section numbers of the manual followed by the types of pages they contain.

    1   Executable programs or shell commands
    2   System calls (functions provided by the kernel)
    3   Library calls (functions within program libraries)
    4   Special files (usually found in /dev)
    5   File formats and conventions eg /etc/passwd
    6   Games
    7   Miscellaneous (including macro packages and conventions), e.g. man(7), groff(7)
    8   System administration commands (usually only for root)
    9   Kernel routines [Non standard]

## stat

display file or file system status





# cat

    cat -n  # show line number
    cat -s # to single line

# PS

UID User id

PID process id

PPID parent process id

C  CPU的利用率

STIME start time

TTY


    ps -l


    F S   UID   PID  PPID  C PRI  NI ADDR SZ WCHAN  TTY          TIME CMD
    0 S  1000  6426  5904  0  80   0 -  1785 wait   pts/11   00:00:00 bash
    0 S  1000  6444  6426  0  80   0 -  1785 wait   pts/11   00:00:00 bash
    0 R  1000  6485  6444  0  80   0 -  1604 -      pts/11   00:00:00 ps


    leo@Lenovo-G460:~/WorkSpace/bamboo/studylinux$ ps aux | less
    USER       PID %CPU %MEM    VSZ   RSS TTY      STAT START   TIME COMMAND
    USER       PID %CPU %MEM    VSZ   RSS TTY      STAT START   TIME COMMAND
    root         1  0.0  0.1   4600  2432 ?        Ss   21:22   0:01 /sbin/init
    USER       PID %CPU %MEM    VSZ   RSS TTY      STAT START   TIME COMMAND



`ps -ef` 是经常被用到的 `-e`是显示所有， `-f`格式化输出



    leo@Lenovo-G460:~/WorkSpace/bamboo/studylinux$ ps -ef | less
    UID        PID  PPID  C STIME TTY          TIME CMD
    root         1     0  0 21:22 ?        00:00:01 /sbin/init
    root         2     0  0 21:22 ?        00:00:00 [kthreadd]
    root         3     2  0 21:22 ?        00:00:01 [ksoftirqd/0]
    root         5     2  0 21:22 ?        00:00:00 [kworker/0:0H]
    root         7     2  0 21:22 ?        00:00:09 [rcu_sched]
    root         8     2  0 21:22 ?        00:00:00 [rcu_bh]
    root         9     2  0 21:22 ?        00:00:02 [migration/0]

To see every process on the system using standard syntax:
          ps -e
          ps -ef
          ps -eF
          ps -ely

具体的用法有

       To see every process on the system using BSD syntax:
          ps ax
          ps axu

       To print a process tree:
          ps -ejH
          ps axjf

       To get info about threads:
          ps -eLf
          ps axms

       To get security info:
          ps -eo euser,ruser,suser,fuser,f,comm,label
          ps axZ
          ps -eM

       To see every process running as root (real & effective ID) in user format:
          ps -U root -u root u

       To see every process with a user-defined format:
          ps -eo pid,tid,class,rtprio,ni,pri,psr,pcpu,stat,wchan:14,comm
          ps axo stat,euid,ruid,tty,tpgid,sess,pgrp,ppid,pid,pcpu,comm
          ps -Ao pid,tt,user,fname,tmout,f,wchan


# top


# df

`df`


`df -h`




# file

查看文件类型

- 文本文件
- 可执行文件
- 数据文件

# cat

`-n` 显示行号
`-b` 只对有文字的显示行号
`-s` 压缩空白




## usermod

modify a user account

### usermod frequent options

`-l`  `--login` The name of the user will be changed from LOGIN to NEW_LOGIN.


    # usermod -l new_name old_name

`-L` `--lock` Lock a user's password. This puts a '!' in front of the encrypted password, effectively disabling the password. You can't use this option with -p or -U.

`-U`, `--unlock` Unlock a user's password This removes the '!' in front of the
encrypted password

`-p` `--password` This option is not recommended because the password (or encrypted password) will be visible by users listing the processes.

`-G` `--groups`

    usermod -G group_name user_name



## chfn

  The chfn command changes user fullname, office room number, office phone number, and home phone number information for a user's account.

  This information is typically printed by finger(1) and similar programs. A normal user may only change the fields for her own account, subject to the restrictions in /etc/login.defs. (The default configuration is to prevent users from changing their fullname.)

  The superuser may change any field for any account. Additionally, only the superuser may use the -o option to change the undefined portions of the GECOS field.


## group

name, passwd, GID, userlist

    root:x:0:
    daemon:x:1:
    bin:x:2:
    sys:x:3:
    adm:x:4:syslog,leo


`groupadd` to new a group


    # add a new group
    groupadd develop

    # tail /etc/group
    develop:x:1001:

    # add user to a group
    sudo usermod -G develop leo

    # tail /etc/group
    develop:x:1001:leo


## umask

umask 026





# IFS

`IFS` stands for `internal field separator`

    IFS=$'\n'

    IFS=$'\n:;'





## gawk

gawk is cool
