
#include "Arduino.h"
extern void CLK_EnableModuleClock(uint32_t);
#ifdef __cplusplus
extern "C" {
#endif

/** Tick Counter united by ms */
static volatile uint32_t _dwTickCount=0 ;

/**
 * \brief SysTick_Handler.
 */
void SysTick_Handler(void)
{
	/* Increment counter necessary in delay(). */
	_dwTickCount++;
}

/**
 *  \brief Get current Tick Count, in ms.
 */
uint32_t GetTickCount( void )
{
  return _dwTickCount ;
}

#ifdef __cplusplus
}
#endif
//---------------------------------------------------------------------------

void Enable_All_IPs(void)
{
  // Enable ADC module
  CLK_EnableModuleClock(ADC_MODULE);
  
  // Enable PWM module
  CLK_EnableModuleClock(BPWM0_MODULE);
  CLK_EnableModuleClock(BPWM1_MODULE);
  CLK_EnableModuleClock(PWM0_MODULE);
  CLK_EnableModuleClock(PWM1_MODULE);

  CLK_SetModuleClock(BPWM0_MODULE, CLK_CLKSEL3_BPWM0_S_PLL, 0);
  CLK_SetModuleClock(BPWM1_MODULE, CLK_CLKSEL3_BPWM1_S_PLL, 0);
  CLK_SetModuleClock(PWM0_MODULE, CLK_CLKSEL3_PWM0_S_PLL, 0);
  CLK_SetModuleClock(PWM1_MODULE, CLK_CLKSEL3_PWM0_S_PLL, 0);
}
//F_CPU
void init(void)
{
  init_flag = 1;

  SYS_UnlockReg(); // Unlock protected registers

  /* STCLK to HXT */
  //CLK_SetSysTickClockSrc(CLK_CLKSEL0_STCLK_S_HXT);
  CLK_EnableXtalRC(CLK_PWRCON_OSC22M_EN_Msk); // Enable Internal RC 22.1184MHz clock
  CLK_WaitClockReady(CLK_CLKSTATUS_OSC22M_STB_Msk); // Waiting for clock ready
  CLK_SetHCLK(CLK_CLKSEL0_HCLK_S_HIRC, CLK_CLKDIV_HCLK(1)); // Switch HCLK clock source to HIRC
  CLK_EnableXtalRC(CLK_PWRCON_XTL12M_EN_Msk | CLK_PWRCON_OSC22M_EN_Msk); // Enable external 12MHz XTAL, internal 22.1184MHz
  CLK_WaitClockReady(CLK_CLKSTATUS_XTL12M_STB_Msk); // Waiting for external XTAL clock ready
  CLK_SetCoreClock(F_CPU); // Set core clock as PLL_CLOCK from PLL

  /* Init I/O Multi-function */
  SystemCoreClockUpdate(); // To update the variable SystemCoreClock

  if(SysTick_Config(SystemCoreClock/1000)) { // Set Systick to 1ms interval
  	while (true); // Capture error
  }    

  Enable_All_IPs(); // Enable All of IP
  SYS_LockReg(); // Lock protected registers
}
