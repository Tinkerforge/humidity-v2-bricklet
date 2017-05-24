/* humidity-v2-bricklet
 * Copyright (C) 2017 Olaf Lüke <olaf@tinkerforge.com>
 *
 * hdc1080.c: Driver for HDC1080 humidity sensor
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

#include "hdc1080.h"

#include <stdint.h>
#include <stdbool.h>

#include "configs/config_hdc1080.h"
#include "bricklib2/hal/system_timer/system_timer.h"

#include "xmc_i2c.h"
#include "xmc_gpio.h"
#include "xmc_usic.h"

typedef enum XMC_I2C_CH_TDF {
	XMC_I2C_CH_TDF_MASTER_SEND         = 0,
	XMC_I2C_CH_TDF_SLAVE_SEND          = 1 << 8,
	XMC_I2C_CH_TDF_MASTER_RECEIVE_ACK  = 2 << 8,
	XMC_I2C_CH_TDF_MASTER_RECEIVE_NACK = 3 << 8,
	XMC_I2C_CH_TDF_MASTER_START        = 4 << 8,
	XMC_I2C_CH_TDF_MASTER_RESTART      = 5 << 8,
	XMC_I2C_CH_TDF_MASTER_STOP         = 6 << 8
} XMC_I2C_CH_TDF_t;


// The i2c read/write functions here assume that the FIFO size is never exceeded.
void hdc1080_i2c_write(HDC1080 *hdc1080, const uint8_t reg, const uint16_t length, const uint16_t *data, const bool send_stop) {
	// I2C Master Start
	HDC1080_I2C->IN[0] = (HDC1080_I2C_ADDRESS << 1) | XMC_I2C_CH_TDF_MASTER_START;
	// I2C Send 8 bit register
	HDC1080_I2C->IN[0] = XMC_I2C_CH_TDF_MASTER_SEND | reg;

	// I2C Master send bytes to write
	bool high = true;
	for(uint16_t i = 0; i < length*2; i++) {
		if(high) {
			// Send high byte first
			HDC1080_I2C->IN[0] = XMC_I2C_CH_TDF_MASTER_SEND | (data[i/2] >> 8);
			high = false;
		} else {
			// and low byte second
			HDC1080_I2C->IN[0] = XMC_I2C_CH_TDF_MASTER_SEND | (data[i/2] & 0xFF);
			high = true;
		}
	}

	if(send_stop) {
		// I2C Master stop
		HDC1080_I2C->IN[0] = (uint32_t)XMC_I2C_CH_TDF_MASTER_STOP;
	}
}


void hdc1080_i2c_read(HDC1080 *hdc1080, const uint8_t reg, const uint32_t length) {
	hdc1080_i2c_write(hdc1080, reg, 0, NULL, false);

	// I2C Master restart
	HDC1080_I2C->IN[0] = XMC_I2C_CH_TDF_MASTER_RESTART | XMC_I2C_CH_CMD_READ | (HDC1080_I2C_ADDRESS << 1);

	// I2C Master send ACK/NACK
	if(length != 0) {
		for(uint16_t i = 0; i < length*2-1; i++) {
			HDC1080_I2C->IN[0] = XMC_I2C_CH_TDF_MASTER_RECEIVE_ACK;
		}

		HDC1080_I2C->IN[0] = XMC_I2C_CH_TDF_MASTER_RECEIVE_NACK;
	}

	// I2C Master stop
	HDC1080_I2C->IN[0] = XMC_I2C_CH_TDF_MASTER_STOP;
}

void hdc1080_i2c_read_measurement(HDC1080 *hdc1080, const uint32_t length) {
	// I2C Master start
	HDC1080_I2C->IN[0] = XMC_I2C_CH_TDF_MASTER_START | XMC_I2C_CH_CMD_READ | (HDC1080_I2C_ADDRESS << 1) ;

	// I2C Master send ACK/NACK
	if(length != 0) {
		for(uint16_t i = 0; i < length*2-1; i++) {
			HDC1080_I2C->IN[0] = XMC_I2C_CH_TDF_MASTER_RECEIVE_ACK;
		}

		HDC1080_I2C->IN[0] = XMC_I2C_CH_TDF_MASTER_RECEIVE_NACK;
	}

	// I2C Master stop
	HDC1080_I2C->IN[0] = XMC_I2C_CH_TDF_MASTER_STOP;
}

void hdc1080_change_state(HDC1080 *hdc1080, HDC1080State state) {
	hdc1080->state = state;
	hdc1080->last_state_change = system_timer_get_ms();
}

void hdc1080_init_i2c(HDC1080 *hdc1080) {
	// First de-configure everything, so we can also use
	// this function as i2c reset

	XMC_I2C_CH_Stop(HDC1080_I2C);

	const XMC_GPIO_CONFIG_t config_reset =  {
		.mode             = XMC_GPIO_MODE_INPUT_PULL_UP,
		.input_hysteresis = XMC_GPIO_INPUT_HYSTERESIS_LARGE,
	};

	XMC_GPIO_Init(HDC1080_SDA_PIN, &config_reset);
	XMC_GPIO_Init(HDC1080_SCL_PIN, &config_reset);

	XMC_USIC_CH_TXFIFO_Flush(HDC1080_I2C);
	XMC_USIC_CH_RXFIFO_Flush(HDC1080_I2C);

	WR_REG(HDC1080_I2C->FMR, USIC_CH_FMR_MTDV_Msk, USIC_CH_FMR_MTDV_Pos, 2);


	const XMC_I2C_CH_CONFIG_t master_channel_config = {
		.baudrate = HDC1080_I2C_BAUDRATE,
		.address  = 0
	};

	const XMC_GPIO_CONFIG_t sda_pin_config =  {
		.mode         = HDC1080_SDA_PIN_MODE,
		.output_level = XMC_GPIO_OUTPUT_LEVEL_HIGH,
	};

	const XMC_GPIO_CONFIG_t scl_pin_config = {
		.mode         = HDC1080_SCL_PIN_MODE,
		.output_level = XMC_GPIO_OUTPUT_LEVEL_HIGH,
	};

	XMC_I2C_CH_Init(HDC1080_I2C, &master_channel_config);

	XMC_USIC_CH_SetInputSource(HDC1080_I2C, HDC1080_SDA_INPUT, HDC1080_SDA_SOURCE);
	XMC_USIC_CH_SetInputSource(HDC1080_I2C, HDC1080_SCL_INPUT, HDC1080_SCL_SOURCE);

	XMC_USIC_CH_TXFIFO_Configure(HDC1080_I2C, 48, XMC_USIC_CH_FIFO_SIZE_16WORDS, 0);
	XMC_USIC_CH_RXFIFO_Configure(HDC1080_I2C, 32, XMC_USIC_CH_FIFO_SIZE_16WORDS, 0);

	XMC_I2C_CH_Start(HDC1080_I2C);

	XMC_GPIO_Init(HDC1080_SDA_PIN, &sda_pin_config);
	XMC_GPIO_Init(HDC1080_SCL_PIN, &scl_pin_config);
}

void hdc1080_handle_state(HDC1080 *hdc1080) {
	const uint32_t status = XMC_I2C_CH_GetStatusFlag(HDC1080_I2C);

	switch(hdc1080->state) {
		case HDC1080_STATE_IDLE: {
			break;
		}

		case HDC1080_STATE_START_READING: {
			hdc1080_i2c_read(hdc1080, hdc1080->current_read_reg, hdc1080->current_read_length);
			hdc1080_change_state(hdc1080, HDC1080_STATE_READING);
			break;
		}

		case HDC1080_STATE_START_READING_MEASUREMENT: {
			// Wait 2x conversion time, since we read temperature and humidity in one read
			if(system_timer_is_time_elapsed_ms(hdc1080->last_state_change, HDC1080_CONVERSION_TIME*2)) {
				hdc1080_i2c_read_measurement(hdc1080, hdc1080->current_read_length);
				hdc1080_change_state(hdc1080, HDC1080_STATE_READING);
			}
			break;
		}

		case HDC1080_STATE_READING: {
			if(status & (XMC_I2C_CH_STATUS_FLAG_NACK_RECEIVED |
			             XMC_I2C_CH_STATUS_FLAG_ARBITRATION_LOST |
			             XMC_I2C_CH_STATUS_FLAG_ERROR |
			             XMC_I2C_CH_STATUS_FLAG_WRONG_TDF_CODE_FOUND)) {
				XMC_I2C_CH_ClearStatusFlag(HDC1080_I2C, 0xFFFFFFFF);

				hdc1080_init_i2c(hdc1080);
				hdc1080_change_state(hdc1080, HDC1080_STATE_IDLE);
			} else if(XMC_USIC_CH_TXFIFO_IsEmpty(HDC1080_I2C)) {
				if(XMC_USIC_CH_RXFIFO_GetLevel(HDC1080_I2C) != hdc1080->current_read_length*2) {
					// Check if we read the expected number of bytes.
					// In case that we didn't read enough bytes yet we return and try again next tick.
					// In case that we read too much, we will eventually run in a timeout and a i2c reset will be triggered.
					break;
				}

				uint16_t *data = NULL;
				switch(hdc1080->current_read_reg) {
					case HDC1080_REG_TEMPERATURE:     data = &hdc1080->value.temperature;   break;
					case HDC1080_REG_HUMIDITY:        data = &hdc1080->value.humidity;      break;
					case HDC1080_REG_CONFIGURATION:   data = &hdc1080->configuration.reg;   break;
					case HDC1080_REG_SERIAL_ID_LOW:   data = &hdc1080->ids.serial_id[0];    break;
					case HDC1080_REG_SERIAL_ID_MID:   data = &hdc1080->ids.serial_id[1];    break;
					case HDC1080_REG_SERIAL_ID_HIGH:  data = &hdc1080->ids.serial_id[2];    break;
					case HDC1080_REG_MANUFACTURER_ID: data = &hdc1080->ids.manufacturer_id; break;
					case HDC1080_REG_DEVICE_ID:       data = &hdc1080->ids.device_id;       break;
				}

				if(data == NULL) {
					hdc1080_init_i2c(hdc1080);
					hdc1080_change_state(hdc1080, HDC1080_STATE_IDLE);
					break;
				}

				uint16_t data_index = 0;
				bool high = true;
				while(!XMC_USIC_CH_RXFIFO_IsEmpty(HDC1080_I2C)) {
					if(high) {
						// Get high byte first
						data[data_index/2] = XMC_I2C_CH_GetReceivedData(HDC1080_I2C) << 8;
						high = false;
					} else {
						// and low byte second
						data[data_index/2] |= XMC_I2C_CH_GetReceivedData(HDC1080_I2C);
						high = true;
					}
					data_index++;
				}

				hdc1080_change_state(hdc1080, HDC1080_STATE_IDLE);
			}
			break;
		}

		case HDC1080_STATE_START_WRITING: {
			hdc1080_i2c_write(hdc1080, hdc1080->current_write_reg, hdc1080->current_write_length, &hdc1080->current_write_data, true);
			hdc1080_change_state(hdc1080, HDC1080_STATE_WRITING);
			break;
		}

		case HDC1080_STATE_WRITING: {
			if(status & (XMC_I2C_CH_STATUS_FLAG_NACK_RECEIVED |
			             XMC_I2C_CH_STATUS_FLAG_ARBITRATION_LOST |
			             XMC_I2C_CH_STATUS_FLAG_ERROR |
			             XMC_I2C_CH_STATUS_FLAG_WRONG_TDF_CODE_FOUND)) {
				XMC_I2C_CH_ClearStatusFlag(HDC1080_I2C, 0xFFFFFFFF);

				hdc1080_init_i2c(hdc1080);
				hdc1080_change_state(hdc1080, HDC1080_STATE_IDLE);
			} else if(XMC_USIC_CH_TXFIFO_IsEmpty(HDC1080_I2C)) {
				if(hdc1080->current_write_reg == HDC1080_REG_TEMPERATURE) {
					// If we wrote to the temperature or humidity reg,
					// it always means that we want to read a measurement
					hdc1080_change_state(hdc1080, HDC1080_STATE_START_READING_MEASUREMENT);
				} else {
					hdc1080_change_state(hdc1080, HDC1080_STATE_IDLE);
				}
			}
			break;
		}
	}

	if(hdc1080->state != HDC1080_STATE_IDLE) {
		if(system_timer_is_time_elapsed_ms(hdc1080->last_state_change, 50)) {
			hdc1080_init_i2c(hdc1080);
			hdc1080_change_state(hdc1080, HDC1080_STATE_IDLE);
		}
	}
}

#if 0
void hdc1080_print_struct(HDC1080 *hdc1080) {
	uartbb_puts("Temp: "); uartbb_puti(hdc1080->value.temperature); uartbb_putnl();
	uartbb_puts("Humi: "); uartbb_puti(hdc1080->value.humidity); uartbb_putnl();
	uartbb_puts("Conf: "); uartbb_puti(hdc1080->configuration); uartbb_putnl();
	uartbb_puts("S ID: "); uartbb_puti(hdc1080->ids.serial_id[0]); uartbb_puts(", "); uartbb_puti(hdc1080->ids.serial_id[1]); uartbb_puts(", "); uartbb_puti(hdc1080->ids.serial_id[2]); uartbb_putnl();
	uartbb_puts("M ID: "); uartbb_puti(hdc1080->ids.manufacturer_id); uartbb_putnl();
	uartbb_puts("D ID: "); uartbb_puti(hdc1080->ids.device_id); uartbb_putnl();
}
#endif

void hdc1080_init(HDC1080 *hdc1080) {
	hdc1080_init_i2c(hdc1080);

	// Set default configuration
	hdc1080->configuration.reg      = 0; // First zero the whole thing

	hdc1080->configuration.bit.rst  = 0; // not reset
	hdc1080->configuration.bit.heat = 0; // heater disabled
	hdc1080->configuration.bit.mode = 1; // temperature and humidity aquired
	hdc1080->configuration.bit.tres = HDC1080_RESOLUTION_T_14BIT; // temperature measurement 14 bit
	hdc1080->configuration.bit.hres = HDC1080_RESOLUTION_H_14BIT; // humidity measurement 14 bit

	hdc1080->write_config = true;
	hdc1080->update_ids = 1;
	hdc1080->powerup_wait_time = system_timer_get_ms();
}

void hdc1080_tick(HDC1080 *hdc1080) {
	if(hdc1080->powerup_wait_time != 0) {
		if(!system_timer_is_time_elapsed_ms(hdc1080->powerup_wait_time, HDC1080_POWERUP_TIME)) {
			return;
		}

		hdc1080->powerup_wait_time = 0;
	}

	hdc1080_handle_state(hdc1080);

	if(hdc1080->state == HDC1080_STATE_IDLE) {
		if(hdc1080->update_ids) {
			switch(hdc1080->update_ids) {
				case 1: hdc1080->current_read_reg = HDC1080_REG_SERIAL_ID_LOW;   break;
				case 2: hdc1080->current_read_reg = HDC1080_REG_SERIAL_ID_MID;   break;
				case 3: hdc1080->current_read_reg = HDC1080_REG_SERIAL_ID_HIGH;  break;
				case 4: hdc1080->current_read_reg = HDC1080_REG_MANUFACTURER_ID; break;
				case 5: hdc1080->current_read_reg = HDC1080_REG_DEVICE_ID;       break;
				// TODO: Check IDs for correctness here?
				//       What do we do if it is not correct?
				default: hdc1080->update_ids = 0; return;
			}

			hdc1080->current_read_length = 1;
			hdc1080_change_state(hdc1080, HDC1080_STATE_START_READING);

			hdc1080->update_ids++;
			return;
		}

		if(hdc1080->write_config) {
			hdc1080->current_write_reg = HDC1080_REG_CONFIGURATION;
			hdc1080->current_write_data = hdc1080->configuration.reg;
			hdc1080->current_write_length = 1;
			hdc1080_change_state(hdc1080, HDC1080_STATE_START_WRITING);

			hdc1080->write_config = false;
			return;
		}

		hdc1080_get_humidity();
		hdc1080_get_temperature();

		hdc1080->current_read_reg = HDC1080_REG_TEMPERATURE;
		hdc1080->current_read_length = 2;
		hdc1080->current_write_reg = HDC1080_REG_TEMPERATURE;
		hdc1080->current_write_data = 0;
		hdc1080->current_write_length = 0;
		hdc1080_change_state(hdc1080, HDC1080_STATE_START_WRITING);
	}
}

extern HDC1080 hdc1080;

uint8_t hdc1080_get_heater_config(void) {
	return hdc1080.configuration.bit.heat;
}

void hdc1080_set_heater_config(uint8_t heater_config) {
	hdc1080.configuration.bit.heat = heater_config ? 1: 0;
	hdc1080.write_config = true;
}

int16_t hdc1080_get_humidity(void) {
	return hdc1080.value.humidity*10000/(1 << 16);
}

int16_t hdc1080_get_temperature(void) {
	return hdc1080.value.temperature*16500/(1 << 16) - 4000;
}
