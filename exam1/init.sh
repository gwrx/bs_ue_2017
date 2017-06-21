#!/bin/bash

# run inside the directory where the executables are

if [ ! -w REQUEST_FIFO ]
	then
		mkfifo REQUEST_FIFO
fi

gcc peter.c -o peter
gcc student.c -o student



./peter &

COUNTER=0
while [ $COUNTER -le 30 ]; do
	./student $COUNTER
	let COUNTER=COUNTER+1
done
