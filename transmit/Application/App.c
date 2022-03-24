/*
 * App.c
 *
 *  Created on: 22 de mar de 2022
 *      Author: nathan
 */

	#include "App.h"



		//variaveis teclado;
		uint8_t row[4] = {0};
		uint8_t button = 0, i  = 0, validacao = 0;
		uint8_t senha[4] = {0}, senhaDefinida[4] = {1,2,3,4};

		//nrf
		uint8_t data_array[4];
		uint8_t sendFlag = 0;
		uint8_t tx_address[5] = {0xA0,0xA0,0xA0,0xA0,0xA0};
		uint8_t rx_address[5] = {0xD7,0xD7,0xD7,0xD7,0xD7};

	void app_init(){



	}

	void app_run(){

//		//variaveis teclado;
//		uint8_t row[4] = {0};
//		uint8_t button = 0, i  = 0, validacao = 0;
//		uint8_t senha[4] = {0}, senhaDefinida[4] = {1,2,3,4};
//
//		//nrf
//		uint8_t data_array[4];
//		uint8_t sendFlag = 0;
//		uint8_t tx_address[5] = {0xA0,0xA0,0xA0,0xA0,0xA0};
//		uint8_t rx_address[5] = {0xD7,0xD7,0xD7,0xD7,0xD7};

		nrf24_init();
		nrf24_config(2, 1);

		nrf24_tx_address(tx_address);
		nrf24_rx_address(rx_address);

		while(1){

		//MATRIZ 4x4 ---------------------
		//for passando pelas 4 linhas
		for(int x = 0; x < 4; x++){

			//se for na linha x
			switch(x){
				//verifica linha 0
				case 0:
					row[0] = 1;
					row[3] = 0;
					break;
				//verifica linha 1PinState
				case 1:
					row[1] = 1;
					row[0] = 0;
					break;
				//verifica linha 2
				case 2:
					row[2] = 1;
					row[1] = 0;
					break;
				//verifica linha 3
				case 3:
					row[3] = 1;
					row[2] = 0;
					break;
				default:
				break;

			}

			//ativa linha de acordo com o switch acima;
			HAL_GPIO_WritePin(R1_GPIO_Port, R1_Pin, row[0]);
			HAL_GPIO_WritePin(R2_GPIO_Port, R2_Pin, row[1]);
			HAL_GPIO_WritePin(R3_GPIO_Port, R3_Pin, row[2]);
			HAL_GPIO_WritePin(R4_GPIO_Port, R4_Pin, row[3]);

			//testa as coluna da linha selecionada;
			if(HAL_GPIO_ReadPin(C1_GPIO_Port, C1_Pin)) {
				button = x*4;
				i++;
				while(HAL_GPIO_ReadPin(C1_GPIO_Port, C1_Pin));
			}
			else if(HAL_GPIO_ReadPin(C2_GPIO_Port, C2_Pin)){
				button = 1+x*4;
				i++;
				while(HAL_GPIO_ReadPin(C2_GPIO_Port, C2_Pin));
			}
			else if(HAL_GPIO_ReadPin(C3_GPIO_Port, C3_Pin)){
				button = 2+x*4;
				i++;
				while(HAL_GPIO_ReadPin(C3_GPIO_Port, C3_Pin));
			}
			else if(HAL_GPIO_ReadPin(C4_GPIO_Port, C4_Pin)){
				button = 3+x*4;
				i++;
				while(HAL_GPIO_ReadPin(C4_GPIO_Port, C4_Pin));
			}
		}

		//armazena senha digitada;
		senha[i-1] = button;

		//se tiver 4 digitos
		if(i == 4){

			uint8_t verifica[4] = {0};

			//roda por cada digito comparando com a senhaDefinida
			for (int cont = 0; cont < 4; cont++) {

				if (senha[cont] == senhaDefinida[cont]) {
					verifica[cont] = CORRETA;
				}else verifica[cont] = INCORRETA;
			}

			validacao = (verifica[0]*verifica[1]*verifica[2]*verifica[3]);

			i = 0;
			sendFlag = 1;
		}

		//se senha for correta
		if(sendFlag == 1){

			//NRF TRANSMIT -------------------
			HAL_GPIO_WritePin(led_GPIO_Port, led_Pin, GPIO_PIN_SET);
			data_array[0] = validacao;
			nrf24_send(data_array);
			sendFlag = 0;
			HAL_Delay(150);
			HAL_GPIO_WritePin(led_GPIO_Port, led_Pin, GPIO_PIN_RESET);

		}

		}



	}

