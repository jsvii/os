#!/bin/bash

IFS.OLD=$IFS
IFS=$'\n;:"'

file=strangetxt

for line in `cat $file`
do
    echo 'new line: ' $line
done


IFS=$IFS.OLD
