#!/bin/zsh

currentDir=`pwd`

# @chameleon/theme/classical/style.scss
keyword="doc"
keywordReg="(\.\./)+${keyword}/"
#keywordReg="${keyword}"
#sedReg="(\.\./|\./)*${keyword}"
replaceTarget="@chameleon/doc/"

find ./ -type f -iname "*.js" | xargs grep -oP ${keywordReg} |
while IFS= read -r line; do
    echo $line
    jsFile=$(echo "$line"  | gawk -F":" '{ print $1}')
    keywordCapture=$(echo "$line"  | gawk -F":" '{ print $2}')
    #
    echo $jsFile;
    echo $keywordCapture
    echo "-----------------------------"

    sed -E -i "s~${keywordReg}~${replaceTarget}~g" $jsFile
#    cd $currentDir
done
