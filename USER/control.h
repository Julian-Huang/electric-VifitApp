#ifndef __CONTROL_H
#define __CONTROL_H

#include "stm32f4xx.h"
#include "rgb.h"
#include "MAX30102.h"

void heartrate_spo2_test(uint8_t side);
void strength_test(void);
void test_control( char *p,uint8_t len );
void RGB_test(void);
void Motor_test(void);

#endif
