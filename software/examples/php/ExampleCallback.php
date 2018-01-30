<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletHumidityV2.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletHumidityV2;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change XYZ to the UID of your Humidity Bricklet 2.0

// Callback function for humidity callback
function cb_humidity($humidity)
{
    echo "Humidity: " . $humidity/100.0 . " %RH\n";
}

$ipcon = new IPConnection(); // Create IP connection
$h = new BrickletHumidityV2(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Register humidity callback to function cb_humidity
$h->registerCallback(BrickletHumidityV2::CALLBACK_HUMIDITY, 'cb_humidity');

// Set period for humidity callback to 1s (1000ms) without a threshold
$h->setHumidityCallbackConfiguration(1000, FALSE, 'x', 0, 0);

echo "Press ctrl+c to exit\n";
$ipcon->dispatchCallbacks(-1); // Dispatch callbacks forever

?>
