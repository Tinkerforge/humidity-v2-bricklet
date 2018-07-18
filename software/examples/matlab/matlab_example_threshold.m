function matlab_example_threshold()
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletHumidityV2;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'XYZ'; % Change XYZ to the UID of your Humidity Bricklet 2.0

    ipcon = IPConnection(); % Create IP connection
    h = handle(BrickletHumidityV2(UID, ipcon), 'CallbackProperties'); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Register humidity callback to function cb_humidity
    set(h, 'HumidityCallback', @(h, e) cb_humidity(e));

    % Configure threshold for humidity "outside of 30 to 60 %RH"
    % with a debounce period of 10s (10000ms)
    h.setHumidityCallbackConfiguration(10000, false, 'o', 30*100, 60*100);

    input('Press key to exit\n', 's');
    ipcon.disconnect();
end

% Callback function for humidity callback
function cb_humidity(e)
    fprintf('Humidity: %g %%RH\n', e.humidity/100.0);
    fprintf('Recommended humidity for human comfort is 30 to 60 %%RH.\n');
end
