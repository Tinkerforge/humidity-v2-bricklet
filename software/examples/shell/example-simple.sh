#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change XYZ to the UID of your Humidity Bricklet 2.0

# Get current humidity (unit is %RH/100)
tinkerforge call humidity-v2-bricklet $uid get-humidity
