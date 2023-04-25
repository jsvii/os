#!/bin/bash

# test string equal

if [ $TERM = 'xterm' ]
then
    echo 'this is the xterm';
else
    echo 'this is other';
fi

echo '===================';
