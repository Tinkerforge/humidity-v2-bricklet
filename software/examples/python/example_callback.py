#!/usr/bin/env python
# -*- coding: utf-8 -*-

HOST = "localhost"
PORT = 4223
UID = "XYZ" # Change XYZ to the UID of your Humidity Bricklet 2.0

from tinkerforge.ip_connection import IPConnection
from tinkerforge.bricklet_humidity_v2 import BrickletHumidityV2

# Callback function for humidity callback
def cb_humidity(humidity):
    print("Humidity: " + str(humidity/100.0) + " %RH")

if __name__ == "__main__":
    ipcon = IPConnection() # Create IP connection
    h = BrickletHumidityV2(UID, ipcon) # Create device object

    ipcon.connect(HOST, PORT) # Connect to brickd
    # Don't use device before ipcon is connected

    # Register humidity callback to function cb_humidity
    h.register_callback(h.CALLBACK_HUMIDITY, cb_humidity)

    # Set period for humidity callback to 1s (1000ms) without a threshold
    h.set_humidity_callback_configuration(1000, False, "x", 0, 0)

    raw_input("Press key to exit\n") # Use input() in Python 3
    ipcon.disconnect()
