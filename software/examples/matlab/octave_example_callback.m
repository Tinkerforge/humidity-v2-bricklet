function octave_example_callback()
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

    % Set period for humidity callback to 1s (1000ms) without a threshold
    h.setHumidityCallbackConfiguration(1000, false, "x", 0, 0);

    input("Press key to exit\n", "s");
    ipcon.disconnect();
end

% Callback function for humidity callback (parameter has unit %RH/100)
function cb_humidity(e)
    fprintf("Humidity: %g %%RH\n", e.humidity/100.0);
end
