#!/bin/bash

#This file generates the data and inputs it into "input_file_name_argument.txt" file

#Delete the file if there exists a file already with same name
rm -f "$1.txt" 

#Create new file
touch "$1.txt"

counter=1
COMPOUND_STR=""
innercount=0
while [ $counter -le $2 ]
do
    STRING="$(pwgen -cn 100 1)"
    val="$(shuf -i 0-2147483647 -n 1)"
    COMPOUND_STR="$COMPOUND_STR${val} ${val} $STRING\n"
    innercount=$((innercount+1))
    if [ $innercount -eq 50 ]
    then
        COMPOUND_STR=${COMPOUND_STR:0:-2}
        echo -e "$COMPOUND_STR">>"$1.txt"
        COMPOUND_STR=""
        innercount=0
    fi
    ((counter++))
done

if [ $innercount -ne 0 ]
then 
    COMPOUND_STR=${COMPOUND_STR:0:-2}
    echo -e "$COMPOUND_STR">>"$1.txt"
    COMPOUND_STR=""
fi
