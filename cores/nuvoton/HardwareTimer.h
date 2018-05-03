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

/**
 *  @brief Wirish timer class.
 */

#ifndef _HARDWARETIMER_H_
#define _HARDWARETIMER_H_

#include "Arduino.h"

typedef void (*voidFuncPtr)(void);

/** Timer mode. */
#define ONESHOT TIMER_ONESHOT_MODE
#define PERIODIC TIMER_PERIODIC_MODE
#define TOGGLE TIMER_TOGGLE_MODE
#define CONTINUOUS TIMER_CONTINUOUS_MODE

/** @brief Deprecated; use TIMER_OUTPUT_COMPARE instead. */
#define TIMER_OUTPUTCOMPARE TIMER_OUTPUT_COMPARE
#define NR_TIMERS 4

extern uint32_t TimeClock;
/**
 * @brief Interface to one of the 16-bit timer peripherals.
 */

class HardwareTimer
{
public:
  /**
    * @brief Construct a new HardwareTimer instance.
    * @param timerNum number of the timer to control.
    * @param moduleIdx is including TMR0_MODULE,TMR1_MODULE,TMR2_MODULE and TMR3_MODULE
    * @param clkse is including CLK_CLKSEL1_TMRx_S_XTAL       
    *													 CLK_CLKSEL1_TMRx_S_IRC10K       
    *													 CLK_CLKSEL1_TMRx_S_HCLK         
    *													 CLK_CLKSEL1_TMRx_S_IRC22M 
    */
  HardwareTimer(uint8_t timerNum, uint32_t moduleIdx, uint32_t clksel);

  /**
    * @brief This API is used to configure timer to operate in specified mode
    *        and frequency. If timer cannot work in target frequency, a closest
    *        frequency will be chose and returned.  
    * @param[in] u32Mode Operation mode. Possible options are
    *                 - \ref ONESHOT
    *                 - \ref PERIODIC
    *                 - \ref TOGGLE
    *                 - \ref CONTINUOUS
    * @param[in] u32Freq Target working frequency
    * @return Real Timer working frequency
    * @note After calling this API, Timer is \b NOT running yet. But could start timer running be calling
    *       \ref TIMER_Start macro or program registers directly
    */
  void open(uint32_t mode, uint32_t freq);

  /**
    * @brief This API starts Timer counting
    * @return None
    */
  void start(void);

  /**
    * @brief This API stops Timer counting and disable the Timer interrupt function  
    * @return None
    */
  void close(void);

  /**
    * @brief This API is used to set timer prescale value and timer source clock will be divided by (prescale + 1) before it is fed into timer	  
    * @return None
    */
  void setPrescaleFactor(uint32_t factor);

  /**
    * @brief This API is used to set timer compared value to adjust timer time-out interval	  
    * @return None
    */
  void setCompare(uint32_t compare);

  /**
    * @brief This API is used to get the clock frequency of Timer	  
    * @return Timer clock frequency
    * @note This API cannot return correct clock rate if timer source is external clock input.
    */
  uint32_t getModuleClock();

  /**
    * @brief This function clears the Timer time-out interrupt flag.		  
    * @return None
    */
  void clearIntFlag();

  /**
    * @brief This function initialize timer in specific operation mode and frequency.		  
    * @return None
    */
  void initialize(uint32_t microseconds = 1000000) __attribute__((always_inline))
  {
    setPeriod(microseconds);
    uint32_t u32ClkInMHz = 0;
    u32ClkInMHz = getModuleClock() / 1000000;
    // set prescaler to u32ClkMHz -1 so the timer source clock is divided by u32ClkMHz
    // Frequency = timer source clock / (getModuleClock / 1000000) = 1000000Hz
    // which gives period = 1us, increment takes 1us to complete
    dev->TCSR = (dev->TCSR & ~TIMER_TCSR_PRESCALE_Msk) | PERIODIC | (u32ClkInMHz - 1);
    if (microseconds < 2)
      microseconds = 2;
    if (microseconds > 0xFFFFFF)
      microseconds = 0xFFFFFF;
    dev->TCMPR = microseconds; // range - 2us - 16,777,216 us
  }

  /**
    * @brief This function initialize timer in specific operation mode and frequency.		  
    * @return None
    */
  void setPeriod(unsigned long microseconds)
  {
    uint32_t frequency = 1000000 / microseconds;

    // minimum frequency = 1Hz, max = 1MHz
    if (frequency == 0)
      frequency = 1;
    pwmFrequency = frequency;
  }

  //****************************
  //  PWM outputs
  //****************************
  void setPwmDuty(char pin, unsigned short duty);
  void pwm(char pin, unsigned short duty);
  void pwm(char pin, unsigned short duty, unsigned long microseconds);
  void disablePwm(char pin);
  
  /**
    * @brief Attach an interrupt handler to the given channel.
    *
    * This interrupt handler will be called when the timer's counter
    * reaches the given channel compare value.
    *
    * @param channel the channel to attach the ISR to, from 1 to 4.
    * @param handler The ISR to attach to the given channel.
    * @see voidFuncPtr
    */
  void attachInterrupt(voidFuncPtr handler);

  /**
    * @brief Remove the interrupt handler attached to the given
    *        channel, if any.
    *
    * The handler will no longer be called by this timer.
    *
    * @param channel the channel whose interrupt to detach, from 1 to 4.
    * @see HardwareTimer::attachInterrupt()
    */
  void detachInterrupt();

private:
  // properties
  unsigned short pwmFrequency = 25000; // default 25KHz
  unsigned short dutyCycle = 0;        // default 0% duty cycle
  TIMER_T *dev;
  int channel;
};

/* -- The rest of this file is deprecated. --------------------------------- */

#if (NR_TIMERS > 0)
extern HardwareTimer Timer1;
#endif
#if (NR_TIMERS > 1)
extern HardwareTimer Timer2;
#endif
#if (NR_TIMERS > 2)
extern HardwareTimer Timer3;
#endif
#if (NR_TIMERS > 3)
extern HardwareTimer Timer4;
#endif
//extern HardwareTimer Timer[NR_TIMERS];

#endif
