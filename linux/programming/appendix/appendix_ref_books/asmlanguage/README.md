Asm Language
============


# x86

`x86`是小端机

# nasm 是什么

`nasm`全称`netwide assembler`。

`nasm`

    nasm -f elf -g -F stabs eatsyscal.asm

## 选择解释

`-f` 输出的文件格式，有哪些格式可供选择呢？因为我的机器是 32位 ubuntu14.04，所以选择了`elf32`，可以在命令行中用`elf`来替代。

合法的格式有以下


    * bin       flat-form binary files (e.g. DOS .COM, .SYS)
    ith       Intel hex
    srec      Motorola S-records
    aout      Linux a.out object files
    aoutb     NetBSD/FreeBSD a.out object files
    coff      COFF (i386) object files (e.g. DJGPP for DOS)
    elf32     ELF32 (i386) object files (e.g. Linux)
    elf64     ELF64 (x86_64) object files (e.g. Linux)
    elfx32    ELFX32 (x86_64) object files (e.g. Linux)
    as86      Linux as86 (bin86 version 0.3) object files
    obj       MS-DOS 16-bit/32-bit OMF object files
    win32     Microsoft Win32 (i386) object files
    win64     Microsoft Win64 (x86-64) object files
    rdf       Relocatable Dynamic Object File Format v2.0
    ieee      IEEE-695 (LADsoft variant) object file format
    macho32   NeXTstep/OpenStep/Rhapsody/Darwin/MacOS X (i386) object files
    macho64   NeXTstep/OpenStep/Rhapsody/Darwin/MacOS X (x86_64) object files
    dbg       Trace of all info passed to output stage
    elf       ELF (short name for ELF32)
    macho     MACHO (short name for MACHO32)
    win       WIN (short name for WIN32)



## 选项 -g

分成`debug`的信息（存疑）,将调试信息嵌入目标文件，从而方便使用调试器

## 选项 -F

以不同的格式分成调试信息，在32位linux 下，一般就使用`stabs`格式

你可以在`nasm`中进行查询。如我想知道输出格式为`elf32`的应该用什么调试信息，那么就输入

    $  nasm -felf32 -y
    # -f elf32
    # -y

你会看到以下信息

    valid debug formats for 'elf32' output format are ('*' denotes default):
        dwarf     ELF32 (i386) dwarf debug format for Linux/Unix
      * stabs     ELF32 (i386) stabs debug format for Linux/Unix

## debug 格式

ELF provides debug information in `STABS` and `DWARF` formats.
Line number information is generated for all executable sections, but please note that only the ".text" section is executable by default.

# LD

`ld` 是`Gnu linker`

ld combines a number of object and archive files, relocates their data and ties up symbol references.

用法如下

    ld -o <output-file-name> /lib/crt0.o hello.o -lc

如果用 `-o`选项来指定一个文件名，那么`ld` 就可能分成一个`a.out`的文件

# kdbg



# insight


# gdb

start

    $ gdb

`file` to load program

    $ (gdb) file <program-file-name>

`r` to run

    $ (gdb) r

quit, user `C-d` or `quit` command

    $ (gdb) quit


# 寄存器

寄存器的最低有效位是在最右边的，

    mov eax, 'WXYZ'

在实际实况下， al中存放着`Z`, ah中存放着`Y`,`W`在最高位


下面两个命令是一样的，都是将一个标号的地址移动到`ecx`之中

    mov ecx, 标号
    mov ecx, [标号]

## breakpoint

我们在生成.o文件是，nasm指定了源文件是什么，所以这些信息都保留在了运行的文件当中，直接用`gdb`就可以调试了
我写一个 `b  10`就是说把断点打在`.asm`文件的第10行


## step into 与 step over

`si` `ni` `s` `n`
这四个命令是类似的，但`si`,`ni`只针对汇编指令，而`s`,`n`是针对的源代码。

## information
`i r` information of all registers

## where
`where` 显示当前程序的位置
