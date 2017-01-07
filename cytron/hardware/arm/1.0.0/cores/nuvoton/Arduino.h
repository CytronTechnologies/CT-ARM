/******************************************************************************
 * @file     Arduino.h
 * @version  V2.00
 * $Date: 13/10/07 3:57p $ 
 * @brief    MINI51 Series Global Control for Arduino
 *
 * @note
 * Copyright (C) 2012 Nuvoton Technology Corp. All rights reserved.
 *
 ******************************************************************************/
 
#ifndef _ARDUINO_H_
#define _ARDUINO_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "NUC131.h"
#include "Pins_arduino.h"
#include "wiring_constants.h"
#include "avr/pgmspace.h"

#define ARDUINO 100

extern uint32_t SystemTickClock;
#define clockCyclesPerMicrosecond() ( SystemCoreClock / 1000000L )
#define clockCyclesToMicroseconds(a) ( ((a) * 1000L) / (SystemCoreClock / 1000L) )
#define microsecondsToClockCycles(a) ( (a) * (SystemCoreClock / 1000000L) )

#ifdef __cplusplus
extern "C" {
#endif
  #include "delay.h"
  #include "wiring_digital.h"
  #include "wiring_analog.h"
  //#include "wiring_shift.h"
  #include "WInterrupts.h"

  void yield(void);

#ifdef __cplusplus
}

#include "WCharacter.h"
#include "WString.h"
#include "WMath.h"
#include "HardwareSerial.h"
#include "HardwareTimer.h"
#include "wiring_pulse.h"

void tone(uint8_t _pin, unsigned int frequency, unsigned long duration = 0);
void noTone(uint8_t _pin);

#endif
	

#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2
static volatile uint8_t init_flag = 0;	

void init(void);

void setup(void);
void loop(void);

#include "USBCore.h"
#include "USBAPI.h"

#include "binary.h"
#include "itoa.h"

#endif
