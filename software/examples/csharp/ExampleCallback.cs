using System;
using Tinkerforge;

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "XYZ"; // Change XYZ to the UID of your Humidity Bricklet 2.0

	// Callback function for humidity callback
	static void HumidityCB(BrickletHumidityV2 sender, int humidity)
	{
		Console.WriteLine("Humidity: " + humidity/100.0 + " %RH");
	}

	static void Main()
	{
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletHumidityV2 h = new BrickletHumidityV2(UID, ipcon); // Create device object

		ipcon.Connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Register humidity callback to function HumidityCB
		h.HumidityCallback += HumidityCB;

		// Set period for humidity callback to 1s (1000ms) without a threshold
		h.SetHumidityCallbackConfiguration(1000, false, 'x', 0, 0);

		Console.WriteLine("Press enter to exit");
		Console.ReadLine();
		ipcon.Disconnect();
	}
}
