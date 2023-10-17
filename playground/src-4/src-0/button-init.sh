#!/bin/bash

FILE=$(pwd)/.config

if [ ! -f $FILE ]; then
    echo ".config file not found"
    exit
fi

source $FILE

if [ ! -e $BUTTON_PATH/gpio$BUTTION_PIN ]; then
    echo $BUTTON_PIN > $BUTTON_PATH/export
    echo "in" > $BUTTON_PATH/gpio$BUTTON_PIN/direction
    echo $BUTTON_EDGE > $BUTTON_PATH/gpio$BUTTON_PIN/edge
    echo "Button init"
    exit
fi
    echo "Button init failed"
    exit
