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
		Console.WriteLine("Recommended humiditiy for human comfort is 30 to 60 %RH.");
	}

	static void Main()
	{
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletHumidityV2 h = new BrickletHumidityV2(UID, ipcon); // Create device object

		ipcon.Connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Register humidity callback to function HumidityCB
		h.HumidityCallback += HumidityCB;

		// Configure threshold for humidity "outside of 30 to 60 %RH"
		// with a debounce period of 10s (10000ms)
		h.SetHumidityCallbackConfiguration(10000, false, 'o', 30*100, 60*100);

		Console.WriteLine("Press enter to exit");
		Console.ReadLine();
		ipcon.Disconnect();
	}
}
