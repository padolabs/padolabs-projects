/*
 * pwm.h
 *
 *  Created on: 22 de mar de 2022
 *      Author: joao
 */

/*#ifndef PWM_PWM_H_
#define PWM_PWM_H_

#include <stdint.h>

#include "stm32g0xx.h"

void PWM_Set_DC(TIM_HandleTypeDef *timer, uint32_t channel, uint8_t dc);
void  HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin);

#endif /* PWM_PWM_H_ */
