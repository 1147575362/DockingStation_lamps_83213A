#ifndef BUCK_OPERATION_H_
#define BUCK_OPERATION_H_

#include <stdint.h>

typedef enum{
  BUCK_FREQ_400KHz = 5U,
}BuckFreq_t;



void BUCKOPT_EnableForcePWMMode(void);
void BUCKOPT_SetBuckFreq(uint8_t freq);
void BUCKOPT_Init(void);





























#endif