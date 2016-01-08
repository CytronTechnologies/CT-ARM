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
//#include <avr/pgmspace.h>
#include <Arduino.h>
#include <SoftwareSerial.h>
//#include <util/delay_basic.h>

//
// Statics
//
SoftwareSerial *SoftwareSerial::active_object = 0;
char SoftwareSerial::_receive_buffer[_SS_MAX_RX_BUFF]; 
volatile uint8_t SoftwareSerial::_receive_buffer_tail = 0;
volatile uint8_t SoftwareSerial::_receive_buffer_head = 0;

//
// Debugging
//
// This function generates a brief pulse
// for debugging or measuring on an oscilloscope.
inline void DebugPulse(uint8_t pin, uint8_t count)
{
#if _DEBUG
  volatile uint8_t *pport = portOutputRegister(digitalPinToPort(pin));

  uint8_t val = *pport;
  while (count--)
  {
    *pport = val | digitalPinToBitMask(pin);
    *pport = val;
  }
#endif
}

//
// Private methods
//

/* static */ 
inline void SoftwareSerial::tunedDelay(uint16_t delay) { 
  //_delay_loop_2(delay);
  delayMicroseconds(delay);
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
/*
#if GCC_VERSION < 40302
// Work-around for avr-gcc 4.3.0 OSX version bug
// Preserve the registers that the compiler misses
// (courtesy of Arduino forum user *etracer*)
  asm volatile(
    "push r18 \n\t"
    "push r19 \n\t"
    "push r20 \n\t"
    "push r21 \n\t"
    "push r22 \n\t"
    "push r23 \n\t"
    "push r26 \n\t"
    "push r27 \n\t"
    ::);
#endif  
*/
  uint8_t d = 0;

  // If RX line is high, then we don't see any start bit
  // so interrupt is probably not for us
  if (_inverse_logic ? rx_pin_read() : !rx_pin_read())
  {
    // Disable further interrupts during reception, this prevents
    // triggering another interrupt directly after we return, which can
    // cause problems at higher baudrates.
    setRxIntMsk(false);

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

    // skip the stop bit
    tunedDelay(_rx_delay_stopbit);
    DebugPulse(_DEBUG_PIN1, 1);

    // Re-enable interrupts when we're sure to be inside the stop bit
    setRxIntMsk(true);

  }
/*
#if GCC_VERSION < 40302
// Work-around for avr-gcc 4.3.0 OSX version bug
// Restore the registers that the compiler misses
  asm volatile(
    "pop r27 \n\t"
    "pop r26 \n\t"
    "pop r23 \n\t"
    "pop r22 \n\t"
    "pop r21 \n\t"
    "pop r20 \n\t"
    "pop r19 \n\t"
    "pop r18 \n\t"
    ::);
#endif
*/
}

uint8_t SoftwareSerial::rx_pin_read()
{
  return ((GPIO_Desc[_receivePin].P)->PIN & GPIO_Desc[_receivePin].bit)? HIGH:LOW;
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

/*
#if defined(PCINT0_vect)
ISR(PCINT0_vect)
{
  SoftwareSerial::handle_interrupt();
}
#endif

#if defined(PCINT1_vect)
ISR(PCINT1_vect, ISR_ALIASOF(PCINT0_vect));
#endif

#if defined(PCINT2_vect)
ISR(PCINT2_vect, ISR_ALIASOF(PCINT0_vect));
#endif

#if defined(PCINT3_vect)
ISR(PCINT3_vect, ISR_ALIASOF(PCINT0_vect));
#endif
*/

//
// Constructor
//
SoftwareSerial::SoftwareSerial(uint8_t receivePin, uint8_t transmitPin, bool inverse_logic /* = false */) : 
  _rx_delay_centering(0),
  _rx_delay_intrabit(0),
  _rx_delay_stopbit(0),
  _tx_delay(0),
  _buffer_overflow(false),
  _inverse_logic(inverse_logic),
  _receiveBitMask(0)
{
  setTX(transmitPin);
  setRX(receivePin);
  
  
  //initialise callback function
  if (_receivePortRegister == PA )
	  callbacksPA[_receiveBitMask] = NULL;
  
  else if (_receivePortRegister == PB )
	  callbacksPB[_receiveBitMask] = NULL;

  else if (_receivePortRegister == PC )
	  callbacksPC[_receiveBitMask] = NULL;
		
  else if (_receivePortRegister == PD )
	  callbacksPD[_receiveBitMask] = NULL;
		
  else if (_receivePortRegister == PE )
	  callbacksPE[_receiveBitMask] = NULL;
		
  else if (_receivePortRegister == PF )
	  callbacksPF[_receiveBitMask] = NULL;
   
  //Enable IRQn
  NVIC_EnableIRQ(GPAB_IRQn);
  NVIC_EnableIRQ(GPCDEF_IRQn);

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
  pinMode(tx, OUTPUT);
  digitalWrite(tx, _inverse_logic ? LOW : HIGH);
  _transmitPin = BoardToPinInfo[tx].pin;

}

void SoftwareSerial::setRX(uint8_t rx)
{
  pinMode(rx, INPUT_PULLUP); // pullup for normal logic!
  //if (!_inverse_logic)
  // digitalWrite(rx, HIGH);  
  _receivePin = BoardToPinInfo[rx].pin;
  
  //get information of RX pin
  _receivePortRegister = GPIO_Desc[_receivePin].P;
  uint32_t mask = GPIO_Desc[_receivePin].bit;
  _receiveBitMask = 0;
  uint32_t t;
	
  for (t = mask; t>1; t>>=1, _receiveBitMask++);

}

//
// Public methods
//

void SoftwareSerial::begin(long speed)
{
  _rx_delay_centering = _rx_delay_intrabit = _rx_delay_stopbit = _tx_delay = 0;

  _tx_delay = round(1000000 / speed) + 8;
  
  if(_tx_delay > 10) _rx_delay_centering= _tx_delay / 2;
  else _rx_delay_centering = _tx_delay;
  
  _rx_delay_intrabit = _rx_delay_centering;
  _rx_delay_stopbit = _rx_delay_centering;
  
  
  if (_receivePortRegister == PA )
	  callbacksPA[_receiveBitMask] = this->handle_interrupt;
  
  else if (_receivePortRegister == PB )
	  callbacksPB[_receiveBitMask] = this->handle_interrupt;

  else if (_receivePortRegister == PC )
	  callbacksPC[_receiveBitMask] = this->handle_interrupt;
		
  else if (_receivePortRegister == PD )
	  callbacksPD[_receiveBitMask] = this->handle_interrupt;
		
  else if (_receivePortRegister == PE )
	  callbacksPE[_receiveBitMask] = this->handle_interrupt;
		
  else if (_receivePortRegister == PF )
	  callbacksPF[_receiveBitMask] = this->handle_interrupt;

#if _DEBUG
  pinMode(_DEBUG_PIN1, OUTPUT);
  pinMode(_DEBUG_PIN2, OUTPUT);
#endif

  listen();
  
  tunedDelay(1000000); //TODO: must be tunedDelay, don't know why, need to test

}

void SoftwareSerial::setRxIntMsk(bool enable)
{
    if (enable)
      GPIO_EnableInt(_receivePortRegister,_receiveBitMask,GPIO_INT_BOTH_EDGE);
  
    else
      GPIO_DisableInt(_receivePortRegister,_receiveBitMask);
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
  
  //bool inv = _inverse_logic;
  
  __disable_irq();//cli();  // turn off interrupts for a clean txmit
  
  (GPIO_Desc[_transmitPin].P)->DOUT |= GPIO_Desc[_transmitPin].bit; //high
  tunedDelay(_tx_delay);
  
  // Write the start bit
 
  (GPIO_Desc[_transmitPin].P)->DOUT &= ~GPIO_Desc[_transmitPin].bit; //low
  
  tunedDelay(_tx_delay);

  // Write each of the 8 bits
  for (uint8_t i = 8; i > 0; --i)
  {
    if (b & 1) // choose bit
      (GPIO_Desc[_transmitPin].P)->DOUT |= GPIO_Desc[_transmitPin].bit;//*reg |= reg_mask; // send 1
    else
      (GPIO_Desc[_transmitPin].P)->DOUT &= ~GPIO_Desc[_transmitPin].bit;//*reg &= inv_mask; // send 0

    tunedDelay(_tx_delay);
    b >>= 1;
  }

  // restore pin to natural state
  (GPIO_Desc[_transmitPin].P)->DOUT |= GPIO_Desc[_transmitPin].bit; //high
  
  tunedDelay(_tx_delay);
  
  __enable_irq(); // turn interrupts back on
  
  return 1;
}

void SoftwareSerial::flush()
{
  if (!isListening())
    return;

  //uint8_t oldSREG = SREG;
  __disable_irq();//cli();
  _receive_buffer_head = _receive_buffer_tail = 0;
  __enable_irq();
  //SREG = oldSREG;
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
