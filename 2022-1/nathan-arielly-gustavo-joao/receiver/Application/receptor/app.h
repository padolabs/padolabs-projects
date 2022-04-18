/*
 * app.h
 *
 *  Created on: 23 de mar de 2022
 *      Author: arielly
 */

#ifndef APP_H_
#define APP_H_

#include "stm32g0xx_hal.h"

#include <stdint.h>
#include "string.h"

#include "nrf24.h"

#define led_GPIO_Port GPIOA
#define led_Pin GPIO_PIN_5


typedef enum{
	Desligado,
	Velocidade_Media,
	Velocidade_Maxima
}velocidade_e;

typedef struct{
	/* endereço para mandar dados */
	uint8_t tx_address[5];
	/* endereço para receber dados */
	uint8_t rx_address[5];

	uint8_t data_array[2];	/* dados recebidos */
	uint8_t data_transmit[2];	/* dados enviados */

	uint8_t debug;
	uint8_t flag_envio;


	uint32_t value;
	float corrente;
	uint16_t vel;
	uint8_t tempo_rodando;
	uint8_t tempo_desligado;
}app_t;


void PWM_Set_DC_(TIM_HandleTypeDef *timer, uint32_t channel, uint8_t dc);
void delay(uint16_t rodando);

void app_init();
void app_run();

#endif /* APP_H_ */
