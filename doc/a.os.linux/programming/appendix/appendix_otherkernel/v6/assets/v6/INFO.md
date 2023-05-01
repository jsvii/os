Some information
==============

# script

- Chapter 1.0 Programming the PDP-11
  * 1.1 Installing Unix V6
  * 1.2 Standalone Programs
  * 1.3 Maintaining userland software.


- Chapter 2.0 Interrupts and Multiprocessing
  * 2.1 Memory Management Unit and Multiprocessing
  * 2.2 Dynamic Memory Allocation.
  * 2.3 Dynamic Memory Allocation with the Stack
  * 2.4 Fork() and Exec()

`pdp11` running `unix v6`


    $ pdp11

    PDP-11 simulator V3.8-1
    sim> set cpu u18
    sim> att rk0 unix0_v6_rk.dsk
    sim> att rk1 unix1_v6_rk.dsk
    sim> att rk2 unix2_v6_rk.dsk
    sim> att rk3 unix3_v6_rk.dsk
    sim> boot rk0
    @unix

    login: root

## 分析

    sim> set cpu u18

设置`CPU`类型为 PDP-11/45, `set cpu u18`就是相当于`set cpu 11/45`,<unix 源码分析>一书讲要用`pdp-11/40`，但设置`set cpu 11/40`，程序并不能正常启动，那是因为Ken Thompson 和 Dennis Ritchie是在PDP-11/45, PDP-11/70中开发`v6`，`v7`。







# 哪些特性

- 管理进程
- 内存管理
- 文件系统
- 文件和周边的共享I/O
- 中断
- 支持终端处理

# CPU

[PDP CPU](http://wwwlehre.dhbw-stuttgart.de/~helbig/os/pdp11/doc/cpu.pdf)


## register

共有8种寄存器，但不是8个，因为R6有两个，8种寄存器分别是R0, R1, R2, R3, R4, R5 (FP), R6 (SP), R7 (PC)，这些寄存器都是以3bit长度的地址形式在指令中表示。

其中有三个寄存器的用途可以用它们的名称中得到启示。

`R7` is `PC` Program Count,指明下一个用于执行的指令,以2为步进长度
`R6` is `SP` stack Pointer，有两个`SP`, 一个是`USP`对应着用户模式，一个是`KSP`对应着内核模式
`R5` is `FP`

## PSW

`psd`是一个被称为处理器状态字的16位寄存器，Processor Status Word PSW (at 0777776)


PSW的各位表示

    name  width     meaning
    CM    2         current operation mode, 00 is kernel mode, 11 user mode
    PM    2         previous operation mode
    ***   4         not used
    IPL   3         interrupt priority level
    T     1         enable trace trap
    NZVC  4         condition codes


# MMU

`MMU` is `Memory Management Unit`


MMU通过`APR` Active page register寄存器将虚拟地址变为物理地址。

而1个`APR`是由1个`PAR`和一个`PDR`组成的。

## The Mapping as Controlled by Paging Register
一种被 `Page`寄存器控制的一种映射关系

The virtual address range [0, 64K) is partitioned into eight pages

    [0*8k, 1*8k), [1*8k, 2*8k), [2*8k, 3*8k), ... ,[7*8k, 8*8k)

虚拟内存地址被分为8页

Each page is assigned a page description register (PDR) and a page address register (PAR). The virtual address in
a page are mapped to a range of physical addresses which starts at p = PAR*64.

每一页都有一个`PDR`  page description register，页描述寄存器，`PAR`页地址寄存器。

那么每一页就映射到一组物理地址，这组物理地址以 `PAR`*64 开头


# PDP的时代特征

- 16bit为单位进行处理的，因而对于`DPD11`而言，16bit为一个字长
- PDP-11/40没有专用的I/O总线，而是用一种叫做`unibus`的总线进行数据的输入输出
- `unibus`具有18位宽的地址总线
- PDP-11/40以及周边设备的寄存器被到内在最高位的`8KB`空间,这种方式被称为内在映射I/O -- Memory Mapped I/O

参见以下一个寄存器操作的示例

``` c
    #define REG_ADDRESS 0170000

    struct {
        int integ;
    }

    main()
    {
       int a;
       a = REG_ADDRESS->integ;
       REG_ADDRESS->integ = 0;
    }



```c

# MMU 内存管理单元

`MMU` Memory Management Unit。，它是中央处理器（CPU）中用来管理虚拟存储器、物理存储器的控制线路，同时也负责虚拟地址映射为物理地址，以及提供硬件机制的内存访问授权。

`MMU`在现代处理器中扮演着重要的角色，

## 解决什么问题？

管理两件事

- 地址变换
- 访问权限控制

PDP-11/40以长度为8KB的`segment`或`page`为单位，对进程所需要的内存进行管理，如果试图访问不具备管理权限的内存时，`MMU`就要引发一个`trap exception` 陷入异常。

MMU通过被称为`APR`的寄存器(页寄存器)对各段进行设定，并将虚拟地址转化为物理地址。

## MMU有什么好处（script文件中总结）

- 提供访问更多空间的能力

如果没有MMU，那么，PDP-11的CPU只能访问2^16的内存，因为它的寄存器都是16位的。
但有了MMU就可以访问2^18 的内在，因为需要多2位，在PDP-11出现的第三年，DEC公司给了PDP-11一个MMU。过了三年后，DEC公司又将虚拟地址扩展到32位。

- 重新定位

如果你想加载程序，那么程序之间的地址是不能重叠的，那么原始程序中的地址就必须被重新调整。有了MMU，那么，就可以映射不同的地址了。这也叫做硬件重定位。

- 保护功能

一个程序只能访问它自己的空间，有了MMU，它肯定不能去访问其他程序的空间了。


## 怎么做的？


## 现在操作系统中有类似的么？
