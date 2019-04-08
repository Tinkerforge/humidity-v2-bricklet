/* humidity-v2-bricklet
 * Copyright (C) 2017-2018 Olaf Lüke <olaf@tinkerforge.com>
 *
 * communication.c: TFP protocol message handling
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "communication.h"

#include "bricklib2/utility/communication_callback.h"
#include "bricklib2/protocols/tfp/tfp.h"
#define CALLBACK_VALUE_TYPE CALLBACK_VALUE_TYPE_UINT16
#include "bricklib2/utility/callback_value.h" // for humidity
#undef CALLBACK_VALUE_TYPE
#define CALLBACK_VALUE_TYPE CALLBACK_VALUE_TYPE_INT16
#include "bricklib2/utility/callback_value.h" // for temperature
#undef CALLBACK_VALUE_TYPE
#include "bricklib2/utility/moving_average.h"
#include "hdc1080.h"

extern HDC1080 hdc1080;

CallbackValue_uint16_t callback_value_humidity;
CallbackValue_int16_t callback_value_temperature;

BootloaderHandleMessageResponse handle_message(const void *message, void *response) {
	switch(tfp_get_fid_from_message(message)) {
		case FID_GET_HUMIDITY: return get_callback_value_uint16_t(message, response, &callback_value_humidity);
		case FID_SET_HUMIDITY_CALLBACK_CONFIGURATION: return set_callback_value_callback_configuration_uint16_t(message, &callback_value_humidity);
		case FID_GET_HUMIDITY_CALLBACK_CONFIGURATION: return get_callback_value_callback_configuration_uint16_t(message, response, &callback_value_humidity);
		case FID_GET_TEMPERATURE: return get_callback_value_int16_t(message, response, &callback_value_temperature);
		case FID_SET_TEMPERATURE_CALLBACK_CONFIGURATION: return set_callback_value_callback_configuration_int16_t(message, &callback_value_temperature);
		case FID_GET_TEMPERATURE_CALLBACK_CONFIGURATION: return get_callback_value_callback_configuration_int16_t(message, response, &callback_value_temperature);
		case FID_SET_HEATER_CONFIGURATION: return set_heater_configuration(message);
		case FID_GET_HEATER_CONFIGURATION: return get_heater_configuration(message, response);
		case FID_SET_MOVING_AVERAGE_CONFIGURATION: return set_moving_average_configuration(message);
		case FID_GET_MOVING_AVERAGE_CONFIGURATION: return get_moving_average_configuration(message, response);
		case FID_SET_SAMPLES_PER_SECOND: return set_samples_per_second(message);
		case FID_GET_SAMPLES_PER_SECOND: return get_samples_per_second(message, response);
		default: return HANDLE_MESSAGE_RESPONSE_NOT_SUPPORTED;
	}
}

BootloaderHandleMessageResponse set_heater_configuration(const SetHeaterConfiguration *data) {
	if(data->heater_config > HUMIDITY_V2_HEATER_CONFIG_ENABLED) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	hdc1080_set_heater_config(data->heater_config);
	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_heater_configuration(const GetHeaterConfiguration *data, GetHeaterConfiguration_Response *response) {
	response->header.length = sizeof(GetHeaterConfiguration_Response);
	response->heater_config = hdc1080_get_heater_config();

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_moving_average_configuration(const SetMovingAverageConfiguration *data) {
	if((data->moving_average_length_humidity    > MOVING_AVERAGE_MAX_LENGTH) ||
	   (data->moving_average_length_temperature > MOVING_AVERAGE_MAX_LENGTH) ||
	   (data->moving_average_length_humidity    < 1) ||
	   (data->moving_average_length_temperature < 1)) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	moving_average_new_length(&hdc1080.moving_average_humidity,    data->moving_average_length_humidity);
	moving_average_new_length(&hdc1080.moving_average_temperature, data->moving_average_length_temperature);

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_moving_average_configuration(const GetMovingAverageConfiguration *data, GetMovingAverageConfiguration_Response *response) {
	response->header.length                     = sizeof(GetMovingAverageConfiguration_Response);
	response->moving_average_length_humidity    = hdc1080.moving_average_humidity.length;
	response->moving_average_length_temperature = hdc1080.moving_average_temperature.length;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_samples_per_second(const SetSamplesPerSecond *data) {
	if(data->sps > HUMIDITY_V2_SPS_01) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	hdc1080.sps = data->sps;

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_samples_per_second(const GetSamplesPerSecond *data, GetSamplesPerSecond_Response *response) {
	response->header.length = sizeof(GetSamplesPerSecond_Response);
	response->sps           = hdc1080.sps;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

bool handle_humidity_callback(void) {
	return handle_callback_value_callback_uint16_t(&callback_value_humidity, FID_CALLBACK_HUMIDITY);
}

bool handle_temperature_callback(void) {
	return handle_callback_value_callback_int16_t(&callback_value_temperature, FID_CALLBACK_TEMPERATURE);
}

void communication_tick(void) {
	communication_callback_tick();
}

void communication_init(void) {
	callback_value_init_uint16_t(&callback_value_humidity, hdc1080_get_humidity);
	callback_value_init_int16_t(&callback_value_temperature, hdc1080_get_temperature);
	communication_callback_init();
}
