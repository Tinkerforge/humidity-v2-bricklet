package main

import (
	"fmt"
	"github.com/Tinkerforge/go-api-bindings/humidity_v2_bricklet"
	"github.com/Tinkerforge/go-api-bindings/ipconnection"
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

	// Get current humidity.
	humidity, _ := h.GetHumidity()
	fmt.Printf("Humidity: %f %RH\n", float64(humidity)/100.0)

	fmt.Print("Press enter to exit.")
	fmt.Scanln()

}
