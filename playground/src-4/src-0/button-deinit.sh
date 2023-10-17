#!/bin/bash

FILE=$(pwd)/.config

if [ ! -f $FILE ]; then
    echo ".config file not found"
    exit
fi

source $FILE

if [ -e $BUTTON_PATH/gpio$BUTTON_PIN ]; then
    echo $BUTTON_PIN > $BUTTON_PATH/unexport
    echo "Button deinit"
    exit
fi
    echo "Button deinit failed"
    exit
