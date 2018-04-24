#!/bin/bash 

line=$(cat file.txt | wc -l)
if [ "$line" -ge 10 ] ; 

then
	cat file.txt | head -n 10 | tail -n 1

fi

