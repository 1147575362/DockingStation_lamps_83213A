#ifndef RUGBY_HP_H__
#define RUGBY_HP_H__

/*
	declare an enumerated type that can be used to refer to interrupts by name, e.g. in calls to the NVIC configuration
	functions - the definitions here must match the vector table positions, offset so that IRQ0 (the 1st ASIC IRQ) has
	the value 0...
*/
typedef enum IRQn
{
  /* Cortex-M0 core exceptions... */
  NMI_IRQn          = -14,       
  Reset_IRQn        = -15,
  HardFault_IRQn    = -13,
  SVC_IRQn          = -5, 
  PendSV_IRQn       = -2, 
  SysTick_IRQn      = -1, 
  /* ASIC-specific IRQs... (should match the vector defined in dig_rugby_top.sv)*/
  WICA_IRQn         = 0, 
  PWMAUX_IRQn       = 1, 
  IOCTRLA_IRQn      = 2, 
  BOR_IRQn          = 3, 
  VBAT_IRQn    	    = 4, 
  OVTEMP_IRQn       = 5, 
  WDT_IRQn          = 6, 
  GPIO_IRQn         = 7, 
  LINS_IRQn         = 8, 
  LINM_IRQn         = 9, 
  UART_IRQn         = 10, 
  ADC_IRQn          = 11, 
  PWM_IRQn          = 12, 
  BUCK_IRQn         = 13, 
  SPI_IRQn          = 14,
  Lullaby_IRQn      = 15,
  /* Verne peripheral IRQs... */
  Timer0_IRQn       = 16, 
  Timer1_IRQn       = 17, 
  Timer2_IRQn       = 18, 
  Watchdog_IRQn     = 19, 
  BTE_IRQn          = 20, 
  SDIO_IRQn         = 21
} IRQn_Type;


#include <stdint.h>
#include "sfrs/rugbyHP_sfrs.h"
#include "verne.h"

#define TRUE                    (1U)
#define FALSE                   (0U)

#endif

