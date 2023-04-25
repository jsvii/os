#!/bin/bash

term=$TERM;
simple='\$ ';
detail='\u@\h:\w\$ ';

if [ $term = 'eterm-color' ]
then
    PS1=$simple;
else
    PS1=$detail;
fi
