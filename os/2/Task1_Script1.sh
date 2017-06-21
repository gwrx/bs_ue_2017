#!/bin/bash
# Script 1
#
# loop through all passed arguments
for FN in "$@"
do
# set file permissions to rwx for owner, r-x for group and nothing (---) for others
chmod 0750 "$FN"
done


# if run without arguments, nothing happens
# otherwise it tries to set file permissions as mentioned for each file passed as argument
