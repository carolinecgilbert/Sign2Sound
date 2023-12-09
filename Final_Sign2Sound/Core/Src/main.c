/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
// OLED Includes
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define BUFFER_SIZE		15000

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */
// DAC PVs
uint16_t audio_buffer[BUFFER_SIZE] = {0};
volatile uint16_t audio_buffer_index = 0;
volatile uint8_t audio_byte_iter = 0;
volatile uint16_t audio_sample = 0;

// OLED PVs
volatile uint8_t rx_letter;
int wait = 1000000;

// Reset code
static uint8_t reset_code = '!';

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART3_UART_Init(void);

/* USER CODE BEGIN PFP */
// DAC Functions
void DAC_Handler(void);
HAL_StatusTypeDef spi_write(uint16_t * data);
void process_audio_data(void);

// OLED Functions
void OLED_Handler(void);
void setup_bb(void);
void small_delay(int t);
void bb_write_bit(int bit);
void bb_write_byte(int byte);
void bb_cmd(int cmd);
void bb_data(int data);
void bb_init_oled(void);
void bb_display1(const char * str);
void bb_clear_display(void);

// Software Reset
void software_reset(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */

  // Setup OLED and Display Instructions
  setup_bb();
  bb_init_oled();
  bb_display1("PRESS THE BUTTON TO START");

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  // Run the OLED Handler
	  OLED_Handler();
	  // Run the DAC Handler
	  DAC_Handler();

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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART3|RCC_PERIPHCLK_USART2;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
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
  hspi1.Init.DataSize = SPI_DATASIZE_16BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
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
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : SPI1_CS_Pin */
  GPIO_InitStruct.Pin = SPI1_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(SPI1_CS_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/**
  * @brief  DAC Handler to receive audio data and send to DAC
  * @retval None
  */
void DAC_Handler(void) {
	if ((USART3->ISR & USART_ISR_RXNE) == USART_ISR_RXNE)
	{
		uint8_t rx_audio_byte = (uint8_t)(USART3->RDR);

		// Save received MSB
		if (audio_byte_iter == 0) {
			audio_sample = rx_audio_byte << 8;
			audio_byte_iter = 1;
		}
		// Save received LSB
		else if (audio_buffer_index < BUFFER_SIZE) {
			audio_sample |= rx_audio_byte;
			audio_byte_iter = 0;

			// Store 16 bit element in buffer
			audio_buffer[audio_buffer_index] = audio_sample;
			audio_buffer_index += 1;
		}
	}

	// Handle overrun
	if (USART3->ISR & USART_ISR_ORE) {
	    USART3->ICR |= USART_ICR_ORECF;
	}
	// Framing error
	if (USART3->ISR & USART_ISR_FE) {
	    USART3->ICR |= USART_ICR_FECF;
	}

	// Send audio buffer to DAC if full
	if (audio_buffer_index == BUFFER_SIZE) {
		process_audio_data();

		// Display instructions on OLED after audio is done
		bb_clear_display();
		bb_display1("PRESS THE BUTTON TO START");
	}
}

/**
  * @brief  Write 16 bit sample to DAC via SPI1
  * @retval None
  */
HAL_StatusTypeDef spi_write(uint16_t * data) {
	HAL_StatusTypeDef ret;
	// Reset CS pin
	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET);

	// Transmit audio sample
	ret = HAL_SPI_Transmit(&hspi1, data, 2, HAL_MAX_DELAY);

	// Set CS pin
	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET);

	return ret;
}

/**
  * @brief  Send audio buffer to DAC and reset parameters
  * @retval None
  */
void process_audio_data(void) {
	// Iterate through audio buffer samples
	for (int i=0; i<BUFFER_SIZE; i++) {
		// Store next sample
		uint16_t * sample = &audio_buffer[i];

		// Reset sample in buffer
		audio_buffer[i] = 0;

		// Send every other sample to DAC
		if (i % 2 == 0) {
			HAL_StatusTypeDef spiHAL = spi_write(sample);

			// Error Handling
			if (spiHAL != HAL_OK) {
				Error_Handler();
			}
		}
	}

	// Reset buffer parameters
	audio_buffer_index = 0;
	audio_byte_iter = 0;
	audio_sample = 0;

}

void OLED_Handler(void) {
	// Receive transmitted letter
	if ((USART2->ISR & USART_ISR_RXNE) == USART_ISR_RXNE)
	{
		rx_letter = (uint8_t)(USART2->RDR);

		// Check for software reset
		if (rx_letter == reset_code) {
			software_reset();
			return;
		}

		// Clear instruction message
		bb_clear_display();

		// Send letter to OLED
		if ((rx_letter == 'A') || rx_letter == 'a') {
			bb_display1("TRANSLATION: A");
		}
		else if ((rx_letter == 'B') || rx_letter == 'b') {
			bb_display1("TRANSLATION: B");
		}
		else if ((rx_letter == 'C') || rx_letter == 'c') {
			bb_display1("TRANSLATION: C");
		}
		else if ((rx_letter == 'D') || rx_letter == 'd') {
			bb_display1("TRANSLATION: D");
		}
		else if ((rx_letter == 'E') || rx_letter == 'e') {
			bb_display1("TRANSLATION: E");
		}
		else if ((rx_letter == 'F') || rx_letter == 'f') {
			bb_display1("TRANSLATION: F");
		}
		else if ((rx_letter == 'G') || rx_letter == 'g') {
			bb_display1("TRANSLATION: G");
		}
		else if ((rx_letter == 'H') || rx_letter == 'h') {
			bb_display1("TRANSLATION: H");
		}
		else if ((rx_letter == 'I') || rx_letter == 'i') {
			bb_display1("TRANSLATION: I");
		}
		else if ((rx_letter == 'K') || rx_letter == 'k') {
			bb_display1("TRANSLATION: K");
		}
		else if ((rx_letter == 'L') || rx_letter == 'l') {
			bb_display1("TRANSLATION: L");
		}
		else if ((rx_letter == 'M') || rx_letter == 'm') {
			bb_display1("TRANSLATION: M");
		}
		else if ((rx_letter == 'N') || rx_letter == 'n') {
			bb_display1("TRANSLATION: N");
		}
		else if ((rx_letter == 'O') || rx_letter == 'o') {
			bb_display1("TRANSLATION: O");
		}
		else if ((rx_letter == 'P') || rx_letter == 'p') {
			bb_display1("TRANSLATION: P");
		}
		else if ((rx_letter == 'Q') || rx_letter == 'q') {
			bb_display1("TRANSLATION: Q");
		}
		else if ((rx_letter == 'R') || rx_letter == 'r') {
			bb_display1("TRANSLATION: R");
		}
		else if ((rx_letter == 'S') || rx_letter == 's') {
			bb_display1("TRANSLATION: S");
		}
		else if ((rx_letter == 'T') || rx_letter == 't') {
			bb_display1("TRANSLATION: T");
		}
		else if ((rx_letter == 'U') || rx_letter == 'u') {
			bb_display1("TRANSLATION: U");
		}
		else if ((rx_letter == 'V') || rx_letter == 'v') {
			bb_display1("TRANSLATION: V");
		}
		else if ((rx_letter == 'W') || rx_letter == 'w') {
			bb_display1("TRANSLATION: W");
		}
		else if ((rx_letter == 'X') || rx_letter == 'x') {
			bb_display1("TRANSLATION: X");
		}
		else if ((rx_letter == 'Y') || rx_letter == 'y') {
			bb_display1("TRANSLATION: Y");
		}
		else {
			bb_display1("INVALID LETTER");
		}
	}

	// Handle overrun and framing errors
	if (USART2->ISR & USART_ISR_ORE) {
		USART2->ICR |= USART_ICR_ORECF;
	}
	if (USART2->ISR & USART_ISR_FE) {
		USART2->ICR |= USART_ICR_FECF;
	}

}

//============================================================================
// setup_bb(), small_delay(), bb_write_bit(), bb_write_byte(), bb_cmd(),
// bb_data(), bb_init_oled(), bb_display1()
//
// Initialize and execute LCD OLED display
//============================================================================

void setup_bb(void){

	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
	//GPIOB->MODER &= 0x0;
	GPIOB->MODER |= GPIO_MODER_MODER12_0 | GPIO_MODER_MODER13_0 | GPIO_MODER_MODER15_0;
	//GPIOB->MODER |= 0xB000;

	// Initialize the ODR so that NSS (PB12) is high and SCK (PB13) is low
	GPIOB->ODR |= GPIO_ODR_12;
	GPIOB->ODR &= ~GPIO_ODR_13;
}

void small_delay(int t){
	nano_wait(t);
}


void bb_write_bit(int bit){

	// Set the MOSI pin to the value of the parameter (0 or 1)
	if (bit) {
		GPIOB->BSRR = GPIO_BSRR_BS_15;  // Set MOSI (PB15) to 1
	}
	else {
		GPIOB->BSRR = GPIO_BSRR_BR_15;  // Clear MOSI (PB15) to 0
	}

	small_delay(wait);

	// Set the SCK pin to high
	GPIOB->BSRR = GPIO_BSRR_BS_13;  // Set SCK (PB13) to high

	small_delay(wait);

	// Set the SCK pin to low
	GPIOB->BSRR = GPIO_BSRR_BR_13;  // Clear SCK (PB13) to low
}

void bb_write_byte(int byte){

	int bit;

	// for 8 bits
	for(int i = 7; i >= 0; i--){

		// get value of LSB
		bit = byte >> i;
		bit = bit & 0x1;

		// write bit to LCD
		bb_write_bit(bit);
	}
}


void bb_cmd(int cmd){

	// Set the NSS pin low to start an SPI transfer
	GPIOB->BSRR = GPIO_BSRR_BR_12;  // Clear NSS (PB12) to low
	small_delay(wait);

	// Send command bits: RS=0 (Register Select), R/W=0 (Write)
	bb_write_bit(0);
	bb_write_bit(0);

	// Send the command byte
	bb_write_byte(cmd);
	small_delay(wait);

	// Set the NSS pin high to signal the end of the SPI transfer
	GPIOB->BSRR = GPIO_BSRR_BS_12;  // Set NSS (PB12) to high
	small_delay(wait);
}

void bb_data(int data){

	// Set the NSS pin low to start an SPI transfer
	GPIOB->BSRR = GPIO_BSRR_BR_12;  // Clear NSS (PB12) to low
	small_delay(wait);

	// Send command bits: RS=0 (Register Select), R/W=0 (Write)
	bb_write_bit(1);
	bb_write_bit(0);

	// Send the command byte
	bb_write_byte(data);
	small_delay(wait);

	// Set the NSS pin high to signal the end of the SPI transfer
	GPIOB->BSRR = GPIO_BSRR_BS_12;  // Set NSS (PB12) to high
	small_delay(wait);
}


void bb_init_oled(void){

	// Wait for 1 ms for the display to power up and stabilize
	small_delay(2000000);

	// Initialize the OLED display with the specified commands
	bb_cmd(0x38);  // Set for 8-bit operation
	bb_cmd(0x08);  // Turn display off
	bb_cmd(0x01);  // Clear display

	// Wait for 2 ms for the display to clear
	small_delay(2000000);

	bb_cmd(0x06);  // Set the display to scroll
	bb_cmd(0x02);  // Move the cursor to the home position
	bb_cmd(0x0C);  // Turn the display on
}


void bb_display1(const char * str){
	bb_cmd(0x02); // move the cursor to the home position

	int strSize = strlen(str);
	int firstLine = 1;
	for(int i = 0; i < strSize; i++){
		if(*str != '\0'){
			// Check display lines for overflow
			if (i>0 && i%16==0) {
				// Move to second line of display
				if (firstLine==1) {
					firstLine = 0;
					bb_cmd(0x80 | 0x40);
				}
				// Clear display and move to first line
				else {
					firstLine = 1;
					bb_cmd(0x01);
					bb_cmd(0x02);
				}
				// Skip space if starting line
				if (*str == 32) {
					str++;
				}
			}
			// Write character
			bb_data(*str);
		}
		str++;
	}
}

void bb_clear_display(void) {
	// Clear OLED display
	bb_cmd(0x01);

	// Wait for 2 ms for the display to clear
	small_delay(2000000);

	// Move the cursor to the home position
	bb_cmd(0x02);
}

void software_reset(void) {
  // Set the system reset request bit
  HAL_NVIC_SystemReset();
}

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
