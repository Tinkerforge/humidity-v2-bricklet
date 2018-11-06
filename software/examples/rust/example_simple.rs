use std::{error::Error, io};

use tinkerforge::{humidity_v2_bricklet::*, ipconnection::IpConnection};

const HOST: &str = "127.0.0.1";
const PORT: u16 = 4223;
const UID: &str = "XYZ"; // Change XYZ to the UID of your Humidity Bricklet 2.0

fn main() -> Result<(), Box<dyn Error>> {
    let ipcon = IpConnection::new(); // Create IP connection
    let humidity_v2_bricklet = HumidityV2Bricklet::new(UID, &ipcon); // Create device object

    ipcon.connect(HOST, PORT).recv()??; // Connect to brickd
                                        // Don't use device before ipcon is connected

    // Get current humidity
    let humidity = humidity_v2_bricklet.get_humidity().recv()?;
    println!("Humidity: {}{}", humidity as f32 / 100.0, " %RH");

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
