#!/usr/bin/env ruby
# -*- ruby encoding: utf-8 -*-

require 'tinkerforge/ip_connection'
require 'tinkerforge/bricklet_humidity_v2'

include Tinkerforge

HOST = 'localhost'
PORT = 4223
UID = 'XYZ' # Change XYZ to the UID of your Humidity Bricklet 2.0

ipcon = IPConnection.new # Create IP connection
h = BrickletHumidityV2.new UID, ipcon # Create device object

ipcon.connect HOST, PORT # Connect to brickd
# Don't use device before ipcon is connected

# Get current humidity (unit is %RH/100)
humidity = h.get_humidity
puts "Humidity: #{humidity/100.0} %RH"

puts 'Press key to exit'
$stdin.gets
ipcon.disconnect
