/*
 * app.h
 *
 *  Created on: Mar 24, 2022
 *      Author: joao
 */

#ifndef APP_H_
#define APP_H_

#include<stdint.h>
#include "main.h"


void HAL_ADC_ConvCpltcallback(ADC_HandleTypeDef *hadc);
void  HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin);
void PWM_Set_DC_(TIM_HandleTypeDef *timer, uint32_t channel, uint8_t dc);
void app_run();
void app_init();
enum motor{Desligado, Velocidade_Media, Velocidadde_Maxima};
#endif /* APP_H_ */
