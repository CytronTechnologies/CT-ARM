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

#ifdef __cplusplus
extern "C" {
#endif

#include "Arduino.h"

extern void pinMode(uint8_t ulPin, uint8_t ulMode)
{
	if (ulPin > BoardToPin_MAX_COUNT)
		return;
	ulPin = BoardToPinInfo[ulPin].pin;

	GPIO_Config(GPIO_Desc[ulPin]);

	switch (ulMode)
	{
	case INPUT:
		GPIO_SetMode(GPIO_Desc[ulPin].P, 1 << GPIO_Desc[ulPin].bit, GPIO_PMD_INPUT);
		break;

	case INPUT_PULLUP:
		GPIO_SetMode(GPIO_Desc[ulPin].P, 1 << GPIO_Desc[ulPin].bit, GPIO_PMD_QUASI);
		// set to HIGH to make pin act as digital input, refer to manual pg 171
		(GPIO_Desc[ulPin].P)->DOUT |= (1 << GPIO_Desc[ulPin].bit);
		break;

	case OUTPUT:
		GPIO_SetMode(GPIO_Desc[ulPin].P, 1 << GPIO_Desc[ulPin].bit, GPIO_PMD_OUTPUT);
		(GPIO_Desc[ulPin].P)->DOUT &= ~(1 << GPIO_Desc[ulPin].bit); // By default, set LOW
		break;

	default:
		break;
	}
}

extern void digitalWrite(uint8_t ulPin, uint8_t ulVal)
{
	if (ulPin > BoardToPin_MAX_COUNT)
		return;
	ulPin = BoardToPinInfo[ulPin].pin;

	GPIO_Config(GPIO_Desc[ulPin]);

	if (ulVal == LOW)
	{
		(GPIO_Desc[ulPin].P)->DOUT &= ~(1 << GPIO_Desc[ulPin].bit);
	}
	else
	{
		(GPIO_Desc[ulPin].P)->DOUT |= (1 << GPIO_Desc[ulPin].bit);
	}
}

extern short digitalRead(uint8_t ulPin)
{
	if (ulPin > BoardToPin_MAX_COUNT)
		return LOW;
	ulPin = BoardToPinInfo[ulPin].pin;
	return ((GPIO_Desc[ulPin].P)->PIN & (1 << GPIO_Desc[ulPin].bit)) ? HIGH : LOW;
}

#ifdef __cplusplus
}
#endif
