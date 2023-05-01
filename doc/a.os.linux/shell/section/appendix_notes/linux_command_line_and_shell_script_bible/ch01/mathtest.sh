#!/bin/bash

no1=4;
no2=5;
let result=no1+no2;
echo "result is:" $result;

result2=$[no1 + 99]
echo "result is: " $result2;

result3=$[$no1 + 99]
echo "result3 is: " $result3;

result4=$(( no1 + 90));
echo "result4 is: " $result4;

result5=$(( $no1 + 90));
echo "result5 is: " $result5;

no10=54;
result10=`echo "$no10 * 1.5" | bc`;
echo "result10 is: " $result10;

result11=`echo "scale=10;1/3" | bc`;
echo "result11 is: " $result11;
