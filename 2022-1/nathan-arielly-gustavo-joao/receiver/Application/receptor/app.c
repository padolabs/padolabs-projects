/*
 * app.c
 *
 *  Created on: 23 de mar de 2022
 *      Author: arielly
 */


#include "app.h"

/*
 *	EXTERNS
 */

extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim2;

extern TIM_HandleTypeDef htim7;
extern TIM_HandleTypeDef htim16;

/*
 *	PUBLICS
 */

/* endereço para mandar dados */
uint8_t tx_address[5] = {0xD7,0xD7,0xD7,0xD7,0xD7};
/* endereço para receber dados */
uint8_t rx_address[5] = {0xA0,0xA0,0xA0,0xA0,0xA0};

uint8_t data_array[2];	/* dados recebidos */
uint8_t data_transmit[2] = {0, 0};	/* dados enviados */

uint8_t debug = 15;
uint8_t flag_envio = 0;


uint32_t value;
float corrente;
uint16_t vel;
uint8_t tempo_rodando = 0;
uint8_t tempo_desligado;

/*app_t app ={
		.tx_address = {0xD7,0xD7,0xD7,0xD7,0xD7},
		.rx_address = {0xA0,0xA0,0xA0,0xA0,0xA0},

		.data_transmit = {0, 0},

		.debug = 15,
		.flag_envio = 0,

		.tempo_rodando = 0
};*/

void app_init()
{
	HAL_TIM_Base_Start_IT(&htim16); /* inicia o timer com interrupcao */

	HAL_ADCEx_Calibration_Start(&hadc1); //Função para calibrar o ADC
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2); //inicia o PWM
}

void PWM_Set_DC_(TIM_HandleTypeDef *timer, uint32_t channel, uint8_t dc){ //
	uint32_t arr, ccrx; //

	arr = __HAL_TIM_GET_AUTORELOAD(timer); //
	if(dc <= 100){ //
		ccrx = arr*dc/100; //
		__HAL_TIM_SET_COMPARE(timer, channel, ccrx); //
	}
}

/* guarda por 200mms os valores do motor, para enviar para o transmit */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim16) /* timer com interrupcao */
{
	/* incrementa flag_envio */
	if(flag_envio > 0){
		flag_envio++;
	}

	/* se o transmit envia um numero maior que 0 */
	if(data_array[0] > 0){
		/* sem o tamanho for maior que 0 */
		if(tempo_rodando > 0){
			tempo_rodando--; /* diminui ate chegar em 0 */

			/* liga o motor */
			PWM_Set_DC_(&htim2, TIM_CHANNEL_2, 100);
			vel = Velocidade_Maxima;
		} else { /* desliga o motor, por um tempo */
			PWM_Set_DC_(&htim2, TIM_CHANNEL_2, 0); // Define a velocidade do motor
			vel = Desligado;

			tempo_desligado++;
			if(tempo_desligado >= 10){ /* fica desligado por 10mms */
				tempo_desligado = 0; /* tempo que fica desligado volta a ser 0 */

				tempo_rodando = data_array[1]; /* tempo que roda o motor, voltar a ter o tempo proposto */
			}
		}
	}

	/* dados que podem ser enviados para o transmit */
	if(data_array[0] != 0)
	{
		/* o que envia para o transmit */
		switch(data_array[0]) {
			case 1:
				data_transmit[0] = corrente;
				data_transmit[1] = 1;
				break;

			case 2:
				data_transmit[0] = corrente;
				data_transmit[1] = 2;
				break;

			default:
				break;
		}
	}
}

void app_run()
{
	nrf24_init();

	nrf24_config(2, 2); /* quantidade que recebe e quantidade que envia */

	nrf24_tx_address(tx_address);
	nrf24_rx_address(rx_address);
	nrf24_powerUpRx();


	while(1)
	{
		if(flag_envio == 3){
			data_transmit[0] = corrente;

			nrf24_send(data_transmit); /* envia os para o transmit */
			while(nrf24_isSending()); /* espera os dados serem enviados */

			debug = nrf24_lastMessageStatus(); /* mostra o status do envio */

			nrf24_powerUpRx(); /* muda para receiver */

			flag_envio = 0; /* depois do envio, flag volta a ser 0 */
		}

		/* MOTOR CONTA */
		value = 0;
		for(int i = 0; i < 250; i++){
			HAL_ADC_Start(&hadc1); // start adc conversion

			HAL_ADC_PollForConversion(&hadc1, 100); // aguarda o fim da conversão

			value += HAL_ADC_GetValue(&hadc1); // obtém valor do registrador de resultados ADC
		}

		value = value/250;
		corrente = (32*value)/3900; // 3900 = maximo que value chega

		/* NRF24L01 RECEBER E ENVIAR*/
		if(nrf24_dataReady()) /* se nrf24 estiver se comunicando */
		{
			/* dados recebido o led acende */
			HAL_GPIO_WritePin(led_GPIO_Port, led_Pin, 1);

			/* recebe dados do transmit */
			nrf24_getData(data_array);

			/* se o tempo que o motor fica desligado for igual a 0 */
			if(tempo_desligado == 0){
				//tempo_rodando = data_array[1] - tempo_rodando;
				tempo_rodando = 0; /* zera o tempo */
				tempo_rodando = data_array[1]; /* comeca uma nova contagem */
			}

			/* se o transmit enviar 0 o motor desliga */
			if(data_array[0] == 0){
				PWM_Set_DC_(&htim2, TIM_CHANNEL_2, 0); // Define a velocidade do motor
				vel = Desligado;

				tempo_rodando = 0;
				tempo_desligado = 0;

				data_transmit[0] = corrente;
			}

			flag_envio = 1; /*  */

		}
		HAL_Delay(50);
		HAL_GPIO_WritePin(led_GPIO_Port, led_Pin, 0); /* fim da transmicao */
	}
}

