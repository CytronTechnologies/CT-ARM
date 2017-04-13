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

#ifndef _WIRING_
#define _WIRING_

#ifdef __cplusplus
extern "C" {
#endif

/**
 *
 */
extern void init( void ) ;

/**
 * \brief Returns the number of milliseconds since the Arduino board began running the current program.
 *
 * This number will overflow (go back to zero), after approximately 50 days.
 *
 * \return Number of milliseconds since the program started (uint32_t)
 */
extern uint32_t millis( void ) ;

/**
 * \brief Returns the number of microseconds since the Arduino board began running the current program.
 *
 * This number will overflow (go back to zero), after approximately 70 minutes. On 16 MHz Arduino boards
 * (e.g. Duemilanove and Nano), this function has a resolution of four microseconds (i.e. the value returned is
 * always a multiple of four). On 8 MHz Arduino boards (e.g. the LilyPad), this function has a resolution
 * of eight microseconds.
 *
 * \note There are 1,000 microseconds in a millisecond and 1,000,000 microseconds in a second.
 */
extern uint32_t micros( void ) ;

/**
 * \brief Pauses the program for the amount of time (in miliseconds) specified as parameter.
 * (There are 1000 milliseconds in a second.)
 *
 * \param dwMs the number of milliseconds to pause (uint32_t)
 */
extern void delay( uint32_t dwMs ) ;

/**
 * \brief Pauses the program for the amount of time (in microseconds) specified as parameter.
 *
 * \param dwUs the number of microseconds to pause (uint32_t)
 */
static inline void delayMicroseconds( uint32_t ) __attribute__((always_inline, unused)) ;
static inline void delayMicroseconds( uint32_t us )
{
	if ( us == 0 ) return ;
#if 0

#if defined(__M451__)
	#define INSTRUCTION_DELAY 8
#elif defined(__NUC240__)
	#define INSTRUCTION_DELAY 20
#elif defined(__NANO100__) | defined(__NANO1X2__)
	#define INSTRUCTION_DELAY 16
#elif defined(__NUC131__)
	#define INSTRUCTION_DELAY 20
#endif

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

#else
	uint32_t n = us * (SystemCoreClock / 1000000) / 3;
	__asm__ __volatile__( 
	"1:              \n"
    "   sub %0, #1   \n" // substract 1 from %0 (n)
    "   bne 1b       \n" // if result is not 0 jump to 1
    : "+r" (n)           // '%0' is n variable with RW constraints
    :                    // no input
    :                    // no clobber
  );
#endif	
}

#ifdef __cplusplus
}
#endif

#endif /* _WIRING_ */
