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
#include "Pins_arduino.h"

const BoardToPin BoardToPinInfo[] = {
  {13, UART_TYPE, 0},  // 0:  PB.0  UART0_RXD UART_RXD
  {14, UART_TYPE, 0},  // 1:  PB.1  UART0_TXD UART_TXD
  {2,  PWM_TYPE,  0},  // 2:  PF.5  PWM1_CH5  PWM0
  {3,  PWM_TYPE,  1},  // 3:  PF.4  PWM1_CH4  PWM1
  {4,  PWM_TYPE,  2},  // 4:  PA.11 PWM1_CH3  PWM2
  {5,  PWM_TYPE,  3},  // 5:  PA.10 PWM1_CH2  PWM3
  {17, PWM_TYPE,  0},  // 6:  PD.6  BPWM1_CH1 BPWM0
  {18, PWM_TYPE,  1},  // 7:  PD.7  BPWM1_CH0 BPWM1

  {19, PWM_TYPE,  2},  // 8:  PD.14 BPWM0_CH5 BPWM2
  {20, PWM_TYPE,  3},  // 9:  PD.15 BPWM0_CH4 BPWM3
  {28, PWM_TYPE,  4},  // 10: PA.12 PWM0_CH0  PWM4
  {27, PWM_TYPE,  5},  // 11: PA.13 PWM0_CH1  PWM5
  {25, PWM_TYPE,  6},  // 12: PA.15 PWM0_CH2  PWM6
  {26, PWM_TYPE,  7},  // 13: PA.14 PWM0_CH3  PWM7

  {22, SPI_TYPE,  0},  // 14: PC.2  SPI0_MISO MISO
  {23, SPI_TYPE,  0},  // 15: PC.1  SPI0_CLK  SCK
  {21, SPI_TYPE,  0},  // 16: PC.3  SPI0_MOSI MOSI
  {24, SPI_TYPE,  0},  // 17: PC.0  SPI0_SS0  SS

  {32, ADC_TYPE,  0},  // 18: PA.0  ADC_CH0   A0
  {33, ADC_TYPE,  1},  // 19: PA.1  ADC_CH1   A1
  {34, ADC_TYPE,  2},  // 20: PA.2  ADC_CH2   A2
  {35, ADC_TYPE,  3},  // 21: PA.3  ADC_CH3   A3
  {37, ADC_TYPE,  4},  // 22: PA.5  ADC_CH5   A4
  {38, ADC_TYPE,  5},  // 23: PA.6  ADC_CH6   A5

  {42, I2C_TYPE,  0},  // 24: PC.6  I2C0_SDA  I2C_SDA
  {41, I2C_TYPE,  0},  // 25: PC.7  I2C0_SCL  I2C_SCL

  {32, PWM_TYPE,  8},  // 26: PA.0  PWM0_CH4  PWM8
  {33, PWM_TYPE,  9},  // 27: PA.1  PWM0_CH5  PWM9
  {34, PWM_TYPE,  10}, // 28: PA.2  PWM1_CH0  PWM10
  {35, PWM_TYPE,  11}, // 29: PA.3  PWM1_CH1  PWM11
};

const GPIOPinDescription GPIO_Desc[] = {
  {NULL, NULL, {NULL, NULL, NULL, NULL, NULL, NULL, NULL}},                                                                                   // 0
  {PB, 12, {(uint32_t)&SYS->GPB_MFP, SYS_GPB_MFP_PB12_Msk, NULL, NULL, NULL,                  NULL,                  SYS_GPB_MFP_PB12_GPIO}}, // 1
  {PF, 5,  {(uint32_t)&SYS->GPF_MFP, SYS_GPF_MFP_PF5_Msk,  NULL, NULL, SYS_ALT_MFP3_PF5_Msk,  NULL,                  SYS_GPF_MFP_PF5_GPIO}},  // 2
  {PF, 4,  {(uint32_t)&SYS->GPF_MFP, SYS_GPF_MFP_PF4_Msk,  NULL, NULL, SYS_ALT_MFP3_PF4_Msk,  NULL,                  SYS_GPF_MFP_PF4_GPIO}},  // 3
  {PA, 11, {(uint32_t)&SYS->GPA_MFP, SYS_GPA_MFP_PA11_Msk, NULL, NULL, SYS_ALT_MFP3_PA11_Msk, NULL,                  SYS_GPA_MFP_PA11_GPIO}}, // 4
  {PA, 10, {(uint32_t)&SYS->GPA_MFP, SYS_GPA_MFP_PA10_Msk, NULL, NULL, SYS_ALT_MFP3_PA10_Msk, NULL,                  SYS_GPA_MFP_PA10_GPIO}}, // 5
  {PA, 9,  {(uint32_t)&SYS->GPA_MFP, SYS_GPA_MFP_PA9_Msk,  NULL, NULL, NULL,                  NULL,                  SYS_GPA_MFP_PA9_GPIO}},  // 6
  {PA, 8,  {(uint32_t)&SYS->GPA_MFP, SYS_GPA_MFP_PA8_Msk,  NULL, NULL, NULL,                  NULL,                  SYS_GPA_MFP_PA8_GPIO}},  // 7
  {PB, 4,  {(uint32_t)&SYS->GPB_MFP, SYS_GPB_MFP_PB4_Msk,  NULL, NULL, NULL,                  NULL,                  SYS_GPB_MFP_PB4_GPIO}},  // 8
  {PB, 5,  {(uint32_t)&SYS->GPB_MFP, SYS_GPB_MFP_PB5_Msk,  NULL, NULL, NULL,                  NULL,                  SYS_GPB_MFP_PB5_GPIO}},  // 9
  {NULL, NULL, {NULL, NULL, NULL, NULL, NULL, NULL, NULL}},                                                                                   // 10
  {NULL, NULL, {NULL, NULL, NULL, NULL, NULL, NULL, NULL}},                                                                                   // 11
  {NULL, NULL, {NULL, NULL, NULL, NULL, NULL, NULL, NULL}},                                                                                   // 12

  {PB, 0,  {(uint32_t)&SYS->GPB_MFP, SYS_GPB_MFP_PB0_Msk,  NULL, NULL, NULL,                  NULL,                  SYS_GPB_MFP_PB0_GPIO}},  // 13
  {PB, 1,  {(uint32_t)&SYS->GPB_MFP, SYS_GPB_MFP_PB1_Msk,  NULL, NULL, NULL,                  NULL,                  SYS_GPB_MFP_PB1_GPIO}},  // 14
  {PB, 2,  {(uint32_t)&SYS->GPB_MFP, SYS_GPB_MFP_PB2_Msk,  NULL, NULL, NULL,                  NULL,                  SYS_GPB_MFP_PB2_GPIO}},  // 15
  {PB, 3,  {(uint32_t)&SYS->GPB_MFP, SYS_GPB_MFP_PB3_Msk,  NULL, NULL, NULL,                  NULL,                  SYS_GPB_MFP_PB3_GPIO}},  // 16
  {PD, 6,  {(uint32_t)&SYS->GPD_MFP, SYS_GPD_MFP_PD6_Msk,  NULL, NULL, SYS_ALT_MFP3_PD6_Msk,  NULL,                  SYS_GPD_MFP_PD6_GPIO}},  // 17
  {PD, 7,  {(uint32_t)&SYS->GPD_MFP, SYS_GPD_MFP_PD7_Msk,  NULL, NULL, SYS_ALT_MFP3_PD7_Msk,  NULL,                  SYS_GPD_MFP_PD7_GPIO}},  // 18
  {PD, 14, {(uint32_t)&SYS->GPD_MFP, SYS_GPD_MFP_PD14_Msk, NULL, NULL, SYS_ALT_MFP3_PD14_Msk, NULL,                  SYS_GPD_MFP_PD14_GPIO}}, // 19
  {PD, 15, {(uint32_t)&SYS->GPD_MFP, SYS_GPD_MFP_PD15_Msk, NULL, NULL, SYS_ALT_MFP3_PD15_Msk, NULL,                  SYS_GPD_MFP_PD15_GPIO}}, // 20
  {PC, 3,  {(uint32_t)&SYS->GPC_MFP, SYS_GPC_MFP_PC3_Msk,  NULL, NULL, NULL,                  NULL,                  SYS_GPC_MFP_PC3_GPIO}},  // 21
  {PC, 2,  {(uint32_t)&SYS->GPC_MFP, SYS_GPC_MFP_PC2_Msk,  NULL, NULL, NULL,                  NULL,                  SYS_GPC_MFP_PC2_GPIO}},  // 22
  {PC, 1,  {(uint32_t)&SYS->GPC_MFP, SYS_GPC_MFP_PC1_Msk,  NULL, NULL, NULL,                  NULL,                  SYS_GPC_MFP_PC1_GPIO}},  // 23
  {PC, 0,  {(uint32_t)&SYS->GPC_MFP, SYS_GPC_MFP_PC0_Msk,  NULL, NULL, NULL,                  NULL,                  SYS_GPC_MFP_PC0_GPIO}},  // 24

  {PA, 15, {(uint32_t)&SYS->GPA_MFP, SYS_GPA_MFP_PA15_Msk, NULL, NULL, NULL,                  NULL,                  SYS_GPA_MFP_PA15_GPIO}}, // 25
  {PA, 14, {(uint32_t)&SYS->GPA_MFP, SYS_GPA_MFP_PA14_Msk, NULL, NULL, NULL,                  NULL,                  SYS_GPA_MFP_PA14_GPIO}}, // 26
  {PA, 13, {(uint32_t)&SYS->GPA_MFP, SYS_GPA_MFP_PA13_Msk, NULL, NULL, NULL,                  SYS_ALT_MFP4_PA13_Msk, SYS_GPA_MFP_PA13_GPIO}}, // 27
  {PA, 12, {(uint32_t)&SYS->GPA_MFP, SYS_GPA_MFP_PA12_Msk, NULL, NULL, NULL,                  SYS_ALT_MFP4_PA12_Msk, SYS_GPA_MFP_PA12_GPIO}}, // 28
  {PF, 7,  {(uint32_t)&SYS->GPF_MFP, SYS_GPF_MFP_PF7_Msk,  NULL, NULL, NULL,                  NULL,                  SYS_GPF_MFP_PF7_GPIO}},  // 29
  {PF, 6,  {(uint32_t)&SYS->GPF_MFP, SYS_GPF_MFP_PF6_Msk,  NULL, NULL, NULL,                  NULL,                  SYS_GPF_MFP_PF6_GPIO}},  // 30
  {NULL, NULL, {NULL, NULL, NULL, NULL, NULL, NULL, NULL}},                                                                                   // 31
  {PA, 0,  {(uint32_t)&SYS->GPA_MFP, SYS_GPA_MFP_PA0_Msk,  NULL, NULL, SYS_ALT_MFP3_PA0_Msk,  SYS_ALT_MFP4_PA0_Msk,  SYS_GPA_MFP_PA0_GPIO}},  // 32
  {PA, 1,  {(uint32_t)&SYS->GPA_MFP, SYS_GPA_MFP_PA1_Msk,  NULL, NULL, SYS_ALT_MFP3_PA1_Msk,  SYS_ALT_MFP4_PA1_Msk,  SYS_GPA_MFP_PA1_GPIO}},  // 33
  {PA, 2,  {(uint32_t)&SYS->GPA_MFP, SYS_GPA_MFP_PA2_Msk,  NULL, NULL, SYS_ALT_MFP3_PA2_Msk,  SYS_ALT_MFP4_PA2_Msk,  SYS_GPA_MFP_PA2_GPIO}},  // 34
  {PA, 3,  {(uint32_t)&SYS->GPA_MFP, SYS_GPA_MFP_PA3_Msk,  NULL, NULL, SYS_ALT_MFP3_PA3_Msk,  SYS_ALT_MFP4_PA3_Msk,  SYS_GPA_MFP_PA3_GPIO}},  // 35
  {PA, 4,  {(uint32_t)&SYS->GPA_MFP, SYS_GPA_MFP_PA4_Msk,  NULL, NULL, NULL,                  NULL,                  SYS_GPA_MFP_PA4_GPIO}},  // 36

  {PA, 5,  {(uint32_t)&SYS->GPA_MFP, SYS_GPA_MFP_PA5_Msk,  NULL, NULL, NULL,                  SYS_ALT_MFP4_PA5_Msk,  SYS_GPA_MFP_PA5_GPIO}},  // 37
  {PA, 6,  {(uint32_t)&SYS->GPA_MFP, SYS_GPA_MFP_PA6_Msk,  NULL, NULL, NULL,                  SYS_ALT_MFP4_PA6_Msk,  SYS_GPA_MFP_PA6_GPIO}},  // 38
  {PA, 7,  {(uint32_t)&SYS->GPA_MFP, SYS_GPA_MFP_PA7_Msk,  NULL, NULL, NULL,                  NULL,                  SYS_GPA_MFP_PA7_GPIO}},  // 39
  {NULL, NULL, {NULL, NULL, NULL, NULL, NULL, NULL, NULL}},                                                                                   // 40
  {PC, 7,  {(uint32_t)&SYS->GPC_MFP, SYS_GPC_MFP_PC7_Msk,  NULL, NULL, SYS_ALT_MFP3_PC7_Msk,  SYS_ALT_MFP4_PC7_Msk,  SYS_GPC_MFP_PC7_GPIO}},  // 41
  {PC, 6,  {(uint32_t)&SYS->GPC_MFP, SYS_GPC_MFP_PC6_Msk,  NULL, NULL, SYS_ALT_MFP3_PC6_Msk,  SYS_ALT_MFP4_PC6_Msk,  SYS_GPC_MFP_PC6_GPIO}},  // 42
  {PB, 15, {(uint32_t)&SYS->GPB_MFP, SYS_GPB_MFP_PB15_Msk, NULL, NULL, NULL,                  NULL,                  SYS_GPB_MFP_PB15_GPIO}}, // 43
  {PF, 0,  {(uint32_t)&SYS->GPF_MFP, SYS_GPF_MFP_PF0_Msk,  NULL, NULL, NULL,                  NULL,                  SYS_GPF_MFP_PF0_GPIO}},  // 44
  {PF, 1,  {(uint32_t)&SYS->GPF_MFP, SYS_GPF_MFP_PF1_Msk,  NULL, NULL, NULL,                  NULL,                  SYS_GPF_MFP_PF1_GPIO}},  // 45
  {NULL, NULL, {NULL, NULL, NULL, NULL, NULL, NULL, NULL}},                                                                                   // 46
  {PF, 8,  {(uint32_t)&SYS->GPF_MFP, SYS_GPF_MFP_PF8_Msk,  NULL, NULL, NULL,                  NULL,                  SYS_GPF_MFP_PF8_GPIO}},  // 47
  {PB, 8,  {(uint32_t)&SYS->GPB_MFP, SYS_GPB_MFP_PB8_Msk,  NULL, NULL, NULL,                  NULL,                  SYS_GPB_MFP_PB8_GPIO}},  // 48
};

BPWMPinDescription BPWM_Desc[] = {
//{BPWMx, BPWMx_MODULE, BPWMx_IRQn, Chan, freq, {Pin, SYS_GPx_MFP_Pxx_PWMx_CHx,   ...,  ...,  ...,  ...}}
  {BPWM1, BPWM1_MODULE, BPWM1_IRQn, 1,    500,  {17,  SYS_GPD_MFP_PD6_BPWM1_CH1,  NULL, NULL, SYS_ALT_MFP3_PD6_BPWM1_CH1,  NULL}}, // 0
  {BPWM1, BPWM1_MODULE, BPWM1_IRQn, 0,    500,  {18,  SYS_GPD_MFP_PD7_BPWM1_CH0,  NULL, NULL, SYS_ALT_MFP3_PD7_BPWM1_CH0,  NULL}}, // 1
  {BPWM0, BPWM0_MODULE, BPWM0_IRQn, 5,    500,  {19,  SYS_GPD_MFP_PD14_BPWM0_CH5, NULL, NULL, SYS_ALT_MFP3_PD14_BPWM0_CH5, NULL}}, // 2
  {BPWM0, BPWM0_MODULE, BPWM0_IRQn, 4,    500,  {20,  SYS_GPD_MFP_PD15_BPWM0_CH4, NULL, NULL, SYS_ALT_MFP3_PD15_BPWM0_CH4, NULL}}, // 3
};

PWMPinDescription PWM_Desc[] = {
//{PWMx, PWMx_MODULE, PWMx_IRQn, Chan, freq, {Pin, SYS_GPx_MFP_Pxx_PWMx_CHx,  ...,  ...,  SYS_ALT_MFP3,               SYS_ALT_MFP4}}
  {PWM1, PWM1_MODULE, PWM1_IRQn, 5,    500,  {2,   SYS_GPF_MFP_PF5_PWM1_CH5,  NULL, NULL, SYS_ALT_MFP3_PF5_PWM1_CH5,  NULL}}, // 0
  {PWM1, PWM1_MODULE, PWM1_IRQn, 4,    500,  {3,   SYS_GPF_MFP_PF4_PWM1_CH4,  NULL, NULL, SYS_ALT_MFP3_PF4_PWM1_CH4,  NULL}}, // 1
  {PWM1, PWM1_MODULE, PWM1_IRQn, 3,    500,  {4,   SYS_GPA_MFP_PA11_PWM1_CH3, NULL, NULL, SYS_ALT_MFP3_PA11_PWM1_CH3, NULL}}, // 2
  {PWM1, PWM1_MODULE, PWM1_IRQn, 2,    500,  {5,   SYS_GPA_MFP_PA10_PWM1_CH2, NULL, NULL, SYS_ALT_MFP3_PA10_PWM1_CH2, NULL}}, // 3

  {PWM0, PWM0_MODULE, PWM0_IRQn, 0,    500,  {28,  SYS_GPA_MFP_PA12_PWM0_CH0, NULL, NULL, NULL,                       SYS_ALT_MFP4_PA12_PWM0_CH0}}, // 4
  {PWM0, PWM0_MODULE, PWM0_IRQn, 1,    500,  {27,  SYS_GPA_MFP_PA13_PWM0_CH1, NULL, NULL, NULL,                       SYS_ALT_MFP4_PA13_PWM0_CH1}}, // 5   
  {PWM0, PWM0_MODULE, PWM0_IRQn, 3,    500,  {25,  SYS_GPA_MFP_PA15_PWM0_CH3, NULL, NULL, NULL,                       NULL}}, // 6
  {PWM0, PWM0_MODULE, PWM0_IRQn, 2,    500,  {26,  SYS_GPA_MFP_PA14_PWM0_CH2, NULL, NULL, NULL,                       NULL}}, // 7

  {PWM0, PWM0_MODULE, PWM0_IRQn, 4,    500,  {32,  SYS_GPA_MFP_PA0_PWM0_CH4,  NULL, NULL, SYS_ALT_MFP3_PA0_PWM0_CH4,  SYS_ALT_MFP4_PA0_PWM0_CH4}}, // 8
  {PWM0, PWM0_MODULE, PWM0_IRQn, 5,    500,  {33,  SYS_GPA_MFP_PA1_PWM0_CH5,  NULL, NULL, SYS_ALT_MFP3_PA1_PWM0_CH5,  SYS_ALT_MFP4_PA1_PWM0_CH5}}, // 9
  {PWM1, PWM1_MODULE, PWM1_IRQn, 0,    500,  {34,  SYS_GPA_MFP_PA2_PWM1_CH0,  NULL, NULL, SYS_ALT_MFP3_PA2_PWM1_CH0,  SYS_ALT_MFP4_PA2_PWM1_CH0}}, // 10
  {PWM1, PWM1_MODULE, PWM1_IRQn, 1,    500,  {35,  SYS_GPA_MFP_PA3_PWM1_CH1,  NULL, NULL, SYS_ALT_MFP3_PA3_PWM1_CH1,  SYS_ALT_MFP4_PA3_PWM1_CH1}}, // 11
};

ADCPinDescription ADC_Desc[] = {
//{ADC, ADC_MODULE, Channel, {Pin, SYS_GPx_MFP_PAx_ADCx, ...,  ...,  ...,  ... }}
  {ADC, ADC_MODULE, 0,       {32,  SYS_GPA_MFP_PA0_ADC0, NULL, NULL, NULL, NULL}}, // 0
  {ADC, ADC_MODULE, 1,       {33,  SYS_GPA_MFP_PA1_ADC1, NULL, NULL, NULL, NULL}}, // 1
  {ADC, ADC_MODULE, 2,       {34,  SYS_GPA_MFP_PA2_ADC2, NULL, NULL, NULL, NULL}}, // 2
  {ADC, ADC_MODULE, 3,       {35,  SYS_GPA_MFP_PA3_ADC3, NULL, NULL, NULL, NULL}}, // 3
  {ADC, ADC_MODULE, 5,       {37,  SYS_GPA_MFP_PA4_ADC4, NULL, NULL, NULL, NULL}}, // 4
  {ADC, ADC_MODULE, 6,       {38,  SYS_GPA_MFP_PA5_ADC5, NULL, NULL, NULL, NULL}}, // 5
};

SPIPinDescription SPI_Desc[] = {
  {SPI0, SPI0_MODULE, SPI0_IRQn, CLK_CLKSEL1_SPI0_S_HCLK, {
    {23, SYS_GPC_MFP_PC1_SPI0_CLK,   NULL, NULL, NULL, NULL},
    {22, SYS_GPC_MFP_PC2_SPI0_MISO0, NULL, NULL, NULL, NULL},
    {21, SYS_GPC_MFP_PC3_SPI0_MOSI0, NULL, NULL, NULL, NULL},
    {24, SYS_GPC_MFP_PC0_SPI0_SS0,   NULL, NULL, NULL, NULL},}},
};

UARTPinDescription UART_Desc[] = {
  {UART0, UART0_MODULE, UART02_IRQn, {
    {13, SYS_GPB_MFP_PB0_UART0_RXD, NULL, NULL, NULL, NULL},
    {14, SYS_GPB_MFP_PB1_UART0_TXD, NULL, NULL, NULL, NULL}}},
  {UART1, UART1_MODULE, UART1_IRQn, {
    {8, SYS_GPB_MFP_PB4_UART1_RXD, NULL, NULL, NULL, NULL}, 
    {9, SYS_GPB_MFP_PB5_UART1_TXD, NULL, NULL, NULL, NULL}}},
  {UART2, UART2_MODULE, UART02_IRQn,{
    {19,SYS_GPD_MFP_PD14_UART2_RXD, NULL, NULL, SYS_ALT_MFP3_PD14_UART2_RXD, NULL}, 
    {20,SYS_GPD_MFP_PD15_UART2_TXD, NULL, NULL, SYS_ALT_MFP3_PD15_UART2_TXD, NULL}}},
  {UART3, UART3_MODULE, UART3_IRQn, {
    {37,SYS_GPA_MFP_PA5_UART3_RXD, NULL, NULL, NULL, SYS_ALT_MFP4_PA5_UART3_RXD}, 
    {38,SYS_GPA_MFP_PA6_UART3_TXD, NULL, NULL, NULL, SYS_ALT_MFP4_PA6_UART3_TXD}}},
  {UART4, UART4_MODULE, UART4_IRQn, {
    {41,SYS_GPC_MFP_PC7_UART4_RXD, NULL, NULL, SYS_ALT_MFP3_PC7_UART4_RXD, SYS_ALT_MFP4_PC7_UART4_RXD}, 
    {42,SYS_GPC_MFP_PC6_UART4_TXD, NULL, NULL, SYS_ALT_MFP3_PC6_UART4_TXD, SYS_ALT_MFP4_PC6_UART4_TXD}}},
  {UART5, UART5_MODULE, UART5_IRQn, {
    {28,SYS_GPA_MFP_PA12_UART5_RXD, NULL, NULL, NULL, SYS_ALT_MFP4_PA12_UART5_RXD}, 
    {27,SYS_GPA_MFP_PA13_UART5_TXD, NULL, NULL, NULL, SYS_ALT_MFP4_PA13_UART5_TXD}}},		
};

I2CPinDescription I2C_Desc[] = {
  {I2C0, I2C0_MODULE, {
    {42, SYS_GPC_MFP_PC6_I2C0_SDA, NULL, NULL, SYS_ALT_MFP3_PC6_I2C0_SDA, SYS_ALT_MFP4_PC6_I2C0_SDA},
    {41, SYS_GPC_MFP_PC7_I2C0_SCL, NULL, NULL, SYS_ALT_MFP3_PC7_I2C0_SCL, SYS_ALT_MFP4_PC7_I2C0_SCL}}},
};
