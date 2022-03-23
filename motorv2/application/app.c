/*
 * app.c
 *
 *  Created on: 22 de mar de 2022
 *      Author: joao
 */

//#include "app.h"

/*
 * Externs
 */

//extern TIM_HandleTypeDef htim2;

/*
 * Publics
 */

/*uint16_t conta = 0;
uint16_t flag = 0;
uint16_t vel = 0;

void app_init(){
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
}

void app_run(){
	uint8_t st, i;

	st = 0;
	i = 0;
	while (1){
		if (st == 0){
			PWM_Set_DC(&htim2, TIM_CHANNEL_1, i);
			i++;
			if (i > 99){
				st = 1;
			}
		}
		else{
			PWM_Set_DC(&htim2, TIM_CHANNEL_1, i);
			i--;
			if (i == 0){
				st = 0;
			}
		}
		HAL_Delay(50);
	}
}
	void  HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin){
	if(conta == 0){
			conta++;
		}
		else if (conta == 1)
		{
			PWM_Set_DC(&htim2, TIM_CHANNEL_2, 0);
			conta ++;
			vel = 1;
		}
		else if (conta == 2){
			PWM_Set_DC(&htim2, TIM_CHANNEL_2, 25);
			conta ++;
			vel = 2;
		}
		else if (conta == 1){
			PWM_Set_DC(&htim2, TIM_CHANNEL_2, 50);
			conta ++;
			vel = 3;
		}
		else if (conta == 2){
			PWM_Set_DC(&htim2, TIM_CHANNEL_2, 100);
			conta ++;
			vel = 4;
		}
		else if (conta == 3){
			PWM_Set_DC(&htim2, TIM_CHANNEL_2, 999);
			conta = 0;
			vel = 5;
		}
	}
*/

