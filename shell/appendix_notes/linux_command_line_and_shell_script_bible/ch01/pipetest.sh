#! /bin/bash

cmd_output=$(ls | cat -n)
echo $cmd_output;


echo --------------------;

cmd_output2=`ls | cat -n`;
echo $cmd_output2;
