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



extern uint32_t GetTickCount( void );

uint32_t millis( void )
{
// todo: ensure no interrupts
	return GetTickCount() ;
}

uint32_t micros( void )
{
    uint32_t ticks ;
    uint32_t count ;

    SysTick->CTRL;
    do {
        ticks = SysTick->VAL;
        count = GetTickCount();
    } while (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk);

    return count * 1000 + (SysTick->LOAD + 1 - ticks) / (SystemCoreClock/1000000) ;  
}

void delay( uint32_t ms )
{
    uint32_t end = GetTickCount() + ms;
    while (GetTickCount() < end);
}
#if defined(__M451__)
	#define INSTRUCTION_DELAY 8
#elif defined(__NUC240__)
	#define INSTRUCTION_DELAY 20
#elif defined(__NANO100__) | defined(__NANO1X2__)
	#define INSTRUCTION_DELAY 16
#elif defined(__NUC131__)
	#define INSTRUCTION_DELAY 20
#endif
void delayMicroseconds( uint32_t us )
{
#if 1	
	  /* The error of the instruction delay mesurement is 3 us */	  
	  if(us>INSTRUCTION_DELAY)
	  { 
	  	
	  	us-=INSTRUCTION_DELAY;
	  	#if defined(__M451__)
	  	if(us>200) 
	  		us+=INSTRUCTION_DELAY;	  	
	  	#endif
	  }
	  else
	  	return;	
#endif	  		    
    uint32_t start = micros();
    while ((micros() - start) < us);         
}

#if defined ( __ICCARM__ ) /* IAR Ewarm 5.41+ */
extern signed int putchar( signed int c ) ;
/**
 * \brief
 *
 * \param c  Character to output.
 *
 * \return The character that was output.
 */
extern WEAK signed int putchar( signed int c )
{
    return c ;
}
#endif /* __ICCARM__ */

#ifdef __cplusplus
}
#endif
