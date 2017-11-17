#!/usr/bin/perl

use Tinkerforge::IPConnection;
use Tinkerforge::BrickletHumidityV2;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'XYZ'; # Change XYZ to the UID of your Humidity Bricklet 2.0

my $ipcon = Tinkerforge::IPConnection->new(); # Create IP connection
my $h = Tinkerforge::BrickletHumidityV2->new(&UID, $ipcon); # Create device object

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Get current humidity (unit is %RH/100)
my $humidity = $h->get_humidity();
print "Humidity: " . $humidity/100.0 . " %RH\n";

print "Press key to exit\n";
<STDIN>;
$ipcon->disconnect();
