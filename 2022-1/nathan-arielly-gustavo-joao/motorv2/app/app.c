/*
 * app.c
 *
 *  Created on: Mar 24, 2022
 *      Author: joao
 */

#include "app.h"

extern ADC_HandleTypeDef hadc1;
extern  TIM_HandleTypeDef htim2;

uint8_t button = 0;
uint16_t conta = 1;
uint16_t flag = 0;
uint16_t vel = 0;
uint32_t value = 0;
float corrente = 0;

float  tensao;

void PWM_Set_DC_(TIM_HandleTypeDef *timer, uint32_t channel, uint8_t dc){
	uint32_t arr, ccrx;

	arr = __HAL_TIM_GET_AUTORELOAD(timer);
	if(dc <= 100){
		ccrx = arr*dc/100;
		__HAL_TIM_SET_COMPARE(timer, channel, ccrx);
	}
}

void  HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin){

	button = 1;


}

void app_init(){
	HAL_ADCEx_Calibration_Start(&hadc1);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
}

void app_run(){
	while (1)
	{
		value = 0;
		for(int i = 0;i < 250; i++ ){
			HAL_ADC_Start(&hadc1); // start adc conversion

			HAL_ADC_PollForConversion(&hadc1, 100); // aguarda o fim da conversão

			value += HAL_ADC_GetValue(&hadc1);//obtém valor do registrador de resultados ADC

		}

		value = value/250;
		tensao = (3.3 / 4095) * value;
		corrente = (32*value)/3900;

//velocidade com valores 1,2,3,4

		if(button == 1){
			if (conta == 1)
			{
				PWM_Set_DC_(&htim2, TIM_CHANNEL_2, 0);
				conta ++;
				vel = Desligado;
			}
			else if (conta == 2){
				PWM_Set_DC_(&htim2, TIM_CHANNEL_2, 25);
				conta ++;
				vel = Velocidade_Media;
			}
			else if (conta == 3){
				PWM_Set_DC_(&htim2, TIM_CHANNEL_2, 100);
				conta = 1;
				vel = Velocidadde_Maxima;

			}
			button = 0;
		}
	}
}
