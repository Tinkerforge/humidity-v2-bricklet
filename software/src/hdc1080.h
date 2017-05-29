/* humidity-v2-bricklet
 * Copyright (C) 2017 Olaf Lüke <olaf@tinkerforge.com>
 *
 * hdc1080.h: Driver for HDC1080 humidity sensor
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

#ifndef HDC1080_H
#define HDC1080_H

#include <stdint.h>
#include <stdbool.h>


#include "bricklib2/utility/moving_average.h"

#define HDC1080_REG_TEMPERATURE     0x00
#define HDC1080_REG_HUMIDITY        0x01
#define HDC1080_REG_CONFIGURATION   0x02
#define HDC1080_REG_SERIAL_ID_LOW   0xFB
#define HDC1080_REG_SERIAL_ID_MID   0xFC
#define HDC1080_REG_SERIAL_ID_HIGH  0xFD
#define HDC1080_REG_MANUFACTURER_ID 0xFE
#define HDC1080_REG_DEVICE_ID       0xFF

#define HDC1080_RESOLUTION_T_14BIT  0b00
#define HDC1080_RESOLUTION_T_11BIT  0b01

#define HDC1080_RESOLUTION_H_14BIT  0b00
#define HDC1080_RESOLUTION_H_11BIT  0b01
#define HDC1080_RESOLUTION_H_8BIT   0b10

#define HDC1080_MANUFACTURER_ID     0x5449
#define HDC1080_DEVICE_ID           0x1050

#define HDC1080_POWERUP_TIME        15 // 15ms until we can read humidity/temperature
#define HDC1080_CONVERSION_TIME     7  // 7ms for conversion, see page 5 "Electrical Characteristics"

typedef union {
  struct {
    uint16_t reserved2:8;
    uint16_t hres:2;
    uint16_t tres:1;
    uint16_t btst:1;
    uint16_t mode:1;
    uint16_t heat:1;
    uint16_t reserved1:1;
    uint16_t rst:1;
  } bit;
  uint16_t reg;
} __attribute__ ((__packed__)) HDC1080RegConfiguration;

typedef struct {
	uint16_t serial_id[3];
	uint16_t manufacturer_id;
	uint16_t device_id;
} HDC1080IDs;

typedef struct {
	uint16_t temperature;
	uint16_t humidity;
} HDC1080Value;

typedef enum {
	HDC1080_STATE_IDLE,
	HDC1080_STATE_START_READING,
	HDC1080_STATE_START_READING_MEASUREMENT,
	HDC1080_STATE_READING,
	HDC1080_STATE_START_WRITING,
	HDC1080_STATE_WRITING,
} HDC1080State;

typedef struct {
	HDC1080Value value;
	HDC1080RegConfiguration configuration;
	HDC1080IDs ids;

	HDC1080State state;
	uint32_t last_state_change;
	uint8_t current_read_reg;
	uint8_t current_read_length;
	uint8_t current_write_reg;
	uint16_t current_write_data;
	uint16_t current_write_length;

	bool write_config;
	uint8_t update_ids; // start update by setting to 1
	uint8_t startup;

	uint32_t last_acquisition_trigger;
	uint32_t powerup_wait_time;

	MovingAverage moving_average_temperature;
	MovingAverage moving_average_humidity;
} HDC1080;

void hdc1080_init(HDC1080 *hdc1080);
void hdc1080_tick(HDC1080 *hdc1080);

void hdc1080_set_heater_config(uint8_t heater_config);
uint8_t hdc1080_get_heater_config(void);
int16_t hdc1080_get_humidity(void);
int16_t hdc1080_get_temperature(void);

#endif
