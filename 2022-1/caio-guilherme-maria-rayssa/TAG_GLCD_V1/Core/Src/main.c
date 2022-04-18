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

#include "ST7920_SERIAL.h"
#include "delay.h"
#include "bitmap.h"
#include "MFRC522.h"
#include "string.h"
#include <stdint.h>

#define buffer 100

uint8_t Dado;
uint8_t flag_transmit, flag_receive, conta=0;
uint8_t Data_AT [7][50] = {"AT\r\n","AT+CWMODE=3\r\n","AT+CWSAP=\"TESTE\",\"12345678\",1,4\r\n","AT+CIPMUX=0\r\n","AT+CIPMODE=1\r\n","AT+CIPSTART=\"UDP\",\"192.168.4.2\",8696,8696\r\n","AT+CIPSEND\r\n"};
uint8_t Rec[buffer];
uint8_t send = 0;
uint8_t Mensagem;

uint8_t status = 144;
uint8_t str[5];
uint8_t cardstr[3];
uint8_t login[10][5];
uint8_t flagCadrst=0, qntsUser=0, flagGraph=0, podeLer = 1, timer=0;
uint8_t i = 0, x = 0;
char msg[] = "Corrente = ";

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
SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart2;
UART_HandleTypeDef huart4;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART4_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* Funções ESP32 -------------------------------------------------------------*/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	flag_transmit = 0;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	HAL_UART_Transmit(&huart2, &Rec[conta], 1, 10);

	if(Rec[conta]== '>'){
		HAL_UART_Receive_IT(&huart4, &Dado, 4); 					// é aqui

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


	return '>';

}



/* FIM FUNÇÕES ESP32 ----------------------------------------------------------*/

/* FUNÇÔES RFID + LCD ---------------------------------------------------------*/

//Le a tag
void leTag(){											// Status recebe o Request avisando se possui ou não uma tag encostada ao sensor
	if(podeLer == 1){
		status = MFRC522_Request(PICC_REQIDL, cardstr);
		if( status == 0){
		  status = MFRC522_Anticoll(str);
		  podeLer = 0;

		}
	}
}

//Limpa a tag recebida
void limpaTag(uint8_t *a){							// Função que limpa a Tag recebida
	for(int i=0; i < 5; i++){a[i] = 0x0;}
}

void bloqueiaTag(uint8_t pos){						// Função que exclui a tag do database
	for (uint8_t y = 0; y < 5; ++y) {
		login[pos][y] = 0xFF;
	}
	for (uint8_t z = pos; z < 10; ++z) {
		for (uint8_t y = 0; y < 5; ++y) {
			login[z][y] = login[z+1][y];
		}
	}
	qntsUser--;
	limpaTag(str);
	return;
}
//Compara um vetor com a database de TAGS
uint8_t comparaTag(uint8_t *comp){
	i = 0;
	x = 0;

	if(comp[0] == 0x27 && comp[1] == 0x25 && comp[2] == 0xd && comp[3] == 0x3c && comp[4] == 0x33){		// Se a tag for essa ( Cartão ) entrará no modo de cadastro
		return 2;
	}


	while(x < 10){					// While compara as tags do database com a que está sendo lida, caso seja uma nova tag, ele a adiciona no proximo endereço do DB
		while(comp[i] == login[x][i] && i != 5){i++;}

		if(i == 5){
			if(flagCadrst == 1 ){
			bloqueiaTag(x);
			flagCadrst = 0;
			}
			return 1;
		}
		x++;
	}

	return 0;
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(podeLer == 0){
		timer++;
	}

	if(timer >= 1){
		podeLer = 1;
		timer = 0;
	}
}

//Verifica o GraphicMode do GLCD
void verificaGMode(uint8_t mode){
	// GMODE 0
	if(mode == 0 && flagGraph == 1){
		flagGraph = 0;
		ST7920_Clear();
		ST7920_GraphicMode(0);
	}
	else if(mode == 0 && flagGraph == 0){
		flagGraph = 0;
		ST7920_Clear();
	}

	// GMODE 1
	else if(mode == 1 && flagGraph == 0){
		flagGraph = 1;
		ST7920_Clear();
		ST7920_GraphicMode(1);
	}
	else if(mode == 1 && flagGraph == 1){
		flagGraph = 1;
		return;
	}
}

/* FIM FUNÇÕES RFID + LCD ------------------------------------------------------*/

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
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
  MX_SPI1_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_USART2_UART_Init();
  MX_USART4_UART_Init();
  /* USER CODE BEGIN 2 */
  //inicia
  HAL_TIM_Base_Start_IT(&htim2);
  delay_init();
  ST7920_Init();
  MFRC522_Init();

  HAL_GPIO_WritePin(led_s_GPIO_Port, led_s_Pin, 1);
  HAL_GPIO_WritePin(led_g_GPIO_Port, led_g_Pin, 1);
  HAL_GPIO_WritePin(led_r_GPIO_Port, led_r_Pin, 1);
  HAL_GPIO_WritePin(led_b_GPIO_Port, led_b_Pin, 1);


  status = Read_MFRC522(VersionReg);


 for (uint8_t x = 0; x < 10; ++x) {						// Limpa todas Tags do database
	  for (uint8_t y = 0; y < 5; ++y) {
		  login[x][y] = 0xFF;
	  }
 }
  ST7920_GraphicMode(1);
  ST7920_DrawBitmap(foto_mosfet);		// Inicia o display e aguarda a inicialização do ESP
  HAL_Delay(5000);

 if(Esp_Comandos()=='>') // Se estiver conectado com o outro módulo, liga o LED
  {
	  HAL_GPIO_WritePin(LED_PLACA_GPIO_Port, LED_PLACA_Pin, 1);
  }
  else
  {
	  while(1)	// Se houve algum erro na conexão, o programa fica travado e o LED fica piscando
	  {
		  HAL_GPIO_TogglePin(LED_PLACA_GPIO_Port, LED_PLACA_Pin);
		  HAL_Delay (250);
		  ST7920_DrawBitmap(kawaideado_recusada);
	  }
  }

  HAL_GPIO_WritePin(led_b_GPIO_Port, led_b_Pin, 0);			// Inicia o LED RGB
  HAL_Delay(500);
  HAL_GPIO_WritePin(led_b_GPIO_Port, led_b_Pin, 1);
  HAL_GPIO_WritePin(APITO_GPIO_Port, APITO_Pin, 1);			// Buzzer apita indicando o inicio
  HAL_Delay(25);
  HAL_GPIO_WritePin(APITO_GPIO_Port, APITO_Pin, 0);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {




	  //Lê a tag podendo retornar
	  leTag();

	  //Ativa o modo cadastro
	  if(flagCadrst == 1){
		  verificaGMode(0);

		  //Enquanto o modo cadastro estiver ativado, lê a tag que se aproxima
		  while(flagCadrst == 1){
			  leTag();
			  ST7920_SendString(1, 2, "Cadastro!");
			  ST7920_SendString(2, 0, "Aproxime a TAG");

			  //Verifica se está lendo alguma coisa
			  if(str[0] != 0x00){
				  if(comparaTag(str) == 1){		// chama a função, podendo ser 0 para novo cadastro | 1 para tag removida | 2 para o modo ler tensão | 3 para modo cadastro
					  verificaGMode(1);
					  ST7920_DrawBitmap(kawaideado_removido);

					  HAL_GPIO_WritePin(APITO_GPIO_Port, APITO_Pin, 1);
					  HAL_Delay(100);
					  HAL_GPIO_WritePin(APITO_GPIO_Port, APITO_Pin, 0);
					  HAL_Delay(400);

					  HAL_GPIO_WritePin(APITO_GPIO_Port, APITO_Pin, 1);
					  HAL_Delay(100);
					  HAL_GPIO_WritePin(APITO_GPIO_Port, APITO_Pin, 0);
					  HAL_Delay(700);
				  }
				  else if(comparaTag(str) == 0){
					  for (uint8_t j = 0; j < 5; ++j) {
						  login[qntsUser][j] = str[j];
					  }
					  qntsUser++;

					  verificaGMode(1);
					  ST7920_DrawBitmap(kawaideado_cadastro);

					  HAL_GPIO_WritePin(led_b_GPIO_Port, led_b_Pin, 0);
					  HAL_Delay(350);
					  HAL_GPIO_WritePin(led_b_GPIO_Port, led_b_Pin, 1);

					  HAL_GPIO_WritePin(led_b_GPIO_Port, led_b_Pin, 0);
					  HAL_Delay(350);
					  HAL_GPIO_WritePin(led_b_GPIO_Port, led_b_Pin, 1);
					  flagCadrst = 0;
				  }else if(comparaTag(str) == 2){
						if(flagCadrst == 1){
							flagCadrst = 0;
						}else if(flagCadrst == 0){
							flagCadrst = 1;
						}
				  }
			  limpaTag(str);
			  }
		  }
	  }else {
		  verificaGMode(1);
		  leTag();

		  while(flagCadrst == 0){		// se não houver tag sendo liga, sensor continua lendo
			  leTag();

			  if(str[0] != 0x00){

				  if(comparaTag(str) == 1){

					  	  // TRANSMISSÃO
						  Mensagem = 1;
						  HAL_UART_Transmit_IT(&huart4, &Mensagem, 1);
						  //HAL_Delay(100);


					  ST7920_DrawBitmap(kawaideado_open);



					  HAL_GPIO_WritePin(APITO_GPIO_Port, APITO_Pin, 1);
					  HAL_GPIO_WritePin(led_r_GPIO_Port, led_r_Pin, 1);
					  HAL_GPIO_WritePin(led_g_GPIO_Port, led_g_Pin, 0);

					  HAL_Delay(100);

					  HAL_GPIO_WritePin(APITO_GPIO_Port, APITO_Pin, 0);
					  HAL_GPIO_WritePin(led_g_GPIO_Port, led_g_Pin, 1);

					  HAL_Delay(50);

					  HAL_GPIO_WritePin(APITO_GPIO_Port, APITO_Pin, 1);
					  HAL_GPIO_WritePin(led_g_GPIO_Port, led_g_Pin, 0);


					  HAL_Delay(100);

					  HAL_GPIO_WritePin(APITO_GPIO_Port, APITO_Pin, 0);
					  HAL_GPIO_WritePin(led_g_GPIO_Port, led_g_Pin, 1);
					  HAL_Delay(600);

				  }else if(comparaTag(str) == 0){
					  ST7920_DrawBitmap(kawaideado_recusada);

					  // TRANSMISSÃO
					  Mensagem = 2;
					  HAL_UART_Transmit_IT(&huart4, &Mensagem, 1);
//					  HAL_Delay(100);


					  HAL_GPIO_WritePin(APITO_GPIO_Port, APITO_Pin, 1);
					  HAL_GPIO_WritePin(led_r_GPIO_Port, led_r_Pin, 0);
					  HAL_GPIO_WritePin(led_g_GPIO_Port, led_g_Pin, 1);

					  HAL_Delay(250);

					  HAL_GPIO_WritePin(APITO_GPIO_Port, APITO_Pin, 0);
					  HAL_GPIO_WritePin(led_r_GPIO_Port, led_r_Pin, 1);
					  HAL_Delay(600);
				  }
				  else if(comparaTag(str) == 2){
						if(flagCadrst == 1){
							flagCadrst = 0;
						}else if(flagCadrst == 0){
							flagCadrst = 1;
						}
				  }
			  }else{
				  verificaGMode(1);
				  ST7920_DrawBitmap(kawaideado_normal);
			  }
			  limpaTag(str);

		  }
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
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 64-1;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 0xffff-1;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 9999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 7200;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

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
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, Reset_Pin|EN_Pin|led_b_Pin|led_s_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, RC522_CS_Pin|LED_PLACA_Pin|DI_Pin|RST_Pin
                          |APITO_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, RW_Pin|IRQ_Pin|led_r_Pin|led_g_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : Reset_Pin EN_Pin led_b_Pin led_s_Pin */
  GPIO_InitStruct.Pin = Reset_Pin|EN_Pin|led_b_Pin|led_s_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : RC522_CS_Pin LED_PLACA_Pin DI_Pin RST_Pin
                           APITO_Pin */
  GPIO_InitStruct.Pin = RC522_CS_Pin|LED_PLACA_Pin|DI_Pin|RST_Pin
                          |APITO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : RW_Pin IRQ_Pin led_r_Pin led_g_Pin */
  GPIO_InitStruct.Pin = RW_Pin|IRQ_Pin|led_r_Pin|led_g_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

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

