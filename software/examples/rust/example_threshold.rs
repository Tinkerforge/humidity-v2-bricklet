use std::{error::Error, io, thread};
use tinkerforge::{humidity_v2_bricklet::*, ipconnection::IpConnection};

const HOST: &str = "127.0.0.1";
const PORT: u16 = 4223;
const UID: &str = "XYZ"; // Change XYZ to the UID of your Humidity Bricklet 2.0

fn main() -> Result<(), Box<dyn Error>> {
    let ipcon = IpConnection::new(); // Create IP connection
    let humidity_v2_bricklet = HumidityV2Bricklet::new(UID, &ipcon); // Create device object

    ipcon.connect(HOST, PORT).recv()??; // Connect to brickd
                                        // Don't use device before ipcon is connected

    //Create listener for humidity events.
    let humidity_listener = humidity_v2_bricklet.get_humidity_receiver();
    // Spawn thread to handle received events. This thread ends when the humidity_v2_bricklet
    // is dropped, so there is no need for manual cleanup.
    thread::spawn(move || {
        for event in humidity_listener {
            println!("Humidity: {}{}", event as f32 / 100.0, " %RH");
            println!("Recommended humidity for human comfort is 30 to 60 %RH.");
        }
    });

    // Configure threshold for humidity "outside of 30 to 60 %RH"
    // with a debounce period of 10s (10000ms)
    humidity_v2_bricklet.set_humidity_callback_configuration(10000, false, 'o', 30 * 100, 60 * 100);

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
