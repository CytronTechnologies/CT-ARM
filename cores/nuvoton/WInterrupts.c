/*
  Copyright (c) 2011-2012 Arduino.  All right reserved.

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

#include "WInterrupts.h"

#define GNUM 16

typedef void (*interruptCB)(void);

static interruptCB callbacksPA[GNUM];
static interruptCB callbacksPB[GNUM];
static interruptCB callbacksPC[GNUM];
static interruptCB callbacksPD[GNUM];
static interruptCB callbacksPE[GNUM];
static interruptCB callbacksPF[GNUM];

/* Configure PIO interrupt sources */
static void __initialize() {
	int i;
	for (i=0; i<GNUM; i++) {
		callbacksPA[i] = NULL;
		callbacksPB[i] = NULL;
		callbacksPC[i] = NULL;
		callbacksPD[i] = NULL;
		callbacksPE[i] = NULL;
		callbacksPF[i] = NULL;
	}
	NVIC_EnableIRQ(GPAB_IRQn);
	NVIC_EnableIRQ(GPCDEF_IRQn);
}

void attachInterrupt(uint32_t pin, void (*callback)(void), uint32_t mode)
{
	static int enabled = 0;
	
#ifdef USE_BoardToPin	
	if(pin > BoardToPin_MAX_COUNT) return;
	pin=BoardToPinInfo[pin].pin;
#else
  	if(pin>GPIO_MAX_COUNT || GPIO_Desc[pin].P==NULL) return;
#endif
  		
	if (!enabled) {
		__initialize();
		enabled = 1;
	}
	// Retrieve pin information
	GPIO_T *pio = GPIO_Desc[pin].P;
	//uint32_t mask = 1<<GPIO_Desc[pin].bit;
	uint32_t pos = GPIO_Desc[pin].bit;
	//uint32_t t;
	
	//for (t = mask; t>1; t>>=1, pos++);
	// Set callback function
	
	if (pio == PA )
		callbacksPA[pos] = callback;
	else if (pio == PB )
		callbacksPB[pos] = callback;
	else if (pio == PC )
		callbacksPC[pos] = callback;
	else if (pio == PD )
		callbacksPD[pos] = callback;
	else if (pio == PE )
		callbacksPE[pos] = callback;
	else if (pio == PF )
		callbacksPF[pos] = callback;		
	// Enable interrupt
	if(mode==FALLING)
		GPIO_EnableInt(pio,pos,GPIO_INT_FALLING);
	else if(mode==RISING)		
		GPIO_EnableInt(pio,pos,GPIO_INT_RISING);	
	else if(mode==CHANGE)
		GPIO_EnableInt(pio,pos,GPIO_INT_BOTH_EDGE);		
}

void detachInterrupt(uint32_t pin)
{

#ifdef USE_BoardToPin	
	if(pin > BoardToPin_MAX_COUNT) return;
	pin=BoardToPinInfo[pin].pin;
#else
  	if(pin>GPIO_MAX_COUNT || GPIO_Desc[pin].P==NULL) return;
#endif
  	
	// Retrieve pin information
	GPIO_T *pio = GPIO_Desc[pin].P;
	//uint32_t mask = 1<<GPIO_Desc[pin].bit;
	uint32_t pos = GPIO_Desc[pin].bit;
	//uint32_t t;
	//for (t = mask; t>1; t>>=1, pos++);	

	// Disable interrupt
	GPIO_DisableInt(pio,pos);
	
	if (pio == PA )
		callbacksPA[pos] = NULL;
	else if (pio == PB )
		callbacksPB[pos] = NULL;
	else if (pio == PC )
		callbacksPC[pos] = NULL;
	else if (pio == PD )
		callbacksPD[pos] = NULL;
	else if (pio == PE )
		callbacksPE[pos] = NULL;
	else if (pio == PF )
		callbacksPF[pos] = NULL;		
}

#ifdef __cplusplus
extern "C" {
#endif

void GPAB_IRQHandler(void)
{
	uint32_t i;		
	for (i=0; i<GNUM; i++) {
		if(PA->ISRC & (1<<i))
	  	{ 
	  		if (callbacksPA[i]) callbacksPA[i]();	  		
	  		PA->ISRC = (1<<i);
	  	}
	  	if(PB->ISRC & (1<<i))
	  	{
	  		if (callbacksPB[i]) callbacksPB[i]();
	  		PB->ISRC = (1<<i);
	  	}
	}  
}
void GPCDEF_IRQHandler(void)
{
	uint32_t i;	
	for (i=0; i<GNUM; i++) {
	  	if(PC->ISRC & (1<<i))
	  	{
	  		if (callbacksPC[i]) callbacksPC[i]();	  		
			PC->ISRC = (1<<i);	  			
	  	}
	  	if(PD->ISRC & (1<<i)) 
	  	{
	  		if (callbacksPD[i]) callbacksPD[i]();
	  		PD->ISRC = (1<<i);	
	  	}
	  	if(PE->ISRC & (1<<i)) 
	  	{
	  		if (callbacksPE[i]) callbacksPE[i]();	  			
	  		PE->ISRC = (1<<i);	
	  	}
	  	if(PF->ISRC & (1<<i)) 
	  	{
	  		if (callbacksPF[i]) callbacksPF[i]();	  			
	  		PF->ISRC = (1<<i);	
	  	}	  	
	}     
}

#ifdef __cplusplus
}
#endif
