<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletHumidityV2.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletHumidityV2;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change XYZ to the UID of your Humidity Bricklet 2.0

// Callback function for humidity callback (parameter has unit %RH/100)
function cb_humidity($humidity)
{
    echo "Humidity: " . $humidity/100.0 . " %RH\n";
    echo "Recommended humiditiy for human comfort is 30 to 60 %RH.\n";
}

$ipcon = new IPConnection(); // Create IP connection
$h = new BrickletHumidityV2(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Register humidity callback to function cb_humidity
$h->registerCallback(BrickletHumidityV2::CALLBACK_HUMIDITY, 'cb_humidity');

// Configure threshold for humidity "outside of 30 to 60 %RH" (unit is %RH/100)
// with a debounce period of 10s (10000ms)
$h->setHumidityCallbackConfiguration(10000, FALSE, 'o', 30*100, 60*100);

echo "Press ctrl+c to exit\n";
$ipcon->dispatchCallbacks(-1); // Dispatch callbacks forever

?>
