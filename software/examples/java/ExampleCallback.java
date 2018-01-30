import com.tinkerforge.IPConnection;
import com.tinkerforge.BrickletHumidityV2;

public class ExampleCallback {
	private static final String HOST = "localhost";
	private static final int PORT = 4223;

	// Change XYZ to the UID of your Humidity Bricklet 2.0
	private static final String UID = "XYZ";

	// Note: To make the example code cleaner we do not handle exceptions. Exceptions
	//       you might normally want to catch are described in the documentation
	public static void main(String args[]) throws Exception {
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletHumidityV2 h = new BrickletHumidityV2(UID, ipcon); // Create device object

		ipcon.connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Add humidity listener
		h.addHumidityListener(new BrickletHumidityV2.HumidityListener() {
			public void humidity(int humidity) {
				System.out.println("Humidity: " + humidity/100.0 + " %RH");
			}
		});

		// Set period for humidity callback to 1s (1000ms) without a threshold
		h.setHumidityCallbackConfiguration(1000, false, 'x', 0, 0);

		System.out.println("Press key to exit"); System.in.read();
		ipcon.disconnect();
	}
}
