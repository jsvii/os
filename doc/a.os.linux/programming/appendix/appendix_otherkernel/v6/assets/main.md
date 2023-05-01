Lion's Commentary on UNIX 6th Edition with Source Code
======================================================

## PDP11 机器特性

### PDP-11的一些特征

- 指令和数据基本都是16比特为单位进行的
- 总线名为Unibus,具有18比特宽的地址总结
- 以Word或byte为单位进程处理
- 异步操作
- 模块化组件
- 栈处理
- 自动化优先级处理

### 通用寄存器

| 名称  | 数量  | 别名  | 描述                             |
| ---- | ---- | ---- | ------------------------------- |
| r0   |  1   |      | 1,表达式求值时作为临时累加器. 2, 在过程返回时存放返回值. 3,在过程调用的*某些情况*下传参(更多的其实是用栈了) |
| r1   |  1   |      | 同 r1                           |
| r2   |  1   |      | 在过程调用时用作局部变量.在过程调用入口处存放这些值,在过程调用结束时要恢复这些值 |
| r3   |  1   |      | 同 r2                           |
| r4   |  1   |      | 同 r2                           |
| r5   |  1   |      | 被称为帧指针或环境指针,用作过程激活记录动态链的链首指针         |
| r6   |  2   | sp   | 栈指针,它有两份.用户态,核心态下各一份                     |
| r7   |  1   | pc   | 程序计数器,也叫指令地址寄存器                            |

### 最高位8K空间

PDP-11/40以及周边设备的寄存器被映射到内存最高的8KB空间, 因此都采用操作内存相同的方法来操纵寄存器.这种方法叫做**内存映射I/O** (Memory Mapped I/O).

最高位的8K字节是作为输入,输出设备界面寄存器的编址(也包括外理器中的各和寄存器),这个称为输入,输出页.

### 地址总线的寻址能力以及MMU内存管理

PDP-11/40 拥有18位的寻址能力, 所以理论上程序可以达到256K - 8K = 248K 的内存地址(即内存可以做成248KB大小的), 总线具有很高的数据传输速率，其最大传输速率达到40Mbit/s（当前计算机前端传输速率可达1033Mbit/s，最新型I/O总线PCI-Express则可达102.4Gbit/s）

248KB的内存地址是物理存在的,因为地址总结有这样的寻址能力.

用MMU进行管理时,使用的是16位的`虚拟地址`.因为虚拟地址有16位,所以程序拥有`64KB`的空间,它被分为8页,每页长`8KB`

```
-------------------------------------------
| APF     | BN              | DIM         |
-------------------------------------------
|15 14 13 |12 11 10 9 8 7 6 | 5 4 3 2 1 0 |
-------------------------------------------
```

上图描述了一个虚拟地址的三个部分

- `APF` `active page field` 活动页字段, 是`15 ~ 13`位,这样能表达8个不同的状态,所以有8页.每页又分为`128`个字符块,每个字符块长度为`64`字节.

- `BN` `Block Number`是每页分成的128个字符块

- `DIB` `distant indent block` 每个字符块中的偏移量

虚地址必须要转化为实际物理地址,这步工作是通过`MMU`来进行管理的,`MMU`分两组,一组是核心态,一组是用户态,每组都有8个`APR`

下面介绍一下`APR`,一个`APR`由一个`PAR` 页地址寄存器和一个`PDR`组成

`PAR` `page address register`的结构如下,真正用到的其实只有 `0 ~ 11`位,这12位被称为`PAF` `页地址字段`,它说明的是虚拟地址空间中相应页在物理空间中的起始地址,单位是内存字符块,所以它的寻址范围是 `0 ~ (2^12 -1 )* 64`字节,即 `0 ~ (256K - 64)`字节

```
------------------------------------------
|            |   PAF                     |
------------------------------------------
|15 14 13 12 | 11 10 9 8 7 6 5 4 3 2 1 0 |
------------------------------------------
```

`PDR` `page description register`是页说明寄存器,它包含了页长,存取方式和扩展方向等信息,它的结构如下:

`PDR` 结构图

```
---------------------------------------------------------
|    | PLF                 |   | W |     | ED | ACF |   |
---------------------------------------------------------
| 15 | 14 13 12 11 10 9 8  | 7 | 6 | 5 4 | 3  | 2 1 | 0 |
---------------------------------------------------------
```

`ACF` 存取控制字段,它说明了该页的存取权

| ACF     |  存取权      | 说明               |
|---------|------------|--------------------|
| 00      | 不驻在内存    | 任何存取操作都会造成错误 |
| 01      | 只读         | 企图写操作会出错      |
| 10      | (未用)       | 任何存取操作都会造成错误|
| 01      | 读写         |                   |

`W`位,存取信息位,表示相应的APR字最近一次装入后,这一页是否写过,1 表示进行过写操作,反之则否.如果有一页的内外存都有副本,那么在将内存副本称作它用前,要检查一下这一页,如果其值为0,就直接销毁,反之,则要将内存中的这一页数据,移到外存当中,保证数据的正确性.

`ED`是扩展方向位,如果一个虚拟页实际使用长度小于128个内存字符块,那么就要用ED来说明这个页的空白部分是在低地址一侧还是高地址一侧,这也说明了如果要扩展这个虚拟页,是要从低地址向高地址扩展,还是从高地址从低地低扩展.在unix中,纯正文和数据段使用的是ED为0的情况,表示空白部分在高地址一端.而栈段使用提是ED为1的方式.扩充方向只对于实际使用小于128个内存字符块的虚页才有意义.

`PLF` `page length field` 它说明的是虚拟页的使用长度(字符块数量).它的变化范围是`0 ~ 128`.它的算法与`ED`相关. 若 `ED`为0,那么,实际长度就是PLF的值加1,若PLF的值是0,那么实际使用的长度为1(至少要使用1页的吧!),那`PLF`为127,那么,实际的长度是128.如果`ED`为1,那么实际长度就是128减去`PLF`的值,即PLF为127,那么使用长度就是1.


##### 虚地址如何转换为实际的物理地址?

1. 根据运行的状态选择相就的活动页寄存器组(内核态,用户态?)
1. 根据虚拟地址的APF选择一组活动的页寄存器`APR`
1. 被选到的`PAR`的`PAF`表示了这一页在内存中的起始块号
1. 虚拟地址字中的`BN`加上`PAR`中的`PAF`字段,得到的就是物理地址的块号
1. 虚拟地址中的DIB与物理块号相连,就构成了真正的18位物理地址


#### 面板寄存器 panel register (at 0777570)

- 面板寄存器

There are two panel registers: a read only switch register (SR), which mirrors the state of the panel switches, and a write only display register (DR) to control the panel lights.
有两种面板寄存器，一个是只读的开关寄存器，表着着面板上的开关状态.另一个是只写寄存器，用来控制面板上的灯

- 面板寄存器在电源关掉情况下

After power on, all but two registers are set to zero. The IPL in the PSW is set to 7 and the switch register is left as it is.

切断电源之后，这两种寄存器都会被置为0，但`PSW`中的`IPL`位会被设为7，其他都保留以前的状态。


#### RK05

维基百科上关于rk05的介绍。[RK05](http://en.wikipedia.org/wiki/RK05) `PK05`是由`DEC`公司制作的一款移头式磁盘驱动设备，在14英寸的磁盒内存储了2.5MB的数据

The RK05 DECpack was a moving head magnetic disk drive manufactured by the Digital Equipment Corporation of Maynard, Massachusetts. It stored approximately 2.5 MB on a 14", single-platter IBM-2315-style front-loading removable disk cartridge. The cartridge permitted users to have relatively unlimited off-line storage and to have very fast access to such data.

At the time DEC had numerous operating systems for each computer architecture so operating systems could also be changed quickly. The RK05 was the disk successor to DECtape for personal, portable, and expandable storage. While the smallest practical configuration was two drives, many systems had four or more drives.

## Unix v6 基本配置

参见 [如何在simh中安装 unixv6 操作系统](http://www.gunkies.org/wiki/Installing_Unix_v6_(PDP-11)_on_SIMH)

### 系统的安装

```
$ pdp11
PDP-11 simulator V3.8-1
sim> set cpu 11/40
Disabling XQ
sim> set tm0 locked
sim> attach tm0 dist.tap
sim> attach rk0 rk0
RK: creating new file
sim> attach rk1 rk1
RK: creating new file
sim> attach rk2 rk2
RK: creating new file
sim> d cpu 100000 012700
sim> d cpu 100002 172526
sim> d cpu 100004 010040
sim> d cpu 100006 012740
sim> d cpu 100010 060003
sim> d cpu 100012 000777
sim> g 100000
# <Ctrl> E  here
Simulation stopped, PC: 100012 (BR 100012)
sim> g 0
=tmrk
disk offset
0
tape offset
100
count
1
=tmrk
disk offset
1
tape offset
101
count
3999
=
Simulation stopped, PC: 137274 (TSTB @#177560)
sim> q
Goodbye
$ ~/v6$ ll # 进程文件查看 rk0
total 8160
drwxr-xr-x  3 leo user    4096 Nov 18 18:37 ./
drwxr-xr-x 56 leo user    4096 Nov 18 12:50 ../
-rw-r--r--  1 leo user 6291996 Nov 18 13:52 dist.tap
-rw-r--r--  1 leo user 2048000 Nov 18 18:39 rk0
-rw-r--r--  1 leo user       0 Nov 18 18:37 rk1
-rw-r--r--  1 leo user       0 Nov 18 18:37 rk2
```


### 初始化启动项

```
PDP-11 simulator V3.8-1
sim>  set cpu 11/40
Disabling XQ
sim> set tto 7b
sim> set tm0 locked
sim> attach tm0 dist.tap
sim> attach rk0 rk0
sim> attach rk1 rk1
sim> attach rk2 rk2
sim> dep system sr 173030
sim> boot rk0
@rkunix
mem = 1035
RESTRICTED RIGHTS
Use, duplication or disclosure is subject to
restrictions stated in Contract with Western
Electric Company, Inc.
```

### Booting up to single user mode 以单用户模式启动

And this will boot us up to the bootloader, to which we just tell it to load the 'unix' kernel.

```
$ pdp11
#
PDP-11 simulator V3.8-1
sim> set cpu 11/40
Disabling XQ
sim> set tto 7b
sim> set tm0 locked
sim> attach tm0 dist.tap
sim> attach rk0 rk0
sim> attach rk1 rk1
sim> attach rk2 rk2
sim> dep system sr 173030
sim> boot rk0
@rklinux
@rkunix
mem = 1035
RESTRICTED RIGHTS
Use, duplication or disclosure is subject to
restrictions stated in Contract with Western
Electric Company, Inc.
# LS
```

执行下一命令,让终端以小写字母进行显示

```
# STTY -LCASE
```

### 重新构建内核

我们必须去重新构建内核,以使它适应SIMH所提供的硬件, 第一步就是我们必须生成 mkconf这一个程序

```
chdir /usr/sys/conf
cc mkconf.c
mv a.out mkconf
```

### 正常启动 start normally


```
set cpu 11/40
set cpu idle
set tto 7b
set tm0 locked
attach rk0 rk0
attach rk1 rk1
attach rk2 rk2
attach lpt printer.txt
set dci en
set dci lines=8
set dco 7b
att dci 5555
boot rk0
PDP-11 simulator V3.8-1
Disabling XQ
Listening on port 5555 (socket 108)
@unix
login: root  // 以root身份登陆
```

## Unix v6 分析

### 存储管理

#### 核心态下,进程的虚拟地址空间

核心态中`0-5`页存放的是`unix`操作系统的代码,不同系统配置的unix代码的长度是不相同的,`PDP-11/40`这一机型,代码长度不能超过`48KB`,如果小于这个长度,那么一些虚拟空间就弃而不用.这里装载的是内核程序

最后一页是*输入,输出页*,实际上就是对应的最高地址上的`8KB`空间.

只有第6页是非常特殊的,它对应的是当前进程的ppda区,实际长度是1024字节,它随着进程的变化而变化.

```
-----------------------------------------
| 0         |                           |
| 1         |                           |
| 2         |  这六页存放 unix 内核代码      |
| 3         |                           |
| 4         |                           |
| 5         |                           |
-----------------------------------------
| 6         | 当前进程的PPDA               |
-----------------------------------------
| 7         | 输入,输出页                  |
-----------------------------------------
```

`ppda` is `Pre Process Data Area` 有以下特性

1. 由`user`结构体和内核栈区域构成
1. 上面讲的内核栈区域,是内核处理这一进程时的临时工作区域,每个进程都有供内核使用的工作区域
1. 这个区域是用户空间无法访问的,它的长度为USIZE * 64字节,即 1KB


#### 用户态下,进程的虚拟地址空间

用户态下,进程的虚拟地址空间的分布没有核心态下那么泾渭分明,它在逻辑上可以分为三段,顺序上也是下面这样:

1. 共享正文段
1. 数据段
1. 栈段

每段都必须占用整数页,比如正文段实际使用长度为2.5页,那么就必须使用3页


#### 内存中进程图像的分布


虚拟地址空间上述的分配规则不能照搬到物理内存当中去.具体参见**unix 操作系统教程**中的第96页

* unix 代码段是从地址0开始的低地址区

* `ppda`,数据段,栈段三者在物理空间中是紧靠在一起的.

* 共享正文段是多个进程共享的


### 进程管理


#### proc结构体

`/root/usr/sys/proc.h`下代码

```
struct	proc
{
    char	p_stat;
    char	p_flag;
    char	p_pri;		/* priority, negative is high */
    char	p_sig;		/* signal number sent to this process */
    char	p_uid;		/* user id, used to direct tty signals */
    char	p_time;		/* resident time for scheduling */
    char	p_cpu;		/* cpu usage for scheduling */
    char	p_nice;		/* nice for scheduling */
    int	p_ttyp;		/* controlling tty */
    int	p_pid;		/* unique process id */
    int	p_ppid;		/* process id of parent */
    int	p_addr;		/* address of swappable image */
    int	p_size;		/* size of swappable image (*64 bytes) */
    int	p_wchan;	/* event process is awaiting */
    int	*p_textp;	/* pointer to text structure */
} proc[NPROC];
/* stat codes */
#define	SSLEEP	1		/* sleeping on high priority */
#define	SWAIT	2		/* sleeping on low priority */
#define	SRUN	3		/* running */
#define	SIDL	4		/* intermediate state in process creation */
#define	SZOMB	5		/* intermediate state in process termination */
#define	SSTOP	6		/* process being traced */
/* flag codes */
#define	SLOAD	01		/* in core */
#define	SSYS	02		/* scheduling process */
#define	SLOCK	04		/* process cannot be swapped */
#define	SSWAP	010		/* process is being swapped out */
#define	STRC	020		/* process is being traced */
#define	SWTED	040		/* another tracing flag */
```

`proc.h`中元素的含义


| member | desc                                                          |
| ------ | ------------------------------------------------------------- |
| p_stat | 状态.等于NULL意味着proc[]数组中这个元素为空,也就是没有这个进程              |
| p_flag | 标志变量.                                                       |
| p_pri  | 执行优先级.                                                      |
| p_sig  | 信号                                                           |
| p_uid  | user id                                                       |
| p_time | 在内存中或交换空间中存在的时间                                         |
| p_cpu | 占用CPU的累计时间                                                   |
| p_nice | 用来降低执行优先级的补正系数                                           |
| p_ttyp | 正在操作进程的终端                                                  |
| p_pid | 进程ID                                                          |
| p_ppid | 父进程ID                                                          |
| p_addr | 数据段的物理地址 (单位为64字节)                                        |
| p_size | 数据段的长度 (单位为64字节)                                           |
| p_wchan | 使进程进入休眠状态的原因                                             |
| *p_textp | 使用的代码段                                                     |


在`/root/usr/sys/param.h`中定义了一个最大进程数量,就是说不可能创建`50`个以上的进程

```
#define	NPROC	50		/* max number of processes */
```


#### 进程状态概述

进程有以下状态,即 `proc` 中 `p_statu`

- SRUN 运行状态

- SSLEEEP 高优先权的睡眠状态

- SWAIT 低优先权的睡眼状态

- SSTOP 暂停状态,等待被跟踪(trace)

- SZOMB 等待善后的处理状态

- SIDL 创建子进程的状态




#### user.h


```
struct user
{
    int	u_rsav[2];		/* save r5,r6 when exchanging stacks */
    int	u_fsav[25];		/* save fp registers */
                                                                    /* rsav and fsav must be first in structure */
    char	u_segflg;		/* flag for IO; user or kernel space */
    char	u_error;		/* return error code */
    char	u_uid;			/* effective user id */
    char	u_gid;			/* effective group id */
    char	u_ruid;			/* real user id */
    char	u_rgid;			/* real group id */
    int	u_procp;		/* pointer to proc structure */
    char	*u_base;		/* base address for IO */
    char	*u_count;		/* bytes remaining for IO */
    char	*u_offset[2];		/* offset in file for IO */
    int	*u_cdir;		/* pointer to inode of current directory */
    char	u_dbuf[DIRSIZ];		/* current pathname component */
    char	*u_dirp;		/* current pointer to inode */
    struct	{			/* current directory entry */
                    int	u_ino;
                    char	u_name[DIRSIZ];
    } u_dent;
    int	*u_pdir;		/* inode of parent directory of dirp */
    int	u_uisa[16];		/* prototype of segmentation addresses */
    int	u_uisd[16];		/* prototype of segmentation descriptors */
    int	u_ofile[NOFILE];	/* pointers to file structures of open files */
    int	u_arg[5];		/* arguments to current system call */
    int	u_tsize;		/* text size (*64) */
    int	u_dsize;		/* data size (*64) */
    int	u_ssize;		/* stack size (*64) */
    int	u_sep;			/* flag for I and D separation */
    int	u_qsav[2];		/* label variable for quits and interrupts */
    int	u_ssav[2];		/* label variable for swapping */
    int	u_signal[NSIG];		/* disposition of signals */
    int	u_utime;		/* this process user time */
    int	u_stime;		/* this process system time */
    int	u_cutime[2];		/* sum of childs' utimes */
    int	u_cstime[2];		/* sum of childs' stimes */
    int	*u_ar0;			/* address of users saved R0 */
    int	u_prof[4];		/* profile arguments */
    char	u_intflg;		/* catch intr from sys */
                                                                    /* kernel stack per user
                                                                     * extends from u + USIZE*64
                                                                     * backward not to reach here
                                                                     */
} u;
```


| member      | desc                                                      |
| ----------- | --------------------------------------------------------- |
| u_rsav[]    | 用来保存 r5, r6 当前值,即记录环境指针,栈指针                       |
| u_fsav[]    | pdp-11/40 不使用这一属性                                     |
| u_segflg    | 读写文件时,使用的标志变量                                       |
| u_procp     | user结构体对应的数据proc[]中的表示这一进程的元素,这是一个指针类型 |

系统中的一个全局变量 u,因为所以汇编程序(以`.s`结尾)中的数据都是八进制

```
.globl	_u
_u	= 140000
```

下面我们看一下计算,我们回复一下前面讲到的存储管理,去想一想内核态的第六页存的是什么数据?

```
_u = 014000 // 49152
49152 / (1024 * 8)  // 值为6, 就是说user.h这个结构永远是内核态读取第六页时得到的
```

是当前进程的`ppda`区


#### 进程标志概述

进程标准有以下状态

- SLOAD 进程图像在内存当中

- SSYS 系统进程,不会被交换至交换空间,在UNIX V6中只有proc[0]是系统进程

- SLOCK 进程调度锁,不允许进程被换出至交换空间

- SSWAP 进程图像已经被换出至交换空间,由于被换出至交换空间, user.u_rsav[]的值不再有效,必须从user.ussav[]中恢复r5,r6的值

- STRC 处理被跟踪的状态

- SWTED 在被跟踪时使用


#### 进程状态分析


可以概括为三种状态

1. 运行状态
1. 睡眠状态
1. 其他状态


##### 运行状态

###### 1, 运行状态 ---- 现运行状态和执行状态

处理器上当前时间内运行的进程,它有以下特性:

1. p_stat 为 SRUN
1. p_flag 为 SLOAD
1. 核心态内存管理寄存器 KISA6, KISD6指向这一进程的`ppda`区.
1. 用户态的内存管理寄存器 UISA0 ~ UISA7, UISD0 ~ UISD7指向这一进程的用户态图像占用的内存区
1. 这个进程如果是核心态运行的话,就在核心态存储区活动,执行系统的代码.如果是用户态,就执行的是用户程序或系统提供的其他类非内核程序

###### 2, 运行状态 ---- 就绪状态

这种进程就是在等待处理器的占用,所以称为就绪状态

1. p_stat 为 SRUN
1. p_flag 可能是 SLOAD,也可能不是,这个状态下,不能保证进程图像一定在内存中,也可能是在交换空间当中
1. 即使它的进程图像在内存中,但mmu中的数据与这一进程不相关

##### 睡眼状态

###### 睡眼状态 ---- 进入睡眠状态的特点

1. p_stat 为 `SSLEEP` 或 `SWAIT`
1. p_flag 可能是 SLOAD,也可能不是,这个状态下,不能保证进程图像一定在内存中,也可能是在交换空间当中

###### 睡眼状态 ---- 进入睡眠状态的原因

1. 资源使用互斥

1. 同步等待输出/输出操作的结束

1. 同步等待出现处理对象

1. 进程间的其他同步

1. 临界区互斥



#### fork的实现

下面是 C 标准库中的fork()

```
.globl	_fork, cerror, _par_uid

_fork:                   // 提供给C标准库的,因为是汇编程序,所以以_开头
        mov	r5,-(sp)     // 保存 环境指令到栈顶,因为栈是由高位向低位走的,所以是先执行-1
        mov	sp,r5        // 将新的sp给r5
        sys	fork         // 系统调用
                br 1f    //
        bec	2f           //
        jmp	cerror
1:
        mov	r0,_par_uid
        clr	r0
2:
        mov	(sp)+,r5
        rts	pc
.bss
_par_uid: .=.+2
```


`/root/usr/sys/ken/sys1.c` 下面是系统调用时用到的`fork`方法

```
fork()
{
        register struct proc *p1, *p2;

        p1 = u.u_procp;                    //u_procp是0 ~ 49,代表的是全局变量u的第几个元素
        for(p2 = &proc[0]; p2 < &proc[NPROC]; p2++)
                if(p2->p_stat == NULL)     // 找到未被使用的进程空间
                        goto found;
        u.u_error = EAGAIN;                // 无法新建进程
        goto out;

found:
        if(newproc()) {
                u.u_ar0[R0] = p1->p_pid;   这一
                u.u_cstime[0] = 0;        // 子进程在内核模式下占用的cpu时间(时针tick数)
                u.u_cstime[1] = 0;
                u.u_stime = 0;            // 进程在内核模式下占用的cpu时间(时针tick数)
                u.u_cutime[0] = 0;        // 子进程在用户模式下占用的cpu时间(时针tick数)
                u.u_cutime[1] = 0;
                u.u_utime = 0;
                return;
        }
        u.u_ar0[R0] = p2->p_pid;

out:
        u.u_ar0[R7] =+ 2;
}
```

`/root/usr/sys/ken/slp.c` 中的`newproc()`方法


```
newproc()
{
        int a1, a2;
        struct proc *p, *up;
        register struct proc *rpp;
        register *rip, n;

        p = NULL;
        /*
         * First, just locate a slot for a process
         * and copy the useful info from this process into it.
         * The panic "cannot happen" because fork has already
         * checked for the existence of a slot.
         */
retry:
        mpid++;
        if(mpid < 0) {
                mpid = 0;
                goto retry;
        }
        for(rpp = &proc[0]; rpp < &proc[NPROC]; rpp++) {
                if(rpp->p_stat == NULL && p==NULL)
                        p = rpp;
                if (rpp->p_pid==mpid)
                        goto retry;
        }
        if ((rpp = p)==NULL)
                panic("no procs");   // 无法分配新进程
        /*
         * make proc entry for new proc
         */
        rip = u.u_procp;    // u是当前的进程的user结构体,它的u_procp指向的是当前进程
        up = rip;
        rpp->p_stat = SRUN;  // rpp 是新进程
        rpp->p_flag = SLOAD;
        rpp->p_uid = rip->p_uid;     // p_uid 是用户ID
        rpp->p_ttyp = rip->p_ttyp;   // p_ttyp 是运行的终端 TTY
        rpp->p_nice = rip->p_nice;   // p_nice 用来降低执行优先级的补正系数
        rpp->p_textp = rip->p_textp; // 指向代码段
        rpp->p_pid = mpid;           // 新进程的id
        rpp->p_ppid = rip->p_pid;    // 新进程的父id
        rpp->p_time = 0;             // 执行时间

        /*
         * make duplicate entries
         * where needed
         */

        for(rip = &u.u_ofile[0]; rip < &u.u_ofile[NOFILE];)   //
                if((rpp = *rip++) != NULL)
                        rpp->f_count++;
        if((rpp=up->p_textp) != NULL) {
                rpp->x_count++;
                rpp->x_ccount++;
        }
        u.u_cdir->i_count++;
        /*
         * Partially simulate the environment
         * of the new process so that when it is actually
         * created (by copying) it will look right.
         */
        savu(u.u_rsav);
        rpp = p;
        u.u_procp = rpp;
        rip = up;
        n = rip->p_size;
        a1 = rip->p_addr;
        rpp->p_size = n;
        a2 = malloc(coremap, n);
        /*
         * If there is not enough core for the
         * new process, swap out the current process to generate the
         * copy.
         */
        if(a2 == NULL) {
                rip->p_stat = SIDL;
                rpp->p_addr = a1;
                savu(u.u_ssav);
                xswap(rpp, 0, 0);
                rpp->p_flag =| SSWAP;
                rip->p_stat = SRUN;
        } else {
        /*
         * There is core, so just copy.
         */
                rpp->p_addr = a2;
                while(n--)
                        copyseg(a1++, a2++);
        }
        u.u_procp = rip;
        return(0);
}
```

`/root/usrsys/system.h` mpid 是用于分成进程序列号的一个数字,也是一个系统全局变量


```
int	mpid;			/* generic for unique process id's */
```

mpid是一个一直在增长的数,它不一定要小于50,实现上可以是最大的int,进程的pid不一定是0~50之个范围之内,因为这样增长,更利于执行效率

`u_error` 用于定义系统错误的

```
/* u_error codes */
#define	EFAULT	106
#define	EPERM	1
#define	ENOENT	2
#define	ESRCH	3
#define	EINTR	4
#define	EIO	5
#define	ENXIO	6
#define	E2BIG	7
#define	ENOEXEC	8
#define	EBADF	9
#define	ECHILD	10
#define	EAGAIN	11
#define	ENOMEM	12
#define	EACCES	13
#define	ENOTBLK	15
#define	EBUSY	16
#define	EEXIST	17
#define	EXDEV	18
#define	ENODEV	19
#define	ENOTDIR	20
#define	EISDIR	21
#define	EINVAL	22
#define	ENFILE	23
#define	EMFILE	24
#define	ENOTTY	25
#define	ETXTBSY	26
#define	EFBIG	27
#define	ENOSPC	28
#define	ESPIPE	29
#define	EROFS	30
#define	EMLINK	31
#define	EPIPE	32
```

| 名称    | 描述                                                     |
| ------ | ------------------------------------------------------- |
| EFAULT | 在用户态和内核态之间传递数据失败                                 |
| EPERM  | 当前用户不是超级用户                                          |


#### panic

`UNIX`没有将自己定义为一个具备"容错","个别部件发生故障时仍能工作"等特性的系统,但一些情况下,通过调用"painc()"可以解决一定的问题,在某些情况下,调用这一方法行之有效.

什么情况下会使用到这一方法呢?

- 资源匮乏时,导致内核处理无法继续进行.

它的工作原理,是不停地调用`idle()`以阻止程序继续进行,同时,idle()将处理器优先级设为0,这样它就可以响应所有优先级的中断

```
/*
 * prdev prints a warning message of the
 * form "mesg on dev x/y".
 * x and y are the major and minor parts of
 * the device argument.
 */
panic(s)
char *s;
{
    panicstr = s;
    update();
    printf("panic: %s\n", s);
    for(;;)
        idle();
}
```


## Unix v6 manuals unix v6用户手册

### Unix v6 基本命令

参见本文档中的[unixv6_commands.md](./unixv6_commands.md)


### Unix v6 系统调用


## ASM  汇编指令


对于单操作数指令,其操作数通常称为“目的操作数”;在双操作数指令中,两个操作数分别被称为“源操作数”和“目的操作数”

### 一些汇编指令及其说明

参见本文档中的[pdp11_asm.md](./pdp11_asm.md).

## Simh


## 术语索引

### A

- APR

`APR` 是`active page register` 它由一个`PAR`和一个`PDR`组成, `PAR`有16位,`PDR`也是16位, PDP--11/40 只使用`PAR`的中低12位,这12位被称为页地址字段(`PAF` is `page address field`),它说明的是虚拟地址中相应的页对应的物理地址空间中的起始地址,

### K

- KISA

`KISA` 核心态下的`PAR`

- KISD

`KISD` 核心态下的`PDR`

### M

`MMU` is `Memory Management Unit` 内存控制单元

`MSB` is "Most Significant bit"最重要的一位。一般指符号位。

### P

- PAF

`PAF`见 APR

- PAR

`PAR`见 APR

- PCB

`PCB` is `process control block`进程控制块,也叫做`process descriptor`

- PDR

`PDR`见 APR

- PPDA

`pre process data area`

- PS

`PS`寄存器

```
mov PS, -(sp) 向栈顶存放了PSW
```

- PSW

`PSW` is `processor status word`是处理器状态字,`PSD`是一个被称为处理器状态字的16位寄存器，Processor Status Word PSW (at 0777776)

下面的描述出自于 **unix内核源码剖析**

|  比特位    |              含义                                               |
| --------- | ---------------------------------------------------------------|
|   15-14   | 处理器当前模式, 00为内核模式, 11为用户模式,永远不出现01或10的情况           |
|   13-12   | 处理器先前模式, 00为内核模式, 11为用户模式,永远不出出现01或10的情况           |
|   7-5     | 处理器优先级, 因为是3位，所以优先级是0-7                                                     |
|   4       | trap位                                                        |
|   3       | N 负位                                                        |
|   2       | Z 零位                                                        |
|   1       | V 溢出位                                                       |
|   0       | C 借位                                                        |





下面的描述出自于 **unix manual**

| name  |  width |  meaning                                            |
| ----- | ------ | --------------------------------------------------- |
| CM    | 2      | current operation mode, 00 is kernel mode, 11 user mode |
| PM    | 2      | previous operation mode                             |
| **    | 4      | not used                                            |
| IPL   | 3      | interrupt priority level                            |
| T     | 1      | enable trace trap                                   |
| NZVC  | 4      | condition codes                                     |


###

### U

- UISA

`UISA` 用户态下的`PAR`

- UISD

`UISD` 用户态下的`PDR`


## 重要工具

* [SIMH -- The Computer History Simulation Project](http://simh.trailing-edge.com/) Simh is  a collection of Emulators for 33 different computers.它是一个模拟器,用于模拟PDP8, PDP11, HP2100等历史上的机器

* [QEMU -- machine emulator and virtualizer](http://wiki.qemu.org/Main_Page) 这是一款模拟器,用于模拟不同类型的系统


* [bochs](http://bochs.sourceforge.net/) 这是一个x86 CPU 的模拟器





## 参考资料

* [Unix内核源码剖析](http://www.amazon.cn/%E5%9B%BE%E4%B9%A6/dp/B00IOB0H7I/ref=pd_sim_b_1?ie=UTF8&refRID=0Z523P59WK9WR7YHSRN3) 青柳隆宏 人民邮电出版社

* [Unix 操作系统教程](http://www.amazon.cn/%E9%AB%98%E7%AD%89%E5%AD%A6%E6%A0%A1%E6%95%99%E6%9D%90-UNIX%E6%93%8D%E4%BD%9C%E7%B3%BB%E7%BB%9F%E6%95%99%E7%A8%8B/dp/B00114FUFO/ref=sr_1_1?ie=UTF8&qid=1416214320&sr=8-1&keywords=Unix+%E6%93%8D%E4%BD%9C%E7%B3%BB%E7%BB%9F%E6%95%99%E7%A8%8B) 尤晋元 主编 西安电子科技大学出版社

* [Unix V6 Manuals](http://man.cat-v.org/unix-6th/) 一些unix v6的命令行手册

* [如何在simh中安装 unixv6 操作系统](http://www.gunkies.org/wiki/Installing_Unix_v6_(PDP-11)_on_SIMH)

* [返璞归真--UNIX技术内幕](http://blog.chinaunix.net/uid/23741326/abstract/4.html)

* [CSDN博客 -- 莱昂氏unix源代码分析导读](http://blog.csdn.net/column/details/lions-unix.html)





## 问题汇总

### 问题:在`xv6`中`make bochs`时,发现以下错误

```
========================================================================
                       Bochs x86 Emulator 2.6.7
              Built from SVN snapshot on November 2, 2014
                  Compiled on Nov 14 2014 at 13:23:18
========================================================================
00000000000i[      ] reading configuration from .bochsrc
00000000000p[SIM   ] >>PANIC<< numerical parameter 'n_processors' was set to 2, which is out of range 1 to 1
00000000000e[SIM   ] notify called, but no bxevent_callback function is registered
========================================================================
Bochs is exiting with the following message:
[SIM   ] numerical parameter 'n_processors' was set to 2, which is out of range 1 to 1
========================================================================
00000000000i[SIM   ] quit_sim called with exit code 1
make: *** [bochs] Error 1
```

### 问题:QEMU 在安装后,发现了这个问题

```
VNC server running on `127.0.0.1:5900'
^Cqemu: terminating on signal 2
```

因为没有安装 `SDL`-- 它是`Simple DirectMedia Layer` 的缩写，它是一个跨平台的多媒体库，它通过 OpenGL 和 2D 音频帧缓冲对音频，键盘，鼠标，游戏杆，3D 硬件进行底层访问

```
sudo apt-get install libsdl1.2-dev
```

### 问题: XV6 中的 `QEMU`

要在`Makefile`中指定一下`qemu`
```
QEMU = /usr/local/bin/qemu-system-i386
```

### 问题: bochs configuration

```
sudo apt-get install libgtk2.0-dev
--enable-debugger --enable-disasm  --enable-x86-64
```
