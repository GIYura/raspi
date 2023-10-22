#!/bin/bash

DEV_FILE=/dev/lcd-driver

if [ ! -e "$DEV_FILE" ]; then
	echo "Device file does not exit"
	exit
fi

echo $1 > $DEV_FILE

