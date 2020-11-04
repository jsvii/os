#!/bin/bash

CMD="cd /" #Substitute with command for which you need to test exit s
##$CMD
if [ $? -eq 0 ];
then
    echo '$CMD executed successfully';
else
    echo '$CMD terminated unsuccessfully';
fi
