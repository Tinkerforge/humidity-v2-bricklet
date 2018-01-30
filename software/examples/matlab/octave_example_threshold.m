function octave_example_threshold()
    more off;

    HOST = "localhost";
    PORT = 4223;
    UID = "XYZ"; % Change XYZ to the UID of your Humidity Bricklet 2.0

    ipcon = javaObject("com.tinkerforge.IPConnection"); % Create IP connection
    h = javaObject("com.tinkerforge.BrickletHumidityV2", UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Register humidity callback to function cb_humidity
    h.addHumidityCallback(@cb_humidity);

    % Configure threshold for humidity "outside of 30 to 60 %RH"
    % with a debounce period of 10s (10000ms)
    h.setHumidityCallbackConfiguration(10000, false, "o", 30*100, 60*100);

    input("Press key to exit\n", "s");
    ipcon.disconnect();
end

% Callback function for humidity callback
function cb_humidity(e)
    fprintf("Humidity: %g %%RH\n", e.humidity/100.0);
    fprintf("Recommended humiditiy for human comfort is 30 to 60 %%RH.\n");
end
