/******************************************************************************
 * The MIT License
 *
 * Copyright (c) 2010 Bryan Newbold.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *****************************************************************************/

#include "HardwareTimer.h"

/** Timer channel numbers */
static voidFuncPtr TimerFuncPtr[NR_TIMERS];

#if(NR_TIMERS > 0)
HardwareTimer Timer1(0, TMR0_MODULE, CLK_CLKSEL1_TMR0_S_HXT);
#endif
#if(NR_TIMERS > 1)
HardwareTimer Timer2(1, TMR1_MODULE, CLK_CLKSEL1_TMR1_S_HXT);
#endif
#if(NR_TIMERS > 2)
HardwareTimer Timer3(2, TMR2_MODULE, CLK_CLKSEL1_TMR2_S_HXT);
#endif
#if(NR_TIMERS > 3)
HardwareTimer Timer4(3, TMR3_MODULE, CLK_CLKSEL1_TMR3_S_HXT);
#endif

HardwareTimer* Timer[NR_TIMERS] = {
  #if(NR_TIMERS > 0)
    &Timer1,
  #endif
  #if(NR_TIMERS > 1)		
    &Timer2,
  #endif
  #if(NR_TIMERS > 2)
    &Timer3,
  #endif
  #if(NR_TIMERS > 3)
    &Timer4
  #endif
};

#ifdef __cplusplus
extern "C"
{
#endif
	#if(NR_TIMERS > 0)
	void TMR0_IRQHandler(void) { 
		TIMER_ClearIntFlag(TIMER0);
		if(TimerFuncPtr[0])	TimerFuncPtr[0]();
	}
	#endif
	
	#if(NR_TIMERS > 1)
	void TMR1_IRQHandler(void) { 
		TIMER_ClearIntFlag(TIMER1);
		if(TimerFuncPtr[1])	TimerFuncPtr[1](); 		
	}
	#endif
	
	#if(NR_TIMERS > 2)
	void TMR2_IRQHandler(void) { 
		TIMER_ClearIntFlag(TIMER2);
		if(TimerFuncPtr[2])	TimerFuncPtr[2](); 
	}	
	#endif
	
	#if(NR_TIMERS > 3)
	void TMR3_IRQHandler(void) { 
		TIMER_ClearIntFlag(TIMER3);
		if(TimerFuncPtr[3])	TimerFuncPtr[3](); 	
	}	
	#endif
#ifdef __cplusplus
}
#endif

HardwareTimer::HardwareTimer(uint8_t timerNum, uint32_t moduleIdx, uint32_t clksel) {
  if (timerNum >= NR_TIMERS) return;            
  TIMER_T * devs[] = {
    #if(NR_TIMERS > 0)
    TIMER0,
    #endif
    #if(NR_TIMERS > 1)
    TIMER1,
    #endif
    #if(NR_TIMERS > 2)
    TIMER2,
    #endif
    #if(NR_TIMERS > 3)
    TIMER3,
    #endif
  };
  dev = devs[timerNum];
  channel = timerNum;      
  TimerFuncPtr[channel] = NULL; 
  CLK_EnableModuleClock(moduleIdx);
  CLK_SetModuleClock(moduleIdx, clksel, NULL); 
}

void HardwareTimer::open(uint32_t mode, uint32_t freq) { 
  TIMER_Open(dev, mode, freq);		
}
/*
void HardwareTimer::initialize(uint32_t microseconds){
	//open(PERIODIC, 1000000 / microseconds);
	uint32_t u32ClkInMHz = 0;
	u32ClkInMHz = getModuleClock() / 1000000;
	//dev->TCSR = PERIODIC | (u32ClkInMHz - 1);	// 
	dev->TCSR = (dev->TCSR & ~TIMER_TCSR_PRESCALE_Msk)| PERIODIC |(u32ClkInMHz - 1);
	//setCompare(microseconds);  
	dev->TCMPR = microseconds;// range - 2us - 16,777,216 us
}
*/
void HardwareTimer::close() { 
  TIMER_Close(dev);		
}

void HardwareTimer::start() {
  TIMER_Start(dev);
}

void HardwareTimer::setPrescaleFactor(uint32_t factor) {    
  TIMER_SET_PRESCALE_VALUE(dev, factor);
}

void HardwareTimer::setCompare(uint32_t val) {
  TIMER_SET_CMP_VALUE(dev, val);
}

void HardwareTimer::attachInterrupt(void (*callback)(void))
{
  TimerFuncPtr[this->channel] = callback;
  TIMER_EnableInt(dev); //dev->TCSR |= TIMER_TCSR_IE_Msk;	
  NVIC_EnableIRQ((IRQn_Type)((int)TMR0_IRQn + this->channel));
  dev->TCSR |= TIMER_TCSR_CEN_Msk; //enable timer counting    
}

void HardwareTimer::detachInterrupt() {
  
  close(); //dev->TCSR = 0; dev->TEXCON = 0;
  //TimerFuncPtr[channel] = NULL;
  //dev->TCSR &= ~TIMER_TCSR_CEN_Msk; // stop/suspend timer counting
  //dev->TCSR &= ~TIMER_TCSR_IE_Msk; //disable interrupt function
  NVIC_DisableIRQ((IRQn_Type)((int)TMR0_IRQn + channel));
}

uint32_t HardwareTimer::getModuleClock(){
	return TIMER_GetModuleClock(dev);
}

void HardwareTimer::clearIntFlag(){
	TIMER_ClearIntFlag(dev);
}
