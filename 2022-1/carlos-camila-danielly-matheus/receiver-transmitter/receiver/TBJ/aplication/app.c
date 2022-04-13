/*
 * app.c
 *
 *  Created on: Mar 24, 2022
 *      Author: gabrielly, Camila
 */

 #include "app.h"

 extern SPI_HandleTypeDef hspi1;
 extern SPI_HandleTypeDef hspi1;
 extern ADC_HandleTypeDef hadc1;

 extern	TIM_HandleTypeDef htim1;
 extern	TIM_HandleTypeDef htim16;

 extern step_motor_t Motor;

 uint32_t vin, corrente;
 uint8_t received_array[2], transmit_array[2];
 uint8_t tx_address[5] = {0xE7,0xE7,0xE7,0xE7,0xE7};
 uint8_t rx_address[5] = {0xD8,0xD8,0xD8,0xD8,0xD8};
 uint8_t status=10;

 void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim16)
{
	if (received_array[0] != 0){
	if (received_array[1]==1)
	{step_drive_steps(&Motor, 1, STEP_DIRECTION_ACW, 0);}
	else if(received_array[1]==2)
	{step_drive_steps(&Motor, 1, STEP_DIRECTION_CW, 0);}
    }

}

 void convic(){
	for(uint8_t i=0; i < 250; i++){
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1,100);
	vin += HAL_ADC_GetValue(&hadc1);
    }
	vin=vin/250;
	corrente=vin;


}

 void app_init(){

	HAL_TIM_Base_Start(&htim1);

	Motor.pins.Blue.gpio = azul_GPIO_Port;
	Motor.pins.Blue.pin = azul_Pin;
	Motor.pins.Orange.gpio = laranja_GPIO_Port;
	Motor.pins.Orange.pin = laranja_Pin;
	Motor.pins.Yellow.gpio = amarelo_GPIO_Port;
	Motor.pins.Yellow.pin = amarelo_Pin;
	Motor.pins.Pink.gpio = rosa_GPIO_Port;
	Motor.pins.Pink.pin = rosa_Pin;


	step_init(&Motor);
		HAL_TIM_Base_Start_IT(&htim16);
		nrf24_config(2,2);

		 nrf24_tx_address(rx_address);
		 nrf24_rx_address(tx_address);
		 nrf24_powerUpRx();


 }

 void app_run(){


	 while (1){
		/* status = nrf24_dataReady();*/
		if(nrf24_dataReady()) {
			nrf24_getData(received_array);
	     __HAL_TIM_SET_PRESCALER(&htim16, (2018 - received_array[0]*18.18));
			HAL_GPIO_WritePin(led_GPIO_Port, led_Pin, 1);
			HAL_Delay(50);
			convic();
			transmit_array[0] = corrente;
			HAL_GPIO_WritePin(led_GPIO_Port, led_Pin, 0);
			nrf24_send(transmit_array);
			while(nrf24_isSending());
			status=nrf24_lastMessageStatus();
			nrf24_powerUpRx();

		}

	    }

 }



