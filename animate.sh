#!/bin/sh
mkdir output/ 2>/dev/null
FILEBASE=`basename $1 | sed 's/\(.*\)\..*/\1/'`
OUTFILE=`echo output/out.$FILEBASE.txt`
./planets $1 $OUTFILE || exit 1
./plot.m $OUTFILE $FILEBASE
convert -delay 20 -loop 0 output/$FILEBASE-frame*.gif output/$FILEBASE.gif
rm -rf output/$FILEBASE-frame*.gif $OUTFILE
echo Generating output/$FILEBASE.gif and output/$FILEBASE.png complete
