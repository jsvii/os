#!/bin/zsh

ls *.org | sed "s/\.org//" |
while IFS= read -r line; do
    echo $line
    echo "---"
    mv ${line}".org" ${line}
done
