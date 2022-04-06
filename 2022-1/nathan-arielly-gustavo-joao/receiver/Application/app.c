/*
 * app.c
 *
 *  Created on: 23 de mar de 2022
 *      Author: arielly
 */


#include "app.h"


/*
 *	externs
 */

//extern SPI_HandleTypeDef hspi1;


/*
 *	publics
 */

uint8_t data_array[4];	/* dados recebidos */


void app_init()
{

}


void app_run()
{
	/* endereço para mandar dados */
	uint8_t tx_address[5] = {0xE7,0xE7,0xE7,0xE7,0xE7};

	/* endereço para receber dados */
	uint8_t rx_address[5] = {0xA0,0xA0,0xA0,0xA0,0xA0};


	nrf24_init();

	nrf24_config(2, 1);

	nrf24_tx_address(tx_address);
	nrf24_rx_address(rx_address);


	while(1)
	{
		/*
		 * NRF24L01 RECEIVER
		 */

		/* se nrf24 estiver se comunicando */
		if(nrf24_dataReady())
		{
			/* recebe dados enviados */
			nrf24_getData(data_array);

			/* a cada dados recebido o led acende */
			HAL_GPIO_WritePin(led_GPIO_Port, led_Pin, 1);
			HAL_Delay(50);
			HAL_GPIO_WritePin(led_GPIO_Port, led_Pin, 0);
		}
	}
}
