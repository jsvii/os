#!/bin/bash

export HELLO=/home/user/;
echo $HELLO;
echo '-------------------';

prepend(){ [ -d "$2" ] && eval $1=\"$2\$\{$1:+':'\$$1\}\" && export $1 ;}
echo "HELLO, LEO";
prepend HELLO /home/user/WorkSpace/;
echo '-------------------';
echo $HELLO;
