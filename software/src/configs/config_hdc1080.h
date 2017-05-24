/* humidity-v2-bricklet
 * Copyright (C) 2017 Olaf Lüke <olaf@tinkerforge.com>
 *
 * config_hdc1080.c: Configuration for HDC1080 humidity sensor
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

#ifndef CONFIG_HDC1080_H
#define CONFIG_HDC1080_H

#include "xmc_gpio.h"

#define HDC1080_I2C_BAUDRATE         400000

#define HDC1080_I2C_ADDRESS          0b1000000
#define HDC1080_I2C_CHANNEL          USIC0_CH1
#define HDC1080_I2C                  XMC_I2C0_CH1

#define HDC1080_SCL_PIN              P0_8
#define HDC1080_SCL_PIN_MODE         XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN_ALT7
#define HDC1080_SCL_INPUT            XMC_USIC_CH_INPUT_DX1
#define HDC1080_SCL_SOURCE           1

#define HDC1080_SDA_PIN              P0_6
#define HDC1080_SDA_PIN_MODE         XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN_ALT7
#define HDC1080_SDA_INPUT            XMC_USIC_CH_INPUT_DX0
#define HDC1080_SDA_SOURCE           2

#define HDC1080_IRQ_RX               11
#define HDC1080_IRQ_RX_PRIORITY      0
#define HDC1080_SERVICE_REQUEST_RX   2
//#define HDC1080_IRQCTRL_RX          XMC_SCU_IRQCTRL_USIC1_SR2_IRQ11

#define HDC1080_IRQ_TX               12
#define HDC1080_IRQ_TX_PRIORITY      1
#define HDC1080_SERVICE_REQUEST_TX   3
//#define HDC1080_IRQCTRL_TX          XMC_SCU_IRQCTRL_USIC1_SR3_IRQ12

#endif
