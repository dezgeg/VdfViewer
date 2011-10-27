#!/bin/sh
mkdir output/ 2>/dev/null
FILEBASE=`basename "$1" 2>/dev/null | sed -e 's/\(.*\)\..*/\1/'`
OUTFILE=`echo "output/$FILEBASE.out"`
rm -rf "$OUTFILE"
./planets "$1" "$OUTFILE" || exit 1
./plot.m "$OUTFILE" "$FILEBASE" "$2"
if [ ! "$2" = "-n" ]
then
	echo -n "Generating output/$FILEBASE.gif... "
	convert -delay 20 -loop 0 "output/$FILEBASE-frame"*.gif "output/$FILEBASE.gif"
	rm -rf "output/$FILEBASE-frame"*.gif
	echo "complete"
fi
