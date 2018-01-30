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

# Register humidity callback
h.register_callback(BrickletHumidityV2::CALLBACK_HUMIDITY) do |humidity|
  puts "Humidity: #{humidity/100.0} %RH"
  puts 'Recommended humiditiy for human comfort is 30 to 60 %RH.'
end

# Configure threshold for humidity "outside of 30 to 60 %RH"
# with a debounce period of 10s (10000ms)
h.set_humidity_callback_configuration 10000, false, 'o', 30*100, 60*100

puts 'Press key to exit'
$stdin.gets
ipcon.disconnect
