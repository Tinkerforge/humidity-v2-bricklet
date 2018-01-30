import com.tinkerforge.IPConnection;
import com.tinkerforge.BrickletHumidityV2;

public class ExampleThreshold {
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
				System.out.println("Recommended humiditiy for human comfort is 30 to 60 %RH.");
			}
		});

		// Configure threshold for humidity "outside of 30 to 60 %RH"
		// with a debounce period of 10s (10000ms)
		h.setHumidityCallbackConfiguration(10000, false, 'o', 30*100, 60*100);

		System.out.println("Press key to exit"); System.in.read();
		ipcon.disconnect();
	}
}
