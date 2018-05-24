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
    print "Recommended humiditiy for human comfort is 30 to 60 %RH.\n";
}

my $ipcon = Tinkerforge::IPConnection->new(); # Create IP connection
my $h = Tinkerforge::BrickletHumidityV2->new(&UID, $ipcon); # Create device object

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Register humidity callback to subroutine cb_humidity
$h->register_callback($h->CALLBACK_HUMIDITY, 'cb_humidity');

# Configure threshold for humidity "outside of 30 to 60 %RH"
# with a debounce period of 10s (10000ms)
$h->set_humidity_callback_configuration(10000, 0, 'o', 30*100, 60*100);

print "Press key to exit\n";
<STDIN>;
$ipcon->disconnect();
