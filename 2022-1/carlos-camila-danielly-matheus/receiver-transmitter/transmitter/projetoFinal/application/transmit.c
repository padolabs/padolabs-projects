/*		  //while(nrf24_isSending());
		  //status = 5;
//		  while(status != NRF24_TRANSMISSON_OK){
//
 * transmit.c
 *
 *  Created on: 29 de mar de 2022
 *      Author: carlos
 */

#include "transmit.h"

/*
 * Externs
 */

uint8_t transmit_array[2];
uint8_t receive_array[2];
SPI_HandleTypeDef hspi1;
uint8_t counter;
encoder_t Encoder;
uint8_t status = 0;
uint8_t numb = 1;
uint8_t flagsend=0;
uint8_t flagreceive=0;
/*
 * on/off enumerate
 */

typedef enum{
	DESLIGADO,
	LIGADO
}on_off_e;

on_off_e on_off = DESLIGADO;

/*
 * direction enumerate
 */

typedef enum{
	DIREITA,
	ESQUERDA

}direction_e;

direction_e direction = DIREITA;

/*
 * Timer interruption to get the encoder value
 */

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){
	counter = encoder_get_value(&Encoder);

	if(counter > 100){
		counter = 100;
	}

}

/*
 * Button interruption to set the direction
 */

void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin){
	if(HAL_GPIO_ReadPin(direction_button_GPIO_Port, direction_button_Pin) == 0){
		if(direction == 0){
			direction = 1;
		}
		else{
			direction = 0;
		}
	}
	flagsend = 1;

}

/*
 * Button interruption to set motor on/off
 */

//void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin){
//
//	if(HAL_GPIO_ReadPin(on_off_button_GPIO_Port, on_off_button_Pin) == 0){
//		on_off = 1;
//	}
//	else if(HAL_GPIO_ReadPin(on_off_button_GPIO_Port, on_off_button_Pin) == 1){
//		on_off = 0;
//	}
//
//}

/*
 * Publics
 */

void transmit_init(){

	uint8_t tx_address[5] = {0xE7,0xE7,0xE7,0xE7,0xE7}; //E7
	uint8_t rx_address[5] = {0xD8,0xD8,0xD8,0xD8,0xD8};

	nrf24_init();
	nrf24_config(2, 2);

	nrf24_tx_address(tx_address);
	nrf24_rx_address(rx_address);

	encoder_init(&Encoder, &htim2, TIM_CHANNEL_ALL);
}

void transmit_run(){
	HD44780_Init(2);
	HD44780_PrintStr("POW:");
	HD44780_SetCursor(0, 1);
	HD44780_PrintStr("Sent:");
	HD44780_SetCursor(10, 0);
	HD44780_PrintStr("mA:");
	HD44780_SetCursor(10, 1);
	HD44780_PrintStr("V:");

	uint8_t shadow;
	shadow = 0;
	char snum[5];
	char cor[5];
	nrf24_powerUpRx();
	while (1){

		itoa(counter, snum, 10);
		HD44780_SetCursor(12, 1);
		HD44780_PrintStr(snum);
		HD44780_SetCursor(14, 1);
		HD44780_PrintStr("  ");
		if(counter>99){
			HD44780_SetCursor(12, 1);
			HD44780_PrintStr("MX");
		}
		if(direction == 0){
			HD44780_SetCursor(5, 1);

			HD44780_PrintStr("RG");
		}
		else{
			HD44780_SetCursor(5, 1);

			HD44780_PrintStr("LF");
		}

		if(on_off == 0){
			HD44780_SetCursor(4, 0);

			HD44780_PrintStr("OFF");
		}
		else{
			HD44780_SetCursor(4, 0);
			HD44780_PrintStr("ON ");
		}
		HD44780_SetCursor(13, 0);
		itoa(receive_array[0], cor, 10);
		HD44780_PrintStr(cor);


	  HAL_GPIO_WritePin(led_GPIO_Port, led_Pin, GPIO_PIN_SET);
	  transmit_array[0] = counter;
	  if(HAL_GPIO_ReadPin(on_off_button_GPIO_Port, on_off_button_Pin)==0){
		  if(on_off == 0){
			 on_off = 1;
			 flagsend=1;
		  }
		  else{
			  on_off = 0;
			  flagsend=1;
		  }
	  }
	  if (shadow != transmit_array[0] || flagsend == 1){
		  shadow = transmit_array[0];
		  flagsend =0;


		  if(on_off == DESLIGADO){
			  transmit_array[1] = 0;
		  }
		  else{
			  transmit_array[1] = direction + 1;
		  }

		  nrf24_send(transmit_array);
		  while(nrf24_isSending());

		  status = nrf24_lastMessageStatus();

		  /*
		   * sending confirmation (used sometimes)
		   */

		  nrf24_powerUpRx();
		  flagreceive = 0;



		  //status = 5;
//		  while(status != NRF24_TRANSMISSON_OK){

//
//		  }
	  }
	  if(nrf24_dataReady()){
		  nrf24_getData(receive_array);
		  flagreceive = 1;

	  }

	  HAL_GPIO_WritePin(led_GPIO_Port, led_Pin, GPIO_PIN_RESET);

	  HAL_Delay(100);



	}
}



