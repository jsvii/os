#!/bin/bash

if [ -d $HOME ]
then
    echo "Your Home directory exists, it is $HOME"
    cd $HOME
    ls -a
else
    echo "Ther is a problem with your HOME directory"
    exit 1
fi

echo '-------------------'
