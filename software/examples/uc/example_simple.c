// This example is not self-contained.
// It requres usage of the example driver specific to your platform.
// See the HAL documentation.

#include "bindings/hal_common.h"
#include "bindings/bricklet_humidity_v2.h"

#define UID "XYZ" // Change XYZ to the UID of your Humidity Bricklet 2.0

void check(int rc, const char* msg);

void example_setup(TF_HalContext *hal);
void example_loop(TF_HalContext *hal);


static TF_HumidityV2 h;

void example_setup(TF_HalContext *hal) {
	// Create device object
	check(tf_humidity_v2_create(&h, UID, hal), "create device object");

	// Get current humidity
	uint16_t humidity;
	check(tf_humidity_v2_get_humidity(&h, &humidity), "get humidity");

	tf_hal_printf("Humidity: %d 1/%d %%RH\n", humidity, 100.0);
}

void example_loop(TF_HalContext *hal) {
	// Poll for callbacks
	tf_hal_callback_tick(hal, 0);
}
