#!/bin/bash

echo ------------ ;

fName()
{
    echo $1, $2;
    echo ----------;
    echo $@;
    echo ----------;
    echo $*;
    echo ----------;
    echo "\$0 is", $0;
    return 0;
}

fName 1 2 3 4 5 6 7 8;
