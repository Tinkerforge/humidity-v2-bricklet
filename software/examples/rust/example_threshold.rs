use std::{io, error::Error};
use std::thread;
use tinkerforge::{ip_connection::IpConnection, 
                  humidity_v2_bricklet::*};


const HOST: &str = "localhost";
const PORT: u16 = 4223;
const UID: &str = "XYZ"; // Change XYZ to the UID of your Humidity Bricklet 2.0.

fn main() -> Result<(), Box<dyn Error>> {
    let ipcon = IpConnection::new(); // Create IP connection.
    let h = HumidityV2Bricklet::new(UID, &ipcon); // Create device object.

    ipcon.connect((HOST, PORT)).recv()??; // Connect to brickd.
    // Don't use device before ipcon is connected.

     let humidity_receiver = h.get_humidity_callback_receiver();

        // Spawn thread to handle received callback messages. 
        // This thread ends when the `h` object
        // is dropped, so there is no need for manual cleanup.
        thread::spawn(move || {
            for humidity in humidity_receiver {           
                		println!("Humidity: {} %RH", humidity as f32 /100.0);
		println!("Recommended humidity for human comfort is 30 to 60 %RH.");
            }
        });

		// Configure threshold for humidity "outside of 30 to 60 %RH"
		// with a debounce period of 10s (10000ms).
		h.set_humidity_callback_configuration(10000, false, 'o', 30*100, 60*100);

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
