#!/bin/bash

# path for gpio
DEV_FILE=/dev/my-gpio-driver

# names of states
ON="1"
OFF="0"

ledState()
{
	echo $2 > $1
}

if [ ! -e "$DEV_FILE" ]; then	
	echo "Device file does not exit"
	exit
fi
 
echo "LED is Blinking ... Press 'CNTL+C' to terminate"
while [ 1 ]
do
	ledState $DEV_FILE $ON
	sleep 1

	ledState $DEV_FILE $OFF
	sleep 1
done

