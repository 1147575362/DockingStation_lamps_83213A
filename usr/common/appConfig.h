#ifndef APP_CONFIG_H__
#define APP_CONFIG_H__

#include <clock_device.h>
#include <pwm_device.h>
#include <string.h>

#define PRAGMA(x) _Pragma(#x)
#define BEGIN_PACK PRAGMA(pack(push, 1))
#define END_PACK   PRAGMA(pack(pop))

#define LIN_STACK_TYPE_LIN2_2A          (0U)
#define LIN_STACK_TYPE_SAEJ2602         (1U)
#define LIN_STACK_TYPE_PRIVATE          (2U)
#define LIN_STACK_TYPE_ISO17987         LIN_STACK_TYPE_LIN2_2A

#define LIN_STACK_TYPE                  LIN_STACK_TYPE_LIN2_2A

#define SDK_APPL_VERSION                (0x113U)
#define AUTO_ADDRESSING_EN              (1U)

/* ************************hardware config ********************************/
#define MAIN_CPU_CLOCK          16U       /*MHz*/

#if MAIN_CPU_CLOCK == 16U
    #define SYS_MAIN_CLOCK_DIV    CLOCK_DIV_1
#else
    #error MAIN_CPU_CLOCK MUST BE 16MHz!*/
#endif

#define ROOM_TEMPERATURE        (24)  /*Celsius degree*/

/* ************************PWMs config ********************************/
#define LED_NUM    (8U)

#if LED_NUM <= 8U
  #define PWM_VALUE_MAX_POS       (16U)
#else
  #define PWM_VALUE_MAX_POS       (14U)
#endif

#define PWM_VALUE_MAX           ((1UL << PWM_VALUE_MAX_POS) -1UL)


#define BOARD_TYPE_DP           (0)
#define BOARD_TYPE_SY           (1)
#define BOARD_TYPE_S2           (2)

#if LED_NUM <= 8U && LED_NUM != 0U
    /* rgb channels settings*/
    static __INLINE PwmChannel_t PHY_CHANNEL_R(uint8_t x){ return (PwmChannel_t)((uint8_t)PWM_CHANNEL_2 + 3U*x);}
    static __INLINE PwmChannel_t PHY_CHANNEL_G(uint8_t x){ return (PwmChannel_t)((uint8_t)PWM_CHANNEL_1 + 3U*x);}
    static __INLINE PwmChannel_t PHY_CHANNEL_B(uint8_t x){ return (PwmChannel_t)((uint8_t)PWM_CHANNEL_0 + 3U*x);}
#else
    #define LED_BOARD_TYPE          BOARD_TYPE_S2
    /* rgb channels settings*/
    #if LED_BOARD_TYPE == BOARD_TYPE_DP
      #define PHY_CHANNEL_R    PWM_CHANNEL_2
      #define PHY_CHANNEL_G    PWM_CHANNEL_1
      #define PHY_CHANNEL_B    PWM_CHANNEL_0
    #elif LED_BOARD_TYPE == BOARD_TYPE_SY
      #define PHY_CHANNEL_R    PWM_CHANNEL_0
      #define PHY_CHANNEL_G    PWM_CHANNEL_2
      #define PHY_CHANNEL_B    PWM_CHANNEL_1
    #elif LED_BOARD_TYPE == BOARD_TYPE_S2
      #define PHY_CHANNEL_R    PWM_CHANNEL_2
      #define PHY_CHANNEL_G    PWM_CHANNEL_1
      #define PHY_CHANNEL_B    PWM_CHANNEL_0
    #else
      #error invalid LED_BOARD_TYPE definition!
    #endif

    #if LED_NUM > 16U || LED_NUM == 0U
      #error LED_NUM should be within 1-16!
    #endif
#endif


/* ************************System config ******************************/
#if AUTO_ADDRESSING_EN == 1U
    #define LIN_MASTER_EN                       (0U)
#else
    #define LIN_MASTER_EN                       (1U)
#endif

#define WATCH_DOG_EN                            (0U)
/* ************************debug config *******************************/
#define ENABLE_FUNCTION_VALIDATION              (0U)


#define UART_MILKY_WAY_EN                       (0U)
#define CODE_DEBUG_EN                           (0U)

#if CODE_DEBUG_EN == 1U
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #define DEBUG_OUT(...) printf(__VA_ARGS__)
#else
  #define DEBUG_OUT(...)      
#endif


#endif