#!/usr/bin/python3

import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
GPIO.setup(21, GPIO.OUT)
while True:
	print ("LED ON")

	GPIO.output(21, GPIO.HIGH)
	time.sleep(1)

	print ("LED OFF")

	GPIO.output(21, GPIO.LOW)
	time.sleep(1)
