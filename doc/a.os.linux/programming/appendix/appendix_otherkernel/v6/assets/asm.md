汇编语言
=======


## stack



## 关于C语言

unix v6中C语言的函数，都会被编译器转化为带有_的函数，所以在汇编中，可以看到很多带有_的label，其实就是用于定义一个函数。

## Summary of Addressing mode

The general registers may be used with an instruction in any of the following ways:


- as accumulators. The data to be manipulated resides within the register.
- as pointers. The contents of the register are the address of the operand, rather than the operand itself.

- as pointers which automatically step through core locations. Automatically stepping forward through consecutive core locations is known as autoincrement addressing; automatically stepping backwards is known as autodecrement addressing. These modes are particularly useful for processing tabular data.

- as index registers. In this instance the contents of the register, and the word following the instruction are summed to produce the address of the operand. This allows easy access to variable entries in a list.

PDP 11's also have instruction addressing mode combinations which facilitate temporary data storage structures for convenient handling of data which must be frequently accessed. This is known as the "stack".

In the PDP 11 any register can be used as a "stack pointer"under program control, however, certain instructions associated with subroutine linkage and inter· rupt service automatically use Register 6 as a "hardware stack pointer". For this reason R6 is frequently referred to as the "SP"
R7 is used by the processor as its program counter (PC). It is recommended that R7 not be used as a stack pointer.


### Single Operand Addressing

The instruction format for all single operand instructions (such as clear, increment, test) is:

|---------------------------------------------------------------|
| 15| 14| 13| 12| 11| 10| 9 | 8 | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
|---------------------------------------------------------------|
|                                       |  Mode     |  Rn       |
|---------------------------------------------------------------|
|  OP CODE                              |  Destination Address  |
|---------------------------------------------------------------|

- Bits 15 through 6 specify the operation code that defines the type of instruction to be executed.
- Bits 5 through 0 form a six-bit field called the destination address field. This consists of two subfields:
  1. Bits 0 through 2 specify which of the eight general purpose registers is to be referenced by this instruction word.
  1. Bits 3 through 5 specify how the selected register will be used (address mode). *Bit 3* indicates direct or deferred (indirect) addressing.


### Double Operand Addressing

Operations which imply two operands (such as add, subtract, move and compare) are handled by instructions that specify two addresses. The first operand is called the source operand, the second the destination operand. Bit assignments in the source and destination address fields may specify different modes and different registers. The Instruction format for the double operand instruction is:

|---------------------------------------------------------------|
| 15| 14| 13| 12| 11| 10| 9 | 8 | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
|---------------------------------------------------------------|
|               |  Mode     |   Rn      |  Mode     |  Rn       |
|---------------------------------------------------------------|
|  OP CODE      |  Source Address       |  Destination Address  |
|---------------------------------------------------------------|



## 寄存器

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

### PS

`PSW` is `processor status word`是处理器状态字,`PSD`是一个被称为处理器状态字的16位寄存器，Processor Status Word PSW (at 0777776)

下面的描述出自于 **unix内核源码剖析**

|  比特位    |              含义                                               |
| --------- | ---------------------------------------------------------------|
|   15-14   | 处理器当前模式, 00为内核模式, 11为用户模式,永远不出现01或10的情况           |
|   13-12   | 处理器先前模式, 00为内核模式, 11为用户模式,永远不出出现01或10的情况           |
|   7-5     | 处理器优先级                                                     |
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

`MSB` is "Most Significant bit"最重要的一位。一般指符号位。

`NZVC`



## 指令

### ADC

将C位加至目的操作数


## 寻址方式

### 寄存器方式


```
clr r0 将r0设为 0  clr为clear的意思
mov r1, r2
add r4, r2
```
### 寄存器延迟方式

指寄存器中包含着操作数的地址

```
inc (r1)
asr (sp)  向sp所表示的地址中的值, 全部向右移1位,同时每15位装入原先C位的值 (非常有意义,因为C也表示着最高位,现在到达最低位了,真正的向右移操作)
add (r2), r1
```


### 先操作,后自动增长1

寄存器中包含着操作数的地址,作为一个副作用,寄存器的值都要加上1
```
clr    (r1)+
mfpi   (r0)+
```

### 先自动减1,后操作方式

寄存器的值先减1,然后定位操作数

```
inc -(r0)
mov -(r1), r2
```



##


## Instructions


| Mnemonic |  Name               |  Binary opcode |  Branch condition |   Comments                 |
|----------|---------------------|----------------|-------------------|----------------------------|
| BR       | Branch              |   00000001     |    unconditional  |                            |
| BNE      | Branch on not equal |   00000010     |    Z=0            |                            |
| BEQ      | Branch on equal     |   00000011     |    Z=1            |       |
| BPL      | Branch on Plus      |   10000000     |    N=0            |       |
| BMI      | Branch on Minus     |   10000001     |    N=1            |      |
| BVC      | V=0
| BVS      | V=1
BHIS C=0
BCC C=0
BLO C=1
BCS C=1
BGE N⊕V=0
BLT N⊕V=1

BGT Z ∨ (N ⊕ V) = 0 BLE Z ∨ (N ⊕ V) = 1 BHI C∨Z=0 BLOS C∨Z=1

BES  Branch on Error set
BEC  Branch on Error clear
