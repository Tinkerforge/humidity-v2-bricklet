package main

import (
	"fmt"
	"tinkerforge/humidity_v2_bricklet"
	"tinkerforge/ipconnection"
)

const ADDR string = "localhost:4223"
const UID string = "XYZ" // Change XYZ to the UID of your Humidity Bricklet 2.0.

func main() {
	ipcon := ipconnection.New()
	defer ipcon.Close()
	h, _ := humidity_v2_bricklet.New(UID, &ipcon) // Create device object.

	ipcon.Connect(ADDR) // Connect to brickd.
	defer ipcon.Disconnect()
	// Don't use device before ipcon is connected.

	h.RegisterHumidityCallback(func(humidity uint16) {
		fmt.Printf("Humidity: %f %RH\n", float64(humidity)/100.0)
		fmt.Println("Recommended humidity for human comfort is 30 to 60 %RH.")
	})

	// Configure threshold for humidity "outside of 30 to 60 %RH"
	// with a debounce period of 10s (10000ms).
	h.SetHumidityCallbackConfiguration(10000, false, 'o', 30*100, 60*100)

	fmt.Print("Press enter to exit.")
	fmt.Scanln()

}
