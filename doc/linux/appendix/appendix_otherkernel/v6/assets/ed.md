ed编辑器
======


## Introduction

`q` 退出

```
$ ed # 进入ed
q
$
```



以`!`开头,进行命令行的输入, `!`被称为`exclamation mark`, aka "bang"
```
leo@Augmentum:~/tmp$ ed test
67
P
*!date
Sat Nov 22 16:45:36 CST 2014
!
*!who
leo      tty6         2014-11-22 13:25
leo      :0           2014-11-22 12:04 (:0)
leo      pts/15       2014-11-22 16:37 (:0)
leo      pts/19       2014-11-22 16:35 (:0)
!
*
```
