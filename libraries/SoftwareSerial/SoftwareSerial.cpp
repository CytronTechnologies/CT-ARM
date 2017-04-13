/*
SoftwareSerial.cpp (formerly NewSoftSerial.cpp) - 
Multi-instance software serial library for Arduino/Wiring
-- Interrupt-driven receive and other improvements by ladyada
   (http://ladyada.net)
-- Tuning, circular buffer, derivation from class Print/Stream,
   multi-instance support, porting to 8MHz processors,
   various optimizations, PROGMEM delay tables, inverse logic and 
   direct port writing by Mikal Hart (http://www.arduiniana.org)
-- Pin change interrupt macros by Paul Stoffregen (http://www.pjrc.com)
-- 20MHz processor support by Garrett Mace (http://www.macetech.com)
-- ATmega1280/2560 support by Brett Hagman (http://www.roguerobotics.com/)

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

The latest version of this library can always be found at
http://arduiniana.org.
*/

// When set, _DEBUG co-opts pins 11 and 13 for debugging with an
// oscilloscope or logic analyzer.  Beware: it also slightly modifies
// the bit times, so don't rely on it too much at high baud rates
#define _DEBUG 0
#define _DEBUG_PIN1 11
#define _DEBUG_PIN2 13
// 
// Includes
// 
#include <WInterrupts.h>
#include <Arduino.h>
#include <SoftwareSerial.h>

//
// Statics
//
SoftwareSerial *SoftwareSerial::active_object = 0;
unsigned char SoftwareSerial::_receive_buffer[_SS_MAX_RX_BUFF]; 
volatile uint8_t SoftwareSerial::_receive_buffer_tail = 0;
volatile uint8_t SoftwareSerial::_receive_buffer_head = 0;

typedef struct _DELAY_TABLE
{
  long baud;
  uint16_t rx_delay_centering;
  uint16_t rx_delay_intrabit;
  uint16_t rx_delay_stopbit;
  uint16_t tx_delay;
} DELAY_TABLE;

// rough delay estimation
static const DELAY_TABLE table[] =
{
  //baud    |rxcenter|rxintra |rxstop  |tx
  { 115200,   1,       98,       1,        99,   }, //Done but not good due to instruction cycle error
  //{ 74880,   69,       139,       62,      162,  }, // estimation
  { 57600,   100,       185,      1,       208,  }, // Done but not good due to instruction cycle error
  { 38400,   135,      298,      150,      317,  }, // Done
  { 19200,   290,      625,      560,      629,  }, // Done
  { 9600,    600,      1282,     1200,     1266, }, // Done
  { 4800,    1974,     2559,     2570,     2597, }, // Done
  { 2400,    4968,     5156,     5170,     5194, }, // Done 
  { 1200,    10144,    10331,    10350,    10388,}, // Done 
};

//
// Debugging
//
// This function generates a brief pulse
// for debugging or measuring on an oscilloscope.
inline void DebugPulse(uint8_t pin, uint8_t count)
{
#if _DEBUG
  GPIO_T *port = digitalPinToPort(pin);

  uint32_t val = port->DOUT;
  while (count--)
  {
    port->DOUT |= digitalPinToBitMask(pin); // set HIGH
    port->DOUT = val;
  }
#endif
}

//
// Private methods
//

/* static */ 
inline void SoftwareSerial::tunedDelay(uint32_t count) { 
    
	asm volatile(   
  
    "mov r3, %[loopsPerMicrosecond] \n\t" //load the initial loop counter
    "1: \n\t"
    "sub r3, r3, #1 \n\t"
    "bne 1b \n\t"
  
    ://empty output list
    :[loopsPerMicrosecond] "r" (count)
    :"r3", "cc" //clobber list
  );
  
}

// This function sets the current object as the "listening"
// one and returns true if it replaces another 
bool SoftwareSerial::listen()
{
  if (!_rx_delay_stopbit)
    return false;

  if (active_object != this)
  {
    if (active_object)
      active_object->stopListening();

    _buffer_overflow = false;
    _receive_buffer_head = _receive_buffer_tail = 0;
    active_object = this;

    setRxIntMsk(true);
    return true;
  }

  return false;
}

// Stop listening. Returns true if we were actually listening.
bool SoftwareSerial::stopListening()
{
  if (active_object == this)
  {
    setRxIntMsk(false);
    active_object = NULL;
    return true;
  }
  return false;
}

//
// The receive routine called by the interrupt handler
//
void SoftwareSerial::recv()
{
  uint8_t d = 0;

  // If RX line is high, then we don't see any start bit
  // so interrupt is probably not for us
  if (_inverse_logic ? rx_pin_read() : !rx_pin_read())
  {
    // Disable further interrupts during reception, this prevents
    // triggering another interrupt directly after we return, which can
    // cause problems at higher baudrates.
    setRxIntMsk(false);//__disable_irq();//
	
    // Wait approximately 1/2 of a bit width to "center" the sample
    tunedDelay(_rx_delay_centering);
    DebugPulse(_DEBUG_PIN2, 1);

    // Read each of the 8 bits
    for (uint8_t i=8; i > 0; --i)
    {
	  tunedDelay(_rx_delay_intrabit);
      d >>= 1;
      DebugPulse(_DEBUG_PIN2, 1);
      if (rx_pin_read())
        d |= 0x80;
    }

    if (_inverse_logic)
      d = ~d;

    // if buffer full, set the overflow flag and return
    uint8_t next = (_receive_buffer_tail + 1) % _SS_MAX_RX_BUFF;
    if (next != _receive_buffer_head)
    {
      // save new data in buffer: tail points to where byte goes
      _receive_buffer[_receive_buffer_tail] = d; // save new byte
      _receive_buffer_tail = next;
    } 
    else 
    {
      DebugPulse(_DEBUG_PIN1, 1);
      _buffer_overflow = true;
    }

	tunedDelay(_rx_delay_stopbit);
    DebugPulse(_DEBUG_PIN2, 1);

    // Re-enable interrupts when we're sure to be inside the stop bit
	setRxIntMsk(true);//__enable_irq();//

  }
}

uint32_t SoftwareSerial::rx_pin_read()
{
  return *_receivePortRegister & _receiveBitMask;
}

//
// Interrupt handling
//

/* static */
inline void SoftwareSerial::handle_interrupt()
{
  if (active_object)
  {
    active_object->recv();
  }
}

//
// Constructor
//
SoftwareSerial::SoftwareSerial(uint8_t receivePin, uint8_t transmitPin, bool inverse_logic /* = false */) : 
  _rx_delay_centering(0),
  _rx_delay_intrabit(0),
  _rx_delay_stopbit(0),
  _tx_delay(0),
  _buffer_overflow(false),
  _inverse_logic(inverse_logic)
{
  setTX(transmitPin);
  setRX(receivePin);

}

//
// Destructor
//
SoftwareSerial::~SoftwareSerial()
{
  end();
}

void SoftwareSerial::setTX(uint8_t tx)
{
  // First write, then set output. If we do this the other way around,
  // the pin would be output low for a short while before switching to
  // output hihg. Now, it is input with pullup for a short while, which
  // is fine. With inverse logic, either order is fine.
  //pinMode(tx, OUTPUT);
  uint32_t _transmitBit = 0;
  _transmitBit = GPIO_Desc[BoardToPinInfo[tx].pin].bit;
  *portModeRegister(digitalPinToPort(tx)) &= (0x3)<<(_transmitBit<<1);
  *portModeRegister(digitalPinToPort(tx)) |= (0x1)<<(_transmitBit<<1); // setting output
  
  digitalWrite(tx, _inverse_logic ? LOW : HIGH);
  _transmitPin = tx;

}

void SoftwareSerial::setRX(uint8_t rx)
{
  pinMode(rx, INPUT_PULLUP); // pullup for normal logic!
  //if (!_inverse_logic)
  // digitalWrite(rx, HIGH); 
  _receivePin = rx;
  GPIO_T * rxPort = digitalPinToPort(rx);
  _receivePortRegister = portInputRegister(rxPort);
  _receiveBitMask = digitalPinToBitMask(rx);

}

//
// Public methods
//

void SoftwareSerial::begin(long speed)
{
  _rx_delay_centering = _rx_delay_intrabit = _rx_delay_stopbit = _tx_delay = 0;

  for(uint8_t i = 0; i < sizeof(table)/sizeof(table[0]); ++i)
  {
    long baud = table[i].baud;
    if(baud == speed)
    {
      _rx_delay_centering = table[i].rx_delay_centering;
      _rx_delay_intrabit = table[i].rx_delay_intrabit;
      _rx_delay_stopbit = table[i].rx_delay_stopbit;
      _tx_delay = table[i].tx_delay;
      break;
    }
  }
  
  attachInterrupt(_receivePin, this->handle_interrupt, CHANGE);
  
  uint8_t u32Pin = GPIO_Desc[BoardToPinInfo[_receivePin].pin].bit;
  _pimd_maskreg = &( (digitalPinToPort(_receivePin))-> IMD);
  _pien_maskreg = &( (digitalPinToPort(_receivePin))-> IEN); 
  _pimd_en_maskvalue = (((GPIO_INT_BOTH_EDGE >> 24) & 0xFFUL) << u32Pin);
  _pien_en_maskvalue = ((GPIO_INT_BOTH_EDGE & 0xFFFFFFUL) << u32Pin);
  _pimd_dis_maskvalue = ~(1UL << u32Pin);
  _pien_dis_maskvalue = ~((0x00010001UL) << u32Pin);
  
#if _DEBUG
  pinMode(_DEBUG_PIN1, OUTPUT);
  pinMode(_DEBUG_PIN2, OUTPUT);
#endif

  listen();
  tunedDelay(_tx_delay);

}

void SoftwareSerial::setRxIntMsk(bool enable)
{
    if (enable){
		*_pimd_maskreg |= _pimd_en_maskvalue;
		*_pien_maskreg |= _pien_en_maskvalue;
	}
		//GPIO_EnableInt(_receivePortRegister,_receiveBitMask,GPIO_INT_BOTH_EDGE);
  
    else{
		*_pimd_maskreg &= _pimd_dis_maskvalue;
		*_pien_maskreg &= _pien_dis_maskvalue;
	}
		//GPIO_DisableInt(_receivePortRegister,_receiveBitMask);
}

void SoftwareSerial::end()
{
  stopListening();
}


// Read data from buffer
int SoftwareSerial::read()
{
  if (!isListening())
    return -1;

  // Empty buffer?
  if (_receive_buffer_head == _receive_buffer_tail)
    return -1;

  // Read from "head"
  uint8_t d = _receive_buffer[_receive_buffer_head]; // grab next byte
  _receive_buffer_head = (_receive_buffer_head + 1) % _SS_MAX_RX_BUFF;
  return d;
}

int SoftwareSerial::available()
{
  if (!isListening())
    return 0;

  return (_receive_buffer_tail + _SS_MAX_RX_BUFF - _receive_buffer_head) % _SS_MAX_RX_BUFF;
}

size_t SoftwareSerial::write(uint8_t b)
{
  // By declaring these as local variables, the compiler will put them
  // in registers _before_ disabling interrupts and entering the
  // critical timing sections below, which makes it a lot easier to
  // verify the cycle timings

  uint32_t reg_mask = digitalPinToBitMask(_transmitPin);
  uint32_t inv_mask = ~reg_mask;
  GPIO_T* port = digitalPinToPort(_transmitPin); 
  bool inv = _inverse_logic;
  uint16_t delay = _tx_delay;
   
  if(inv)
	  b = ~b;
  
  __disable_irq();//cli();  // turn off interrupts for a clean txmit
  
  // Write the start bit
  if (inv)
    port->DOUT |= reg_mask;
  else
    port->DOUT &= inv_mask;
  
  tunedDelay(delay);

  // Write each of the 8 bits
  for (uint8_t i = 8; i > 0; --i)
  {
    if (b & 1) // choose bit
      port->DOUT |= reg_mask; // send 1 //(GPIO_Desc[_transmitPin].P)->DOUT |= GPIO_Desc[_transmitPin].bit;
    else
      port->DOUT &= inv_mask; // send 0 //(GPIO_Desc[_transmitPin].P)->DOUT &= ~GPIO_Desc[_transmitPin].bit;

    tunedDelay(delay);
    b >>= 1;
  }

  // restore pin to natural state
  if (inv)
    port->DOUT &= inv_mask;
  else
    port->DOUT |= reg_mask;
	
	__enable_irq(); // turn interrupts back on
  tunedDelay(delay);
 
  return 1;
}

void SoftwareSerial::flush()
{
  if (!isListening())
    return;

  __disable_irq();
  _receive_buffer_head = _receive_buffer_tail = 0;
  __enable_irq();
}

int SoftwareSerial::peek()
{
  if (!isListening())
    return -1;

  // Empty buffer?
  if (_receive_buffer_head == _receive_buffer_tail)
    return -1;

  // Read from "head"
  return _receive_buffer[_receive_buffer_head];
}
