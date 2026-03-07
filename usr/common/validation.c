#include <appConfig.h>
#if ENABLE_FUNCTION_VALIDATION == 1


#include <rugbyHP.h>
#include <validation.h>
#include <gpio_device.h>
#include <adc_device.h>
#include <uart_device.h>
#include <clock_device.h>
#include <buck_device.h>
#include <spi_device.h>


void Digital_Mux_LFRC_Output(void)
{

    //GPIO1
    IOCTRLA_SFRS->GPIO[0].HWMODE    = 2U;   //TestMuxMode
    IOCTRLA_SFRS->GPIO[0].PULLMODE  = 1U;   //None
    GPIO_SFRS->GPIO_CFG[0].DIR      = 1U;   //Output
    IOCTRLA_SFRS->GPIO[0].RDENA     = 0U;   //InputDisable
    IOCTRLA_SFRS->GPIO[0].MUXSEL = 0x03U;     //0x3: CRGA (Clock Reset Generation Asic) lf_rc_clk

}

void Digital_Mux_SystemClock_Output(void)
{
    //GPIO1
    IOCTRLA_SFRS->GPIO[0].HWMODE    = 2U;   //TestMuxMode
    IOCTRLA_SFRS->GPIO[0].PULLMODE  = 1U;   //None
    GPIO_SFRS->GPIO_CFG[0].DIR      = 1U;   //Output
    IOCTRLA_SFRS->GPIO[0].RDENA     = 0U;   //InputDisable
    IOCTRLA_SFRS->GPIO[0].MUXSEL = 0x07U;   //CRGA (Clock Reset Generation Asic) clk_sys_gated
}


void validation_main(void)
{
    SYSCTRLA_SFRS->DEBUG_ACCESS_KEY.DEBUG_ACCESS_KEY = 0x05U;
    SYSCTRLA_SFRS->PMU_ACCESS_KEY.PMU_ACCESS_KEY = 0x0AU;
    SYSCTRLA_SFRS->TRIM_ACCESS_KEY.TRIM_ACCESS_KEY = 0x0E;
    PMUAPRE5V_SFRS->CFG_ACCESS.CFG_ACCESS_KEY = 0x0B;
  
//    Clock_SystemMainClockInit(SYS_MAIN_CLOCK_DIV);

//    Digital_Mux_LFRC_Output();
    Digital_Mux_SystemClock_Output();
    while(1);
}


#endif
