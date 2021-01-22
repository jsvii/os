#!/bin/bash
# testing string equality

testuser=leo

if [ $USER = $testuser ]
then
    echo "Welcome $testuser"
fi
