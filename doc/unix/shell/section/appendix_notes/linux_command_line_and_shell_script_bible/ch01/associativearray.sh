#!/bin/bash

declare -A ass_array;
ass_array=([index1]='hello' [index2]='world');
echo 'list of array index' ${!ass_array[*]};

array_var=(1 2 3 4 5 6);
echo "index test:" ${!array_var[@]};
