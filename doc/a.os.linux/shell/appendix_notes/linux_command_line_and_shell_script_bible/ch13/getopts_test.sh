#!/bin/bash

set -- `getopt -q ab:c "$@"`

while [ -n "$1" ]
do
    case "$1" in
    -a) echo "Found the -a option";;
    -b) param="$2"
        echo "Found the -b option, with the param value $param"
        shift;;
    -c) echo "Found the -c option";;
    --) shift
        break;;
    *) echo "$1 is not a command";;
    esac
    shift
done

echo '----------------'

count=1
for param in "$@"
do
    echo "Parame #$count is: $param"
    count=$[ $count + 1 ]
done
