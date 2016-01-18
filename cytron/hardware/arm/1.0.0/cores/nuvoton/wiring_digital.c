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

extern void pinMode(uint32_t ulPin, uint32_t ulMode)
{
	uint32_t len;

	if(ulPin > BoardToPin_MAX_COUNT) return;
	if(BoardToPinInfo[ulPin].pin == -1) return;
	ulPin = BoardToPinInfo[ulPin].pin;

  GPIO_Config(GPIO_Desc[ulPin]);

	switch(ulMode)
  {
    case INPUT:                        
      GPIO_SetMode(GPIO_Desc[ulPin].P, 1<<GPIO_Desc[ulPin].bit, GPIO_PMD_INPUT);
      break ;

    case INPUT_PULLUP:            
      GPIO_SetMode(GPIO_Desc[ulPin].P, 1<<GPIO_Desc[ulPin].bit, GPIO_PMD_QUASI);                    
      break ;

    case OUTPUT:
    	GPIO_SetMode(GPIO_Desc[ulPin].P, 1<<GPIO_Desc[ulPin].bit, GPIO_PMD_OUTPUT);
      (GPIO_Desc[ulPin].P)->DOUT &= ~(1<<GPIO_Desc[ulPin].bit); // By default, set LOW
      break ;

    default:
      break ;
  }
}

extern void digitalWrite(uint32_t ulPin, uint32_t ulVal)
{
	if(ulPin > BoardToPin_MAX_COUNT) return;
	if(BoardToPinInfo[ulPin].pin == -1) return;
	ulPin = BoardToPinInfo[ulPin].pin;

  GPIO_Config(GPIO_Desc[ulPin]);

	if(ulVal == HIGH)
		(GPIO_Desc[ulPin].P)->DOUT |= (1<<GPIO_Desc[ulPin].bit);
	else
		(GPIO_Desc[ulPin].P)->DOUT &= ~(1<<GPIO_Desc[ulPin].bit);
}

extern int digitalRead(uint32_t ulPin)
{
	if(ulPin > BoardToPin_MAX_COUNT) return;     
	if(BoardToPinInfo[ulPin].pin == -1) return;
	ulPin = BoardToPinInfo[ulPin].pin;
	return ((GPIO_Desc[ulPin].P)->PIN & (1<<GPIO_Desc[ulPin].bit))? HIGH:LOW;
}

#ifdef __cplusplus
}
#endif
