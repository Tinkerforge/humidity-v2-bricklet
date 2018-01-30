#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change XYZ to the UID of your Humidity Bricklet 2.0

# Handle incoming humidity callbacks
tinkerforge dispatch humidity-v2-bricklet $uid humidity\
 --execute "echo Humidity: {humidity}/100 %RH. Recommended humiditiy for human comfort is 30 to 60 %RH." &

# Configure threshold for humidity "outside of 30 to 60 %RH"
# with a debounce period of 10s (10000ms)
tinkerforge call humidity-v2-bricklet $uid set-humidity-callback-configuration 10000 false threshold-option-outside 3000 6000

echo "Press key to exit"; read dummy

kill -- -$$ # Stop callback dispatch in background
