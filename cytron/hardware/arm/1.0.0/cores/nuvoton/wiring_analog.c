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

#include "Arduino.h"

#ifdef __cplusplus
extern "C" {
#endif

static int _readResolution = 10;
static int _writeResolution = 8;

void analogReadResolution(int res) {
	_readResolution = res;
}

void analogWriteResolution(int res) {
	_writeResolution = res;
}

static inline uint32_t mapResolution(uint32_t value, uint32_t from, uint32_t to) {
	if (from == to)
		return value;
	if (from > to)
		return (value >> (from-to));
	else
		return (value << (to-from));
}

eAnalogReference analog_reference = AR_DEFAULT;

void analogReference(eAnalogReference ulMode)
{
	analog_reference = ulMode;
}

uint32_t analogRead(uint32_t ulPin)
{
  uint32_t ulValue = 0;

  if(ulPin < 6) { // We consider user are using number 0 for A0
    ulPin = ulPin + A0;
  }
  ulPin = BoardToPinInfo[ulPin].num; // Map GPIO pin to analog channel

  if(ulPin > ADC_MAX_COUNT || ADC_Desc[ulPin].A == NULL) return;
  
  ADC_Config(ADC_Desc[ulPin]);
  //GPIO_ENABLE_DIGITAL_PATH(GPIO_Desc[ADC_Desc[ulPin].pintype.num].P,GPIO_Desc[ADC_Desc[ulPin].pintype.num].bit);
	ADC_Open(ADC_Desc[ulPin].A, 0, 0, (1 << ADC_Desc[ulPin].ch)); // Enable channel 0
	ADC_POWER_ON(ADC_Desc[ulPin].A); // Power on ADC
  while(ADC_IS_BUSY(ADC_Desc[ulPin].A)); // Wait for busy of conversion
  ADC_START_CONV(ADC_Desc[ulPin].A); // Start for conversion
	while(!ADC_GET_INT_FLAG(ADC_Desc[ulPin].A, ADC_ADF_INT)); // Wait for end of conversion
	ADC_CLR_INT_FLAG(ADC_Desc[ulPin].A, ADC_ADF_INT); // Clear ADC flag
	
	// Read the value
	ulValue = ADC_GET_CONVERSION_DATA(ADC_Desc[ulPin].A, ADC_Desc[ulPin].ch);	
	//ulValue = mapResolution(ulValue, 12, _readResolution); // Convert 12 to 10 bit
  
  //GPIO_DISABLE_DIGITAL_PATH(GPIO_Desc[ADC_Desc[ulPin-1].pintype.num].P,GPIO_Desc[ADC_Desc[ulPin].pintype.num-1].bit);
  ADC_Close(ADC_Desc[ulPin].A); // Close ADC

  return ulValue;	
}

// Right now, PWM output only works on the pins with
// hardware support. These are defined in the appropriate
// pins_*.c file. For the rest of the pins, we default
// to digital output.

//static uint8_t pinEnabled[PWM_MAX_COUNT] = {0};
//static uint32_t fixValue[PWM_MAX_COUNT] = {0};

void analogWrite(uint32_t ulPin, uint32_t ulValue)
{
	uint8_t bpwm = 0;

	if(ulPin > BoardToPin_MAX_COUNT) return;
	
  if(ulPin > 5 && ulPin < 10) { // BPWM pins - 6, 7, 8, 9
    bpwm = 1;
  }
  if(ulPin > 17 && ulPin < 22) { // PWM at analog pins - A0, A1, A2, A3
    ulPin = ulPin + 8;
  }

  if(BoardToPinInfo[ulPin].type != PWM_TYPE) return;

  uint32_t pwmPin = BoardToPinInfo[ulPin].num;
  if(bpwm) {
    if(pwmPin > BPWM_MAX_COUNT || BPWM_Desc[pwmPin].P == NULL) return;
  }
	else {
    if(pwmPin > PWM_MAX_COUNT || PWM_Desc[pwmPin].P == NULL) return;
  }
	
	ulValue = mapResolution(ulValue, _writeResolution, 8);
	ulValue = ((ulValue + 1) * 100) / (1 << _writeResolution);

	if(ulValue == 0) {
    digitalWrite(ulPin, LOW);
//		pinEnabled[ulPin] = 0;
//		fixValue[ulPin] = ulValue;
		return;
	}
  else if(ulValue == 255) {
    digitalWrite(ulPin, HIGH);
//    pinEnabled[ulPin] = 0;
//    fixValue[ulPin] = ulValue;
  }
  else {
    if(bpwm) {
      BPWM_Config(BPWM_Desc[pwmPin]); // Set Mutifunction pins
      BPWM_ConfigOutputChannel(BPWM_Desc[pwmPin].P, BPWM_Desc[pwmPin].ch, BPWM_Desc[pwmPin].freq, ulValue); // Config BPWMs
      BPWM_EnableOutput(BPWM_Desc[pwmPin].P, (1 << BPWM_Desc[pwmPin].ch)); //Enable BPWM output
      BPWM_Start(BPWM_Desc[pwmPin].P, (1 << BPWM_Desc[pwmPin].ch)); //Start BPWM
    }
    else {
      PWM_Config(PWM_Desc[pwmPin]); // Set Mutifunction pins
      PWM_ConfigOutputChannel(PWM_Desc[pwmPin].P, PWM_Desc[pwmPin].ch, PWM_Desc[pwmPin].freq, ulValue); // Config PWMs
      PWM_EnableOutput(PWM_Desc[pwmPin].P, (1 << PWM_Desc[pwmPin].ch)); //Enable PWM output
      PWM_Start(PWM_Desc[pwmPin].P, (1 << PWM_Desc[pwmPin].ch)); //Start PWM
    }
  }

/*
  if(!pinEnabled[ulPin]) {
    if(bpwm) {
      BPWM_Config(BPWM_Desc[pwmPin]); // Set Mutifunction pins
      BPWM_ConfigOutputChannel(BPWM_Desc[pwmPin].P, BPWM_Desc[pwmPin].ch, BPWM_Desc[pwmPin].freq, ulValue); // Config BPWMs
      BPWM_EnableOutput(BPWM_Desc[pwmPin].P, (1 << BPWM_Desc[pwmPin].ch)); //Enable BPWM output
      BPWM_Start(BPWM_Desc[pwmPin].P, (1 << BPWM_Desc[pwmPin].ch)); //Start BPWM
    }
    else {
      PWM_Config(PWM_Desc[pwmPin]); // Set Mutifunction pins
      PWM_ConfigOutputChannel(PWM_Desc[pwmPin].P, PWM_Desc[pwmPin].ch, PWM_Desc[pwmPin].freq, ulValue); // Config PWMs
      PWM_EnableOutput(PWM_Desc[pwmPin].P, (1 << PWM_Desc[pwmPin].ch)); //Enable PWM output
      PWM_Start(PWM_Desc[pwmPin].P, (1 << PWM_Desc[pwmPin].ch)); //Start PWM
    }
    pinEnabled[ulPin] = 1;
  }
	
	// Config PWMs
	if(fixValue[ulPin] != ulValue) {
    if(bpwm) {
      BPWM_ConfigOutputChannel(BPWM_Desc[pwmPin].P, BPWM_Desc[pwmPin].ch, BPWM_Desc[pwmPin].freq, ulValue);
    }
    else {
      PWM_ConfigOutputChannel(PWM_Desc[pwmPin].P, PWM_Desc[pwmPin].ch, PWM_Desc[pwmPin].freq, ulValue);
    }
		fixValue[ulPin] = ulValue;
	}
*/
	
	return;
}

#ifdef __cplusplus
}
#endif
