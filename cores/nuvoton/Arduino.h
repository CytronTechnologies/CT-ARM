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

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "wiring_constants.h"

#include "avr/pgmspace.h"
#include "avr/interrupt.h"
#include "avr/io.h"

#include "binary.h"
#include "itoa.h"

static volatile uint8_t init_flag = 0;

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "NUC131.h"
#include "pins_arduino.h"

extern uint32_t SystemTickClock;
#define clockCyclesPerMicrosecond() (SystemCoreClock / 1000000L)
#define clockCyclesToMicroseconds(a) (((a)*1000L) / (SystemCoreClock / 1000L))
#define microsecondsToClockCycles(a) ((a) * (SystemCoreClock / 1000000L))

void yield(void);

/* system functions */
int main(void);
void init(void);

/* sketch */
void setup(void);
void loop(void);

#ifdef __cplusplus
} // extern "C"
#endif

// The following headers are for C++ only compilation
#ifdef __cplusplus

#include "WCharacter.h"
#include "WString.h"
#include "Tone.h"
#include "WMath.h"
#include "HardwareSerial.h"
#include "HardwareTimer.h"
#include "wiring_pulse.h"

#endif

#include "delay.h"
#include "wiring_digital.h"
#include "wiring_analog.h"
#include "wiring_shift.h"
#include "WInterrupts.h"

// USB device
#include "USBCore.h"
#include "USBAPI.h"

#endif
