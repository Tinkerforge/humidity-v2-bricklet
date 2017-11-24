#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change XYZ to the UID of your Humidity Bricklet 2.0

# Handle incoming humidity callbacks (parameter has unit %RH/100)
tinkerforge dispatch humidity-v2-bricklet $uid humidity &

# Set period for humidity callback to 1s (1000ms) without a threshold
tinkerforge call humidity-v2-bricklet $uid set-humidity-callback-configuration 1000 false threshold-option-off 0 0

echo "Press key to exit"; read dummy

kill -- -$$ # Stop callback dispatch in background
