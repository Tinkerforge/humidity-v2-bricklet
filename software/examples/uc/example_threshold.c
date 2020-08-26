#include "bindings/hal_common.h"
#include "bindings/bricklet_humidity_v2.h"

#define UID "XYZ" // Change XYZ to the UID of your Humidity Bricklet 2.0

void check(int rc, const char* msg);

// Callback function for humidity callback
void humidity_handler(TF_HumidityV2 *device, uint16_t humidity, void *user_data) {
	(void)device; (void)user_data; // avoid unused parameter warning

	tf_hal_printf("Humidity: %d 1/%d %%RH\n", humidity, 100.0);
	tf_hal_printf("Recommended humidity for human comfort is 30 to 60 %%RH.\n");
}

TF_HumidityV2 h;

void example_setup(TF_HalContext *hal) {
	// Create device object
	check(tf_humidity_v2_create(&h, UID, hal), "create device object");

	// Register humidity callback to function humidity_handler
	tf_humidity_v2_register_humidity_callback(&h,
	                                          humidity_handler,
	                                          NULL);

	// Configure threshold for humidity "outside of 30 to 60 %RH"
	// with a debounce period of 10s (10000ms)
	tf_humidity_v2_set_humidity_callback_configuration(&h, 10000, false, 'o', 30*100, 60*100);
}

void example_loop(TF_HalContext *hal) {
	// Poll for callbacks
	tf_hal_callback_tick(hal, 0);
}
