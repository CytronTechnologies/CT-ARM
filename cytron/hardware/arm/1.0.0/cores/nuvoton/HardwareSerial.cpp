/******************************************************************************
 * The MIT License
 *
 * Copyright (c) 2010 Perry Hung.
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
 * @file HardwareSerial.cpp
 * @brief Wirish serial port implementation.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Arduino.h"

#if(UART_MAX_COUNT>0)
ring_buffer rx_buffer = { { 0 }, 0, 0};
HardwareSerial Serial(UART_Desc[0].U,0,CLK_CLKSEL1_UART_S_HXT,1,UART_Desc[0].irq,&rx_buffer);
#endif

#if(UART_MAX_COUNT>1)
ring_buffer rx_buffer1 = { { 0 }, 0, 0};
HardwareSerial Serial1(UART_Desc[1].U,1,CLK_CLKSEL1_UART_S_HXT,1,UART_Desc[1].irq,&rx_buffer1);
#endif

#if(UART_MAX_COUNT>2)
ring_buffer rx_buffer2 = { { 0 }, 0, 0};
HardwareSerial Serial2(UART_Desc[2].U,2,CLK_CLKSEL1_UART_S_HXT,1,UART_Desc[2].irq,&rx_buffer2);
#endif

#if(UART_MAX_COUNT>3)
ring_buffer rx_buffer3 = { { 0 }, 0, 0};
HardwareSerial Serial3(UART_Desc[3].U,3,CLK_CLKSEL1_UART_S_HXT,1,UART_Desc[3].irq,&rx_buffer3);
#endif

#if(UART_MAX_COUNT>4)
ring_buffer rx_buffer4 = { { 0 }, 0, 0};
HardwareSerial Serial4(UART_Desc[4].U,4,CLK_CLKSEL1_UART_S_HXT,1,UART_Desc[4].irq,&rx_buffer4);
#endif

#if(UART_MAX_COUNT>5)
ring_buffer rx_buffer5 = { { 0 }, 0, 0};
HardwareSerial Serial5(UART_Desc[5].U,5,CLK_CLKSEL1_UART_S_HXT,1,UART_Desc[5].irq,&rx_buffer5);
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if(UART_MAX_COUNT>0)
void UART02_IRQHandler(void)
{	 
	while(UART_GET_INT_FLAG(UART0,UART_IER_RDA_IEN_Msk))
	{
		unsigned char c = UART0->RBR;
		// if we should be storing the received character into the location
		// just before the tail (meaning that the head would advance to the
		// current location of the tail), we're about to overflow the buffer
		// and so we don't write the character or advance the head.
		int i = (unsigned int)(rx_buffer.head + 1) % SERIAL_BUFFER_SIZE;
		if (i != rx_buffer.tail) {
			rx_buffer.buffer[rx_buffer.head] = c;
			rx_buffer.head = i;
		}
		else{//read byte but discard it
			UART0->RBR;
		}
	}
#if(UART_MAX_COUNT>2)	
	while(UART_GET_INT_FLAG(UART2,UART_IER_RDA_IEN_Msk))
	{
		unsigned char c = UART2->RBR;
		int i = (unsigned int)(rx_buffer2.head + 1) % SERIAL_BUFFER_SIZE;
		if (i != rx_buffer2.tail) {
			rx_buffer2.buffer[rx_buffer2.head] = c;
			rx_buffer2.head = i;
		}
		else{
			UART2->RBR;
		}
	}
#endif		
}
#endif

#if(UART_MAX_COUNT>1)

void UART1_IRQHandler(void)
{
	while(UART_GET_INT_FLAG(UART1,UART_IER_RDA_IEN_Msk))
	{
		unsigned char c = UART1->RBR;
		int i = (unsigned int)(rx_buffer1.head + 1) % SERIAL_BUFFER_SIZE;
		if (i != rx_buffer1.tail) {
			rx_buffer1.buffer[rx_buffer1.head] = c;
			rx_buffer1.head = i;
		}
		else{
			UART1->RBR;
		}
	}
		
}
#endif

#if(UART_MAX_COUNT>3)

void UART3_IRQHandler(void)
{
	while(UART_GET_INT_FLAG(UART3,UART_IER_RDA_IEN_Msk))
	{
		unsigned char c = UART3->RBR;
		int i = (unsigned int)(rx_buffer3.head + 1) % SERIAL_BUFFER_SIZE;
		if (i != rx_buffer3.tail) {
			rx_buffer3.buffer[rx_buffer3.head] = c;
			rx_buffer3.head = i;
		}
		else{
			UART3->RBR;
		}
	}
		
}
#endif

#if(UART_MAX_COUNT>4)

void UART4_IRQHandler(void)
{
	while(UART_GET_INT_FLAG(UART4,UART_IER_RDA_IEN_Msk))
	{
		unsigned char c = UART4->RBR;
		int i = (unsigned int)(rx_buffer4.head + 1) % SERIAL_BUFFER_SIZE;
		if (i != rx_buffer4.tail) {
			rx_buffer4.buffer[rx_buffer4.head] = c;
			rx_buffer4.head = i;
		}
		else{
			UART4->RBR;
		}
	}
		
}
#endif

#if(UART_MAX_COUNT>5)

void UART5_IRQHandler(void)
{
	while(UART_GET_INT_FLAG(UART5,UART_IER_RDA_IEN_Msk))
	{
		unsigned char c = UART5->RBR;
		int i = (unsigned int)(rx_buffer5.head + 1) % SERIAL_BUFFER_SIZE;
		if (i != rx_buffer5.tail) {
			rx_buffer5.buffer[rx_buffer5.head] = c;
			rx_buffer5.head = i;
		}
		else{
			UART5->RBR;
		}
	}		
}
#endif

#ifdef __cplusplus
}
#endif

#if(UART_MAX_COUNT>0)
void serialEvent() __attribute__((weak));
#endif

#if(UART_MAX_COUNT>1)
void serialEvent1() __attribute__((weak));
#endif

#if(UART_MAX_COUNT>2)
void serialEvent2() __attribute__((weak));
#endif

#if(UART_MAX_COUNT>3)
void serialEvent3() __attribute__((weak));
#endif

#if(UART_MAX_COUNT>4)
void serialEvent4() __attribute__((weak));
#endif

#if(UART_MAX_COUNT>5)
void serialEvent5() __attribute__((weak));
#endif

void serialEventRun(void)
{
	#if(UART_MAX_COUNT>0)
    if (Serial.available()) serialEvent();
    #endif
   	
    #if(UART_MAX_COUNT>1)
    if (Serial1.available()) serialEvent1();
    #endif
	
	#if(UART_MAX_COUNT>2)
    if (Serial2.available()) serialEvent2();
    #endif
	
	#if(UART_MAX_COUNT>3)
    if (Serial3.available()) serialEvent3();
    #endif
	
	#if(UART_MAX_COUNT>4)
    if (Serial4.available()) serialEvent4();
    #endif
	
	#if(UART_MAX_COUNT>5)
    if (Serial5.available()) serialEvent5();
    #endif
      	
}

HardwareSerial::HardwareSerial(UART_T *uart_device,
															 uint32_t u32Idx,															                              
                               uint32_t u32ClkSrc,
                               uint32_t u32ClkDiv,
                               IRQn_Type u32IrqId,
                               ring_buffer *rx_buffer) {
    this->uart_device = uart_device;
    this->u32Idx = u32Idx;
    this->u32ClkSrc = u32ClkSrc;
    this->u32ClkDiv = u32ClkDiv;
    this->u32IrqId = u32IrqId;
    this->_rx_buffer = rx_buffer;
}

/*
 * Set up/tear down
 */

void HardwareSerial::begin(uint32_t baud) {
    
    UART_Config(UART_Desc[u32Idx]);
		/* Enable IP clock */       
    CLK_EnableModuleClock(UART_Desc[u32Idx].module);    	
   	
   	/* Select IP clock source and clock divider */
		CLK_SetModuleClock(UART_Desc[u32Idx].module,u32ClkSrc,CLK_CLKDIV_UART(u32ClkDiv));

    /* Reset IP */
    //SYS_ResetModule(UART_Desc[u32Idx].module);
    
    /* Enable Interrupt */
    UART_ENABLE_INT(uart_device,UART_IER_RDA_IEN_Msk);
    NVIC_EnableIRQ(u32IrqId);
    
    /* Configure UART and set UART Baudrate */
    UART_Open(uart_device, baud);

}

void HardwareSerial::end(void) {
    UART_Close(uart_device);
}

/*
 * I/O
 */

int HardwareSerial::read(void) {        
  // if the head isn't ahead of the tail, we don't have any characters
  if (_rx_buffer->head == _rx_buffer->tail) {
    return -1;
  } else {
    unsigned char c = _rx_buffer->buffer[_rx_buffer->tail];
    _rx_buffer->tail = (unsigned int)(_rx_buffer->tail + 1) % SERIAL_BUFFER_SIZE;
    return c;
  }
}

int HardwareSerial::available(void) {    
   return (unsigned int)(SERIAL_BUFFER_SIZE + _rx_buffer->head - _rx_buffer->tail) % SERIAL_BUFFER_SIZE;
}

size_t HardwareSerial::write(const uint8_t ch) {

	while(uart_device->FSR & UART_FSR_TX_FULL_Msk);
	uart_device->THR = ch;
	/*if(ch == '\n'){
	//	while(uart_device->FSR & UART_FSR_TX_FULL_Msk);
	//	uart_device->THR = '\r';
	}*/
	return 1;
}

void HardwareSerial::flush(void) {
    
}


int HardwareSerial::peek( void ) {	
  if (_rx_buffer->head == _rx_buffer->tail) {
    return -1;
  } else {
    return _rx_buffer->buffer[_rx_buffer->tail];
  }
}
