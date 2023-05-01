#!/bin/bash

IFS_OLD=$IFS
IFS=$'\n'
for state in `cat states`
do
    echo "Visit beautiful $state"
done

IFS=$IFS_OLD
echo '-----------------------'
