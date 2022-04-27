// This example is not self-contained.
// It requires usage of the example driver specific to your platform.
// See the HAL documentation.

#include "src/bindings/hal_common.h"
#include "src/bindings/bricklet_humidity_v2.h"

void check(int rc, const char *msg);
void example_setup(TF_HAL *hal);
void example_loop(TF_HAL *hal);

static TF_HumidityV2 h;

void example_setup(TF_HAL *hal) {
	// Create device object
	check(tf_humidity_v2_create(&h, NULL, hal), "create device object");

	// Get current humidity
	uint16_t humidity;
	check(tf_humidity_v2_get_humidity(&h, &humidity), "get humidity");

	tf_hal_printf("Humidity: %d 1/%d %%RH\n", humidity, 100);
}

void example_loop(TF_HAL *hal) {
	// Poll for callbacks
	tf_hal_callback_tick(hal, 0);
}
