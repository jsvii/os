Understand linux programming
============================


# man

man is the system's manual pager. Each page argument given to man is normally the name of a program, utility or function.  The  manual  page associated with each of these arguments is then found and displayed. A section, if provided, will direct man to look only in that section of the manual.  The default action is to search in all of the available sections following a pre-defined order ("1 n l 8 3 2 3posix 3pm 3perl 5 4 9 6 7" by default, unless overridden by the SECTION directive in /etc/manpath.config), and to show only the first page found, even if page exists in several sections.

`man`文件是系统的用户手册。

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


# include 的文件 来自哪里

    /usr/include
    /lib


# exit(1)
警告信息如下

    exit(1);
    warning: incompatible implicit declaration of built-in function

解决方案

    // method 1
    gcc -fno-builtin-exit demo.c

    // method 2
    #include <stdlib.h>


# ch01

 ch01中 more2.c 比more.c 多作了一些处理，具体比较可根据下面的命令进行理解

    ls /bin | ./more
    ls /bin | ./more2

要将文件重定向到`/dev/tty` 之中。


## /dev/tty

`/dev/tty`是键盘和显示器的设备描述文件，向这个文件写就是显示在用户屏幕之上，读相当于从键盘获取用户输入。


# POSIX


# utmp

`utmp` user temporary login in.
