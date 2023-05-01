Unix V6 Commands
===========

## 基本概要

- 如果出现了误打,早期的pdp11是不支持`backspace` 删除的,必须以`#`号对前面的字符进行否认

```
date
// 打错的情形
de#atte##e
```

## 命令一览

- ar

`NAME`: ar	- archive and library maintainer 早期的库命令,用于进行库管理

`SYNOPSIS`

```
ar	key afile name ...
```

`DESCRIPTION`


Ar	maintains groups of files combined into	a single archive file.  Its	main use is to create and update library files as used by the loader.  It can be used, though, for any similar purpose.

Key is one	character from the set drtux, optionally concatenated with	v.  Afile is the archive file.	The names are constituent files in the archive file.The meanings of the key characters are:

`d` means delete the	named files from the archive file.

`r` means replace the named files in	the archive file.  If the archive file does not exist, r creates it.  If the named files are not in the archive file,	they are appended.

`t` prints a	table of contents of the archive file.	If no names are given, all files	in the archive are tabled.  If names are given, only those files are tabled.

`u` is similar to r except that only	those files that have been modified are replaced.  If no	names are given, all files in the archive that have been modified are replaced by the modified version.

`x` extracts	the named files.  If no	names are given, all files in the archive are extracted.  In neither case does x alter the archive file.

`v` means verbose.  Under the verbose option, ar gives a file-by-file description of the making of a new archive file from the old archive and the constituent files.  The following abbreviations are used:

        c copy    a append    d	delete    r replace    x extract

FILES
     /tmp/vtm?       temporary

SEE ALSO
     ld	(I), archive (V)

BUGS
     Option tv should be implemented as	a table	with more infor-
     mation.

 1

     There should be a way to specify the placement of a new file
     in	an archive.  Currently,	it is placed at	the end.

     Since ar has not been rewritten to	deal properly with the
     new file system modes, extracted files have mode 666.

     For the same reason, only the first 8 characters of file
     names are significant.

     If	the same file is mentioned twice in an argument	list, it
     may be put	in the archive twice.

- AS

NAME
     as	- assembler

SYNOPSIS
     as	[ - ] name ...

DESCRIPTION
     As	assembles the concatenation of the named files.  If the
     optional first argument - is used,	all undefined symbols in
     the assembly are treated as global.

     The output	of the assembly	is left	on the file a.out.  It is
     executable	if no errors occurred during the assembly, and if
     there were	no unresolved external references.

FILES
     /lib/as2        pass 2 of the assembler
     /tmp/atm[1-3]?  temporary
     a.out           object

SEE ALSO
     ld	(I), nm	(I), db	(I), a.out (V),	`UNIX Assembler	Manual'.

DIAGNOSTICS
     When an input file	cannot be read,	its name followed by a
     question mark is typed and	assembly ceases.  When syntactic
     or	semantic errors	occur, a single-character diagnostic is
     typed out together	with the line number and the file name in
     which it occurred.  Errors	in pass	1 cause	cancellation of
     pass 2.  The possible errors are:

     )	Parentheses error
     ]	Parentheses error
     <	String not terminated properly
     *	Indirection used illegally
     A	Error in address
     B	Branch instruction is odd or too remote
     E	Error in expression
     F	Error in local (`f' or `b') type symbol
     G	Garbage	(unknown) character
     I	End of file inside an if
     M	Multiply defined symbol	as label
     O	Word quantity assembled	at odd address
     P	`.' different in pass 1	and 2
     R	Relocation error
     U	Undefined symbol
     X	Syntax error

BUGS
     Symbol table overflow is not checked.  x errors can cause
     incorrect line numbers in following diagnostics.

 1


- DATE

display data information

    FRI OCT 10 14:51:03 EDT 1975

- WHO

    ROOT    TTY8 OCT 10 13:02




- stty

`NAME`
     stty - set	typewriter options

`SYNOPSIS`
     stty [ option ... ]

`DESCRIPTION`
     Stty sets certain I/O options on the current output type-writer.  With no argument,	it reports the current settings of	the options.  The option strings are selected from the following set:

```
     even      allow even parity
     -even     disallow	even parity
     odd       allow odd parity
     -odd      disallow	odd parity
     raw       raw mode	input (no erase, kill, interrupt, quit,
               EOT; parity bit passed back)
     -raw      negate raw mode
     cooked    same as `-raw'
     -nl       allow carriage return for new-line, and output
               CR-LF for carriage return or new-line
     nl        accept only new-line to end lines
     echo      echo back every character typed
     -echo     do not echo characters
     lcase     map upper case to lower case
     -lcase    do not map case
     -tabs     replace tabs by spaces when printing
     tabs      preserve	tabs
     ek        reset erase and kill characters back to normal #
               and @.
     erase c   set erase character to c.
     kill c    set kill	character to c.
     cr0 cr1 cr2 cr3
               select style of delay for carriage return (see
               below)
     nl0 nl1 nl2 nl3
               select style of delay for linefeed (see below)
     tab0 tab1 tab2 tab3
               select style of delay for tab (see below)
     ff0 ff1
               select style of delay for form feed (see	below)
     tty33     set all modes suitable for Teletype model 33
     tty37     set all modes suitable for Teletype model 37
     vt05      set all modes suitable for DEC VT05 terminal
     tn300     set all modes suitable for GE Terminet 300
     ti700     set all modes suitable for Texas	Instruments 700
               terminal
     tek       set all modes suitable for Tektronix 4014 terminal
     hup       hang up dataphone on last close.
     -hup      do not hang up dataphone	on last	close.
     0         hang up phone line immediately
     50	75 110 134 150 200 300 600 1200	1800 2400 4800 9600 exta

               extb
               Set typewriter baud rate	to the number given, if
               possible.  (These are the speeds	supported by the
               DH-11 interface).

     The various delay algorithms are tuned to various kinds of
     terminals.  In general the	specifications ending in `0' mean
     no	delay for the corresponding character.
```

`SEE ALSO`
     stty (II)



- CHDIR

    chdir /usr

- LS

    ls -t # show total
    ls -l
