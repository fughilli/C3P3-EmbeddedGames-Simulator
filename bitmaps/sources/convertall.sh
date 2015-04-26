#!/bin/bash

convertscript=../../tools/bitmapgen.py
allfiles=$(ls | grep -e ".*\.png$")

for file in $allfiles
do
python $convertscript $file
done

mv *.h *.c ../
	