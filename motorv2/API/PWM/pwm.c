/*
 * pwm.c
 *
 *  Created on: 22 de mar de 2022
 *      Author: joao
 */


/*#include "pwm.h"

void PWM_Set_DC(TIM_HandleTypeDef *timer, uint32_t channel, uint8_t dc){
	uint32_t arr, ccrx;

	arr = __HAL_TIM_GET_AUTORELOAD(timer);
	if(dc <= 100){
		ccrx = arr*dc/100;
		__HAL_TIM_SET_COMPARE(timer, channel, ccrx);
	}
}
*/
//void  HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin){
//if(conta == 0){
//		conta++;
//	}
//	else if (conta == 1)
//	{
//		PWM_Set_DC(&htim2, TIM_CHANNEL_2, 0);
//		conta ++;
//		vel = 1;
//	}
//	else if (conta == 2){
//		PWM_Set_DC(&htim2, TIM_CHANNEL_2, 25);
//		conta ++;
//		vel = 2;
//	}
//	else if (conta == 1){
//		PWM_Set_DC(&htim2, TIM_CHANNEL_2, 50);
//		conta ++;
//		vel = 3;
//	}
//	else if (conta == 2){
//		PWM_Set_DC(&htim2, TIM_CHANNEL_2, 100);
//		conta ++;
//		vel = 4;
//	}
//	else if (conta == 3){
//		PWM_Set_DC(&htim2, TIM_CHANNEL_2, 999);
//		conta = 0;
//		vel = 5;
//	}
//}
