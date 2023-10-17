#!/bin/bash

# base path for all gpio
BASE_GPIO_PATH=/sys/class/gpio

# names of states
ON="1"
OFF="0"

# LED pin
LED=7

# Utility function to export a pin if not already exported
exportPin()
{
  if [ ! -e $BASE_GPIO_PATH/gpio$1 ]; then
    echo "$1" > $BASE_GPIO_PATH/export
  fi
}

# Utility function to set a pin as an output
setOutput()
{
  echo "out" > $BASE_GPIO_PATH/gpio$1/direction
}

# Utility function to change state of a light
setLightState()
{
  echo $2 > $BASE_GPIO_PATH/gpio$1/value
}

# set up
exportPin $LED
setOutput $LED

while [ 1 ]
do
	setLightState $LED $ON
	sleep 1
	setLightState $LED $OFF
	sleep 1
done
