#!/bin/bash
# Script 3
# store the first 2 arguments in variables INFILE and OUTFILE
INFILE=$1
OUTFILE=$2
# check if INFILE exists
if [ -e "$INFILE" ]
then
# check if OUTFILE is writable
if [ -w "$OUTFILE" ]
then
# on successful checks append INFILE to OUTFILE
cat "$INFILE" >> "$OUTFILE"
else
# if OUTFILE is not writable, output error (to stdout)
echo "can not write to $OUTFILE"
fi
else
# if INFILE is not writable, output error (to stdout)
echo "can not read from $INFILE"
fi

# if run without arguments, the INFILE exists check will fail
# and the script will output "can not read from "

# if both a readable INFILE (first argument) and a writable OUTFILE (second argument) are passed as arguments
# the script will append INFILE to OUTFILE without printing anything

# otherwise if INFILE is not readable or OUTFILE is not writable
# the appropriate error message will be printed to stdout

# in either case the exit-code will be 0
