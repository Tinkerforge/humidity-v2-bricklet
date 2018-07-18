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
        // Configure threshold for humidity "outside of 30 to 60 %RH"
        // with a debounce period of 10s (10000ms)
        h.setHumidityCallbackConfiguration(10000, false, 'o', 30*100, 60*100);
    }
);

// Register humidity callback
h.on(Tinkerforge.BrickletHumidityV2.CALLBACK_HUMIDITY,
    // Callback function for humidity callback
    function (humidity) {
        console.log('Humidity: ' + humidity/100.0 + ' %RH');
        console.log('Recommended humidity for human comfort is 30 to 60 %RH.');
    }
);

console.log('Press key to exit');
process.stdin.on('data',
    function (data) {
        ipcon.disconnect();
        process.exit(0);
    }
);
