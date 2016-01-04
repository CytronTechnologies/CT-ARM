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

int Servo_MAX=100;
int Servo_MIN=0;
int Servo_Pin=0;
int Servo_Val=0;

Servo::Servo()
{

}

uint8_t Servo::attach(int pin)
{
  return this->attach(pin, 0, 100);
}

static uint8_t pinEnabled[PWM_MAX_COUNT];
static uint32_t fixValue[PWM_MAX_COUNT];

uint8_t Servo::attach(int ulPin, int min, int max)
{      
	uint32_t ulValue=0;
	#ifdef USE_BoardToPin
	if(ulPin > BoardToPin_MAX_COUNT) return 0;
	if(BoardToPinInfo[ulPin].type!=PWM_TYPE) return 0;
	ulPin=BoardToPinInfo[ulPin].num;
	#else
	if(ulPin>PWM_MAX_COUNT || PWM_Desc[ulPin].P==NULL) return 0;
	#endif	
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
		int32_t pin=PWM_Desc[ulPin].pintype.num;
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
	
  Servo_MAX=max;
  Servo_MIN=min;
  Servo_Pin=ulPin;
}

void Servo::detach()
{

}

void Servo::write(int value)
{
    if (value < 0)
      value = 0;
    else if (value > 180)
      value = 180;

    //value = map(value, 0, 180, Servo_MIN, Servo_MAX);
    value = map(value, 0, 180, 5, 12);
    Servo_Val=value;
	  PWM_ConfigOutputChannel(PWM_Desc[Servo_Pin].P,PWM_Desc[Servo_Pin].ch,50,Servo_Val);	 
}

int Servo::read() // return the value as degrees
{
  return Servo_Val;
}
