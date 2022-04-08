/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include <stdint.h>

#define buffer 100
/*uint8_t AT[]="AT\r\n";
uint8_t Rx[20];
uint32_t NB = 0; // números de bytes lidos
uint8_t Saida = 10;
uint8_t Entrada = 10;*/

uint8_t flag_transmit, flag_receive, conta=0;
uint8_t Data_AT [7][50] = {"AT\r\n","AT+CWMODE=3\r\n","AT+CWSAP=\"TESTE\",\"12345678\",1,4\r\n","AT+CIPMUX=0\r\n","AT+CIPMODE=1\r\n","AT+CIPSTART=\"UDP\",\"192.168.4.2\",8696,8696\r\n","AT+CIPSEND\r\n"};
uint8_t Rec[buffer];
uint8_t Dado[5];

//,"+++","AT+CIPCLOSE\r\n"


//"AT\r\n","AT+CWMODE=3\r\n","AT+CWJAP=\"\",\"12345678\"\r\n","AT+CIPMUX=0\r\n","AT+CIPMODE=1\r\n","AT+CIPSTART=\"UDP\",\"192.168.4.1\",8696,8696\r\n","AT+CIPSEND\r\n","+++","AT+CIPCLOSE\r\n"

//"AT\r\n","AT+CWMODE=3\r\n","AT+CWSAP=\"MOSFET\",\"12345678\",1,4\r\n","AT+CWLIF\r\n","AT+CIPMUX=1\r\n","AT+CIPSERVER=1\r\n"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart4;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART4_UART_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

//void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart);

/*void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	NB++;

	HAL_UART_Receive_IT(&huart4, &Rx[NB], 1);

}*/

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	flag_transmit = 0;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	   HAL_UART_Transmit(&huart2, &Rec[conta], 1, 10);
/*	        if ((Rec[conta]=='\n')&&(Rec[conta-2]=='K')){
	                flag_receive = 0;
	                conta = 0;
	        }
	        else
	        {
	                conta++;
	                if(conta>buffer)
	                        conta=0;
	                HAL_UART_Receive_IT(&huart4, &Rec[conta], 1);

	        }
*/

	if(Rec[conta]== '>'){
		HAL_UART_Receive_IT(&huart4, Dado, 5);

	}else{
		conta++;
		if(conta>=buffer)
				conta=0;

		HAL_UART_Receive_IT(&huart4, &Rec[conta], 1);
	}



}

// Retorna o valor do ultimo byte recebido: se for '>' quer dizer que a comunicação está estabelecida
uint8_t Esp_Comandos(void){

	for(int i=0; i<7;i++){


		flag_transmit = 1;
		HAL_UART_Transmit_IT(&huart4, Data_AT[i], strlen(Data_AT[i]));
		while(flag_transmit);

		conta=0;
		flag_receive = 1;
		HAL_UART_Receive_IT(&huart4, Rec, 1);
		while(flag_receive)
		{
			//HAL_Delay(1);
			if(Rec[conta-1]=='\n')
			{
				if(Rec[conta-3]=='K')
					flag_receive = 0;
				else if(Rec[conta-3]=='R')
				{
					i--;
					flag_receive = 0;
				}
			}
		}
		HAL_Delay(2000);
	}


	return Rec[conta-1];

}

uint8_t send = 0;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
uint8_t Mensagem[] = "teste";
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART4_UART_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  //HAL_Delay(200);
/*
  Saida = HAL_UART_Transmit(&huart4, AT, 4, 100);


  Entrada = HAL_UART_Receive_IT(&huart4, &Rx[NB], 2)*/;
  //remove_Busy();

  HAL_GPIO_WritePin(RESET_GPIO_Port, RESET_Pin, 0);
  HAL_Delay(1000);

  HAL_GPIO_WritePin(RESET_GPIO_Port, RESET_Pin, 1);
  HAL_Delay(5000);

  if(Esp_Comandos()=='>') // Se estiver conectado com o outro módulo, liga o LED
  {
	  HAL_GPIO_WritePin(LED_PLACA_GPIO_Port, LED_PLACA_Pin, 1);
  }
//  else					// Se houve algum erro na conexão, o programa fica travado e o LED fica piscando
//  {
//	  while(1)
//	  {
//		  HAL_GPIO_TogglePin(LED_PLACA_GPIO_Port, LED_PLACA_Pin);
//		  HAL_Delay (250);
//	  }
//  }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if (send){
		  HAL_UART_Transmit_IT(&huart4, Mensagem, strlen(Mensagem));
		  HAL_Delay(1000);
		  send = 0;
	  }

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 8;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 460800;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart2, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart2, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief USART4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART4_UART_Init(void)
{

  /* USER CODE BEGIN USART4_Init 0 */

  /* USER CODE END USART4_Init 0 */

  /* USER CODE BEGIN USART4_Init 1 */

  /* USER CODE END USART4_Init 1 */
  huart4.Instance = USART4;
  huart4.Init.BaudRate = 115200;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  huart4.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart4.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart4.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART4_Init 2 */

  /* USER CODE END USART4_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_PLACA_GPIO_Port, LED_PLACA_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(RESET_GPIO_Port, RESET_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : LED_PLACA_Pin */
  GPIO_InitStruct.Pin = LED_PLACA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LED_PLACA_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : RESET_Pin */
  GPIO_InitStruct.Pin = RESET_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(RESET_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

