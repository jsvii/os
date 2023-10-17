#!/bin/bash
# This file is used to execute

CONFIG="../src/protected/config/configlocal.php"

#cat $CONFIG | gawk -F ');' '/define/ {print $1}' | sed "s/define(//g; s/);/ /g" | gawk -F "," '{print $1 " value is" $2}'
echo "test new-----"


declare -A ass_array;

cat $CONFIG | sed -n 's/define(//; s/);//p' | gawk -F , '{ass_array[$1]=$2}'

echo $ass_array;
## | sed "s/define(/ /g; s/);/ /g"
