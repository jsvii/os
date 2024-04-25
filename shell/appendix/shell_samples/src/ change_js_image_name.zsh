#!/bin/zsh

currentDir=`pwd`

find ./src -type f -iname "*.js" | xargs grep -oP "'(../)*img/.+'" |
while IFS= read -r line; do
    jsFile=$(echo "$line"  | gawk -F"'|:" '{ print $1}')
    imageFile=$(echo "$line"  | gawk -F"'|:" '{ print $3}')
    jsDir=$(dirname $jsFile)
    jsFileName=$(basename $jsFile)
    orgImageDir=$(dirname $imageFile)

    cd $jsDir
    mkdir -p img
    # cp file to image dir
    cp $imageFile ./img
    sed -i "s~$orgImageDir~./img~" $jsFileName
    cd $currentDir
done
