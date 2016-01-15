/*
  Copyright (c) 2013 Arduino LLC. All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
*/
#include "Arduino.h"
#include "Servo.h"

Servo::Servo():
	Servo_MAX(12),
	Servo_MIN(3),
	Servo_Freq(50),
	Servo_Pin(0),
	Servo_Val(0),
	bpwm(0)
{}

//static uint8_t pinEnabled[PWM_MAX_COUNT];
//static uint32_t fixValue[PWM_MAX_COUNT];

uint8_t Servo::attach(int ulPin, int min, int max, int freq)
{      
	uint32_t ulValue=0;
	bpwm = 0;
	
	#ifdef USE_BoardToPin
	if(ulPin > BoardToPin_MAX_COUNT) return 0;
	
	if(ulPin > 5 && ulPin < 10) { // BPWM pins - 6, 7, 8, 9
		bpwm = 1;
	}
	
	if(ulPin > 17 && ulPin < 22) { // PWM at analog pins - A0, A1, A2, A3
		ulPin = ulPin + 8;
	}
	
	if(BoardToPinInfo[ulPin].type!=PWM_TYPE) return 0;
	
	ulPin=BoardToPinInfo[ulPin].num;
	
	if(bpwm) {
		if(ulPin > BPWM_MAX_COUNT || BPWM_Desc[ulPin].P == NULL) return 0;
	}
	else {
		if(ulPin  > PWM_MAX_COUNT || PWM_Desc[ulPin].P == NULL) return 0;
	}
	#else
		return 0;
	#endif	
	
	if(bpwm) 
	{
		BPWM_Config(BPWM_Desc[ulPin]); // Set Mutifunction pins
		BPWM_ConfigOutputChannel(BPWM_Desc[ulPin].P, BPWM_Desc[ulPin].ch, BPWM_Desc[ulPin].freq, ulValue); // Config BPWMs
		BPWM_EnableOutput(BPWM_Desc[ulPin].P, (1 << BPWM_Desc[ulPin].ch)); //Enable BPWM output
		BPWM_Start(BPWM_Desc[ulPin].P, (1 << BPWM_Desc[ulPin].ch)); //Start BPWM
	}
	
	else 
	{
		PWM_Config(PWM_Desc[ulPin]); // Set Mutifunction pins
		PWM_ConfigOutputChannel(PWM_Desc[ulPin].P, PWM_Desc[ulPin].ch, PWM_Desc[ulPin].freq, ulValue); // Config PWMs
		PWM_EnableOutput(PWM_Desc[ulPin].P, (1 << PWM_Desc[ulPin].ch)); //Enable PWM output
		PWM_Start(PWM_Desc[ulPin].P, (1 << PWM_Desc[ulPin].ch)); //Start PWM
	}
	
	Servo_MAX = max;
	Servo_MIN = min;
	Servo_Pin = ulPin;
	Servo_Freq = freq;
				
/*
#if defined(__M451__)
	if(ulValue==100)
	{  
		int32_t pin=PWM_Desc[ulPin].pintype.num;
		GPIO_Config(GPIO_Desc[pin]);
		GPIO_SetMode(GPIO_Desc[pin].P, GPIO_Desc[pin].bit, GPIO_MODE_OUTPUT);
		(GPIO_Desc[pin].P)->DOUT |= GPIO_Desc[pin].bit;
		pinEnabled[ulPin]= 0;
		fixValue[ulPin]=ulValue;
		//return 0;
	}
#elif defined(__NUC240__)
	if(ulValue==0)
	{  
		int32_t pin=PWM_Desc[ulPin].pintype.num;
		GPIO_Config(GPIO_Desc[pin]);
		GPIO_SetMode(GPIO_Desc[pin].P, GPIO_Desc[pin].bit, GPIO_PMD_OUTPUT);
		(GPIO_Desc[pin].P)->DOUT &= ~GPIO_Desc[pin].bit;
		pinEnabled[ulPin]= 0;
		fixValue[ulPin]=ulValue;
		//return 0;
	}
#elif defined(__NANO100__) | defined(__NANO1X2__)
	if(ulValue==0)
	{  
		int32_t pin=PWM_Desc[ulPin].pintype.num;
		GPIO_Config(GPIO_Desc[pin]);
		GPIO_SetMode(GPIO_Desc[pin].P, GPIO_Desc[pin].bit, GPIO_PMD_OUTPUT);
		(GPIO_Desc[pin].P)->DOUT &= ~GPIO_Desc[pin].bit;
		pinEnabled[ulPin]= 0;
		fixValue[ulPin]=ulValue;
		//return 0;
	}
#elif defined(__NUC131__)

	if(ulValue==0)
	{  
		uint32_t pin = PWM_Desc[ulPin].pintype.num;
		GPIO_Config(GPIO_Desc[pin]);
		GPIO_SetMode(GPIO_Desc[pin].P, GPIO_Desc[pin].bit, GPIO_PMD_OUTPUT);
		(GPIO_Desc[pin].P)->DOUT &= ~GPIO_Desc[pin].bit;
		pinEnabled[ulPin]= 0;
		fixValue[ulPin]=ulValue;
		//return 0;
	}
#endif

	if (!pinEnabled[ulPin]){
		//Set Mutifunction pins

		PWM_Config(PWM_Desc[ulPin]);		
    
		//Config PWMs
		PWM_ConfigOutputChannel(PWM_Desc[ulPin].P,PWM_Desc[ulPin].ch,50,0);
		
		//Enable PWM output
		PWM_EnableOutput(PWM_Desc[ulPin].P,(1<<PWM_Desc[ulPin].ch));
		
		//Start PWM
		PWM_Start(PWM_Desc[ulPin].P,(1<<PWM_Desc[ulPin].ch));
		
		pinEnabled[ulPin] = 1;
	}
	
	//Config PWMs		
	if(fixValue[ulPin]!=ulValue)
	{
		PWM_ConfigOutputChannel(PWM_Desc[ulPin].P,PWM_Desc[ulPin].ch,50,ulValue);
		fixValue[ulPin]=ulValue;
	}
*/

}

void Servo::detach()
{
	if(bpwm)
		BPWM_DisableOutput(BPWM_Desc[Servo_Pin].P,(1<<BPWM_Desc[Servo_Pin].ch));
	
	else
		PWM_DisableOutput(PWM_Desc[Servo_Pin].P,(1<<PWM_Desc[Servo_Pin].ch));	

}

void Servo::write(int value)
{
    if (value < 0)
      value = 0;
    else if (value > 180)
      value = 180;
  	
	Servo_Val = value;

    value = map(value, 0, 180, Servo_MIN, Servo_MAX);
    //value = map(value, 0, 180, 5, 12);
	if(bpwm)
		BPWM_ConfigOutputChannel(BPWM_Desc[Servo_Pin].P, BPWM_Desc[Servo_Pin].ch, Servo_Freq, value);
	else
		PWM_ConfigOutputChannel(PWM_Desc[Servo_Pin].P,PWM_Desc[Servo_Pin].ch,Servo_Freq,value);	 
}

int Servo::read() // return the value as degrees
{
  return Servo_Val;
}
