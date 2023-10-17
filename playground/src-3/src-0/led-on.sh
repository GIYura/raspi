#!/bin/bash

FILE=$(pwd)/.config

if [ ! -f $FILE ]; then
    echo ".config file not found"
    exit
fi

source $FILE

if [ ! -e $LED_PATH/gpio$LED_PIN ]; then
    echo "Can't open file"
    exit
fi

echo 1 > $LED_PATH/gpio$LED_PIN/value
echo "LED ON"
