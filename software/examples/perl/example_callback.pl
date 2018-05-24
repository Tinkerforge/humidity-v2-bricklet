#!/usr/bin/perl

use strict;
use Tinkerforge::IPConnection;
use Tinkerforge::BrickletHumidityV2;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'XYZ'; # Change XYZ to the UID of your Humidity Bricklet 2.0

# Callback subroutine for humidity callback
sub cb_humidity
{
    my ($humidity) = @_;

    print "Humidity: " . $humidity/100.0 . " %RH\n";
}

my $ipcon = Tinkerforge::IPConnection->new(); # Create IP connection
my $h = Tinkerforge::BrickletHumidityV2->new(&UID, $ipcon); # Create device object

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Register humidity callback to subroutine cb_humidity
$h->register_callback($h->CALLBACK_HUMIDITY, 'cb_humidity');

# Set period for humidity callback to 1s (1000ms) without a threshold
$h->set_humidity_callback_configuration(1000, 0, 'x', 0, 0);

print "Press key to exit\n";
<STDIN>;
$ipcon->disconnect();
