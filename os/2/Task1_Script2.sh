#!/bin/bash
# Script 2
# check if argument count is too small
if (( $# < 3 ))
then
# print error and usage information to stderr
printf "%b" "Error. Not enough arguments.\n" >&2
printf "%b" "usage: myscript file1 op file2\n" >&2
# signal error through non-zero exit code
exit 1
# check if argument count is too big
elif (( $# > 3 ))
then
# print error and usage information to stderr
printf "%b" "Error. Too many arguments.\n" >&2
printf "%b" "usage: myscript file1 op file2\n" >&2
# signal error through non-zero exit code
exit 2
else
# otherise argument count is correct
printf "%b" "Argument count correct. Proceeding...\n"
fi

# if run without arguments or less than 3, "Error. Not enough arguments."
# and usage information usage: myscript file1 op file2" is printed to stderr.
# the error gets signaled by the non-zero exit code 1

# if run with more than 3 arguments, "Error. Too many arguments."
# and usage information is printed to stderr.
# the error gets signaled by the non-zero exit code 2

# if run with 3 arguments, "Argument count correct. Proceeding..." is printed to stdout
# and the script exits succesfully (because it reaches the end)
