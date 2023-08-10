#!/bin/bash

AWK=`which awk`;
HOSTS="/etc/hosts";

awkscript='!/dev_/{ print $0 } END { print "hello";}';

echo $awkscript;

$AWK "$awkscript" $HOSTS;
