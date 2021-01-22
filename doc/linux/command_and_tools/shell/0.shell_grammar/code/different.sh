#!/bin/sh

# ./different.sh --version 1.1.0 --copyright linux

echo '$* =' $*

echo '$@ =' $@


echo '-------- $* example ----------'
for var in $*
do
echo $var
done

echo '-------- "$*" example ----------'
for var in "$*"
do
echo $var
done



echo '-------- $@ example ----------'
for var in $@
do
echo $var
done


echo '-------- "$@" example ----------'
for var in "$@"
do
echo $var
done
