/*
  Copyright (c) 2011 Arduino.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef _PINS_ARDUINO_H_
#define _PINS_ARDUINO_H_

#include "Arduino.h"

#define USE_BoardToPin

/* Types used for the tables below */
typedef struct _PinType
{
	uint8_t num;
	uint32_t type;
	uint32_t AMsk;
	uint32_t AMsk2;
	uint32_t AMsk3;
	uint32_t AMsk4;
} PinType;

typedef struct _PinDescription
{
	uint32_t MFP;
	uint32_t Mask;
	uint32_t ALTMsk;
	uint32_t ALT2Msk;
	uint32_t ALT3Msk;
	uint32_t ALT4Msk;
	uint32_t Type;
} PinDescription;

typedef struct _GPIOPinDescription
{
	GPIO_T *P;
	uint16_t bit;
	PinDescription Pin;
} GPIOPinDescription;

typedef struct _ADCPinDescription
{
	ADC_T *A;
	uint32_t module;
	uint32_t ch;
	PinType pintype;
} ADCPinDescription;

typedef struct _BPWMPinDescription
{
	BPWM_T *P;
	uint32_t module;
	IRQn_Type irq;
	uint32_t ch;
	uint32_t freq;
	PinType pintype;
} BPWMPinDescription;

typedef struct _PWMPinDescription
{
	PWM_T *P;
	uint32_t module;
	IRQn_Type irq;
	uint32_t ch;
	uint32_t freq;
	PinType pintype;
} PWMPinDescription;

typedef struct _SPIPinDescription
{
	SPI_T *S;
	uint32_t module;
	IRQn_Type irq;
	uint32_t clksel;
	PinType pintype[4];
} SPIPinDescription;

typedef struct _UARTPinDescription
{
	UART_T *U;
	uint32_t module;
	IRQn_Type irq;
	PinType pintype[2];
} UARTPinDescription;

#if defined(__M451__) | defined(__NUC240__)
typedef struct _CANPinDescription
{
	CAN_T *C;
	uint32_t module;
	IRQn_Type irq;
	PinType pintype[2];
} CANPinDescription;
#endif

typedef struct _I2CPinDescription
{
	I2C_T *I;
	uint32_t module;
	PinType pintype[2];
} I2CPinDescription;

typedef struct _BoardToPin
{
	uint8_t pin;
	uint8_t type;
	uint8_t num;
} BoardToPin;

#ifdef USE_BoardToPin /* GPIO mapping */

#define GPIO_TYPE (0u)
#define UART_TYPE (1u)
#define I2C_TYPE (2u)
#define SPI_TYPE (3u)
#define ADC_TYPE (4u)
#define PWM_TYPE (5u)
#define CAN_TYPE (6u)
#define BoardToPin_MAX_COUNT (29u)
extern const BoardToPin BoardToPinInfo[];

#define GPIO_MAX_COUNT (48u)
extern const GPIOPinDescription GPIO_Desc[];
#define GPIO_Config(Desc)                                                             \
	do                                                                                \
	{                                                                                 \
		outp32(Desc.Pin.MFP, (inp32(Desc.Pin.MFP) & ~Desc.Pin.Mask) | Desc.Pin.Type); \
		outp32(&SYS->ALT_MFP, (inp32(&SYS->ALT_MFP) & ~Desc.Pin.ALTMsk));             \
		outp32(&SYS->ALT_MFP2, (inp32(&SYS->ALT_MFP2) & ~Desc.Pin.ALT2Msk));          \
		outp32(&SYS->ALT_MFP3, (inp32(&SYS->ALT_MFP3) & ~Desc.Pin.ALT3Msk));          \
		outp32(&SYS->ALT_MFP4, (inp32(&SYS->ALT_MFP4) & ~Desc.Pin.ALT4Msk));          \
	} while (0);

#define BPWM_MAX_COUNT (4u)
extern BPWMPinDescription BPWM_Desc[];
#define BPWM_Config(Desc)                                                                                                                                      \
	do                                                                                                                                                         \
	{                                                                                                                                                          \
		outp32(GPIO_Desc[Desc.pintype.num].Pin.MFP, (inp32(GPIO_Desc[Desc.pintype.num].Pin.MFP) & ~GPIO_Desc[Desc.pintype.num].Pin.Mask) | Desc.pintype.type); \
		if (GPIO_Desc[Desc.pintype.num].Pin.ALTMsk != NULL)                                                                                                    \
			outp32(&SYS->ALT_MFP, (inp32(&SYS->ALT_MFP) & ~GPIO_Desc[Desc.pintype.num].Pin.ALTMsk) | Desc.pintype.AMsk);                                       \
		if (GPIO_Desc[Desc.pintype.num].Pin.ALT2Msk != NULL)                                                                                                   \
			outp32(&SYS->ALT_MFP2, (inp32(&SYS->ALT_MFP2) & ~GPIO_Desc[Desc.pintype.num].Pin.ALT2Msk) | Desc.pintype.AMsk2);                                   \
		if (GPIO_Desc[Desc.pintype.num].Pin.ALT3Msk != NULL)                                                                                                   \
			outp32(&SYS->ALT_MFP3, (inp32(&SYS->ALT_MFP3) & ~GPIO_Desc[Desc.pintype.num].Pin.ALT3Msk) | Desc.pintype.AMsk3);                                   \
		if (GPIO_Desc[Desc.pintype.num].Pin.ALT4Msk != NULL)                                                                                                   \
			outp32(&SYS->ALT_MFP4, (inp32(&SYS->ALT_MFP4) & ~GPIO_Desc[Desc.pintype.num].Pin.ALT4Msk) | Desc.pintype.AMsk4);                                   \
	} while (0);

#define PWM_MAX_COUNT (12u)
extern PWMPinDescription PWM_Desc[];
#define PWM_Config(Desc)                                                                                                                                       \
	do                                                                                                                                                         \
	{                                                                                                                                                          \
		outp32(GPIO_Desc[Desc.pintype.num].Pin.MFP, (inp32(GPIO_Desc[Desc.pintype.num].Pin.MFP) & ~GPIO_Desc[Desc.pintype.num].Pin.Mask) | Desc.pintype.type); \
		if (GPIO_Desc[Desc.pintype.num].Pin.ALTMsk != NULL)                                                                                                    \
			outp32(&SYS->ALT_MFP, (inp32(&SYS->ALT_MFP) & ~GPIO_Desc[Desc.pintype.num].Pin.ALTMsk) | Desc.pintype.AMsk);                                       \
		if (GPIO_Desc[Desc.pintype.num].Pin.ALT2Msk != NULL)                                                                                                   \
			outp32(&SYS->ALT_MFP2, (inp32(&SYS->ALT_MFP2) & ~GPIO_Desc[Desc.pintype.num].Pin.ALT2Msk) | Desc.pintype.AMsk2);                                   \
		if (GPIO_Desc[Desc.pintype.num].Pin.ALT3Msk != NULL)                                                                                                   \
			outp32(&SYS->ALT_MFP3, (inp32(&SYS->ALT_MFP3) & ~GPIO_Desc[Desc.pintype.num].Pin.ALT3Msk) | Desc.pintype.AMsk3);                                   \
		if (GPIO_Desc[Desc.pintype.num].Pin.ALT4Msk != NULL)                                                                                                   \
			outp32(&SYS->ALT_MFP4, (inp32(&SYS->ALT_MFP4) & ~GPIO_Desc[Desc.pintype.num].Pin.ALT4Msk) | Desc.pintype.AMsk4);                                   \
	} while (0);

#define ADC_MAX_COUNT (6u)
extern ADCPinDescription ADC_Desc[];
#define ADC_Config(Desc)                                                                                                                                       \
	do                                                                                                                                                         \
	{                                                                                                                                                          \
		outp32(GPIO_Desc[Desc.pintype.num].Pin.MFP, (inp32(GPIO_Desc[Desc.pintype.num].Pin.MFP) & ~GPIO_Desc[Desc.pintype.num].Pin.Mask) | Desc.pintype.type); \
		if (GPIO_Desc[Desc.pintype.num].Pin.ALTMsk != NULL)                                                                                                    \
			outp32(&SYS->ALT_MFP, (inp32(&SYS->ALT_MFP) & ~GPIO_Desc[Desc.pintype.num].Pin.ALTMsk) | Desc.pintype.AMsk);                                       \
		if (GPIO_Desc[Desc.pintype.num].Pin.ALT2Msk != NULL)                                                                                                   \
			outp32(&SYS->ALT_MFP2, (inp32(&SYS->ALT_MFP2) & ~GPIO_Desc[Desc.pintype.num].Pin.ALT2Msk) | Desc.pintype.AMsk2);                                   \
		if (GPIO_Desc[Desc.pintype.num].Pin.ALT3Msk != NULL)                                                                                                   \
			outp32(&SYS->ALT_MFP3, (inp32(&SYS->ALT_MFP3) & ~GPIO_Desc[Desc.pintype.num].Pin.ALT3Msk) | Desc.pintype.AMsk3);                                   \
		if (GPIO_Desc[Desc.pintype.num].Pin.ALT4Msk != NULL)                                                                                                   \
			outp32(&SYS->ALT_MFP4, (inp32(&SYS->ALT_MFP4) & ~GPIO_Desc[Desc.pintype.num].Pin.ALT4Msk) | Desc.pintype.AMsk4);                                   \
	} while (0);

#define SPI_MAX_COUNT (1u)
#define SPI_CHANNELS_NUM (1u)
extern SPIPinDescription SPI_Desc[];
#define SPI_SCK (0u)
#define SPI_MOSI (1u)
#define SPI_MISO (2u)
#define SPI_SS (3u)
#define SPI_Config(Desc)                                                                                        \
	do                                                                                                          \
	{                                                                                                           \
		uint8_t i;                                                                                              \
		for (i = 0; i < 4; i++)                                                                                 \
			if (Desc.pintype[i].num != 0)                                                                       \
				outp32(GPIO_Desc[Desc.pintype[i].num].Pin.MFP, (inp32(GPIO_Desc[Desc.pintype[i].num].Pin.MFP) & \
																~GPIO_Desc[Desc.pintype[i].num].Pin.Mask) |     \
																   Desc.pintype[i].type);                       \
	} while (0);

#define UART_MAX_COUNT (6u)
extern UARTPinDescription UART_Desc[];
#define UART_RX (0u)
#define UART_TX (1u)
#define UART_Config(Desc)                                                                                                                                                      \
	do                                                                                                                                                                         \
	{                                                                                                                                                                          \
		uint8_t i;                                                                                                                                                             \
		for (i = 0; i < 2; i++)                                                                                                                                                \
		{                                                                                                                                                                      \
			outp32(GPIO_Desc[Desc.pintype[i].num].Pin.MFP, (inp32(GPIO_Desc[Desc.pintype[i].num].Pin.MFP) & ~GPIO_Desc[Desc.pintype[i].num].Pin.Mask) | Desc.pintype[i].type); \
			if (GPIO_Desc[Desc.pintype[i].num].Pin.ALTMsk != NULL)                                                                                                             \
				outp32(&SYS->ALT_MFP, (inp32(&SYS->ALT_MFP) & ~GPIO_Desc[Desc.pintype[i].num].Pin.ALTMsk) | Desc.pintype[i].AMsk);                                             \
			if (GPIO_Desc[Desc.pintype[i].num].Pin.ALT2Msk != NULL)                                                                                                            \
				outp32(&SYS->ALT_MFP2, (inp32(&SYS->ALT_MFP2) & ~GPIO_Desc[Desc.pintype[i].num].Pin.ALT2Msk) | Desc.pintype[i].AMsk2);                                         \
			if (GPIO_Desc[Desc.pintype[i].num].Pin.ALT3Msk != NULL)                                                                                                            \
				outp32(&SYS->ALT_MFP3, (inp32(&SYS->ALT_MFP3) & ~GPIO_Desc[Desc.pintype[i].num].Pin.ALT3Msk) | Desc.pintype[i].AMsk3);                                         \
			if (GPIO_Desc[Desc.pintype[i].num].Pin.ALT4Msk != NULL)                                                                                                            \
				outp32(&SYS->ALT_MFP4, (inp32(&SYS->ALT_MFP4) & ~GPIO_Desc[Desc.pintype[i].num].Pin.ALT4Msk) | Desc.pintype[i].AMsk4);                                         \
		}                                                                                                                                                                      \
	} while (0);

#if defined(__M451__) | defined(__NUC240__)
#define CAN_MAX_COUNT 1
extern const CANPinDescription CAN_Desc[];
#define CAN_RX 0
#define CAN_TX 1
#define CAN_Config(Desc)                                                                                    \
	do                                                                                                      \
	{                                                                                                       \
		uint8_t i;                                                                                          \
		for (i = 0; i < 2; i++)                                                                             \
			outp32(GPIO_Desc[Desc.pintype[i].num].Pin.MFP, (inp32(GPIO_Desc[Desc.pintype[i].num].Pin.MFP) & \
															~GPIO_Desc[Desc.pintype[i].num].Pin.Mask) |     \
															   Desc.pintype[i].type);                       \
	} while (0);
#endif

#define I2C_MAX_COUNT (1u)
extern I2CPinDescription I2C_Desc[];
#define I2C_SCL (0u)
#define I2C_SDA (1u)
#define I2C_Config(Desc)                                                                                                                                                       \
	do                                                                                                                                                                         \
	{                                                                                                                                                                          \
		uint8_t i;                                                                                                                                                             \
		for (i = 0; i < 2; i++)                                                                                                                                                \
		{                                                                                                                                                                      \
			outp32(GPIO_Desc[Desc.pintype[i].num].Pin.MFP, (inp32(GPIO_Desc[Desc.pintype[i].num].Pin.MFP) & ~GPIO_Desc[Desc.pintype[i].num].Pin.Mask) | Desc.pintype[i].type); \
			if (GPIO_Desc[Desc.pintype[i].num].Pin.ALTMsk != NULL)                                                                                                             \
				outp32(&SYS->ALT_MFP, (inp32(&SYS->ALT_MFP) & ~GPIO_Desc[Desc.pintype[i].num].Pin.ALTMsk) | Desc.pintype[i].AMsk);                                             \
			if (GPIO_Desc[Desc.pintype[i].num].Pin.ALT2Msk != NULL)                                                                                                            \
				outp32(&SYS->ALT_MFP2, (inp32(&SYS->ALT_MFP2) & ~GPIO_Desc[Desc.pintype[i].num].Pin.ALT2Msk) | Desc.pintype[i].AMsk2);                                         \
			if (GPIO_Desc[Desc.pintype[i].num].Pin.ALT3Msk != NULL)                                                                                                            \
				outp32(&SYS->ALT_MFP3, (inp32(&SYS->ALT_MFP3) & ~GPIO_Desc[Desc.pintype[i].num].Pin.ALT3Msk) | Desc.pintype[i].AMsk3);                                         \
			if (GPIO_Desc[Desc.pintype[i].num].Pin.ALT4Msk != NULL)                                                                                                            \
				outp32(&SYS->ALT_MFP4, (inp32(&SYS->ALT_MFP4) & ~GPIO_Desc[Desc.pintype[i].num].Pin.ALT4Msk) | Desc.pintype[i].AMsk4);                                         \
		}                                                                                                                                                                      \
	} while (0);

#endif /* end GPIO mapping */

/*
 * Pins
 */
#define NUM_DIGITAL_PINS (20u)
#define NUM_ANALOG_INPUTS (6u)
#define analogInputToDigitalPin(p) ((p < 6) ? (p) + 14 : -1)
#define digitalPinHasPWM(p) ((p) >= 2 && (p) <= 17)

#define digitalPinToPort(p) (GPIO_Desc[BoardToPinInfo[p].pin].P)
#define digitalPinToBitMask(p) (1 << GPIO_Desc[BoardToPinInfo[p].pin].bit)
//#define analogInPinToBit(p) (p)
#define portOutputRegister(port) (&(port->DOUT))
#define portInputRegister(port) (&(port->PIN))
#define portModeRegister(port) (&(port->PMD))
/*
 * Interrupts
 */
#define digitalPinToInterrupt(p) ((p >= 0 && p < 30) ? p : NOT_AN_INTERRUPT)
/*
 * SPI Interfaces
 */
static const uint8_t SS = 25;
static const uint8_t MOSI = 24;
static const uint8_t MISO = 22;
static const uint8_t SCK = 23;
/*
 * Wire Interfaces
 */
static const uint8_t SDA = 20;
static const uint8_t SCL = 21;
/*
 * LED
 */
#define LED_BUILTIN (13u)
/*
 * Analog pins
 */
static const uint8_t A0 = 14;
static const uint8_t A1 = 15;
static const uint8_t A2 = 16;
static const uint8_t A3 = 17;
static const uint8_t A4 = 18;
static const uint8_t A5 = 19;

#endif
