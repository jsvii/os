gawk and sed
============


# gawk

格式：

    gawk options program file

选项：

* `-F` fs
指定新的分格符，默认是以空格的. 见`d1.sh`

* `-f` file
指定读取的**程序**的文件名. 见`d2.sh`

* `-v` var=value
指定一个变量及其默认值.

* `-mf` N
指定要处理的数据文件中的最大字段数

* `mr` N

* `-W` key

## 在处理程序之前，先执行BEGIN后的

    gawk  'BEGIN {print "hello World"}'


##


# sed

## 强大的替换规则

s/pattern/replacement/flag

flag

数字  如果指定数字为n,新文本会替换所有旧文本出现的第m处的地方
`g`  新文本会替换所有旧文本出现的地方
`p`  将原来行的内容打印出来，一般和`sed`的`-n`合作使用
