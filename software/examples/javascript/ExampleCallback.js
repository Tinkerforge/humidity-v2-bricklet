var Tinkerforge = require('tinkerforge');

var HOST = 'localhost';
var PORT = 4223;
var UID = 'XYZ'; // Change XYZ to the UID of your Humidity Bricklet 2.0

var ipcon = new Tinkerforge.IPConnection(); // Create IP connection
var h = new Tinkerforge.BrickletHumidityV2(UID, ipcon); // Create device object

ipcon.connect(HOST, PORT,
    function (error) {
        console.log('Error: ' + error);
    }
); // Connect to brickd
// Don't use device before ipcon is connected

ipcon.on(Tinkerforge.IPConnection.CALLBACK_CONNECTED,
    function (connectReason) {
        // Set period for humidity callback to 1s (1000ms) without a threshold
        h.setHumidityCallbackConfiguration(1000, false, 'x', 0, 0);
    }
);

// Register humidity callback
h.on(Tinkerforge.BrickletHumidityV2.CALLBACK_HUMIDITY,
    // Callback function for humidity callback
    function (humidity) {
        console.log('Humidity: ' + humidity/100.0 + ' %RH');
    }
);

console.log('Press key to exit');
process.stdin.on('data',
    function (data) {
        ipcon.disconnect();
        process.exit(0);
    }
);
