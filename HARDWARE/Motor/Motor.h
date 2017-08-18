#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f4xx_conf.h"
#include "delay.h"

extern uint8_t motor_flag;

void StepMotor_Init(void);
void StepMotor_ctrl(uint8_t dir, uint32_t steps);

#endif

