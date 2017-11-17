function matlab_example_simple()
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletHumidityV2;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'XYZ'; % Change XYZ to the UID of your Humidity Bricklet 2.0

    ipcon = IPConnection(); % Create IP connection
    h = handle(BrickletHumidityV2(UID, ipcon), 'CallbackProperties'); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Get current humidity (unit is %RH/100)
    humidity = h.getHumidity();
    fprintf('Humidity: %g %%RH\n', humidity/100.0);

    input('Press key to exit\n', 's');
    ipcon.disconnect();
end
