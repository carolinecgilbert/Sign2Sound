
//===========================================================================
// ECE 362 lab experiment 8 -- SPI and DMA
//===========================================================================

#include "main.h"
#include "stm32f0xx.h"
#include "lcd.h"
#include <stdio.h> // for sprintf()
#include <math.h>

// Be sure to change this to your login...
const char login[] = "xyz";
SPI_HandleTypeDef hspi1;

// Prototypes for miscellaneous things in lcd.c
//void nano_wait(unsigned int){};

// Write your subroutines below

int wait = 1000000;

void setup_bb(){

	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
	//GPIOB->MODER &= 0x0;
	GPIOB->MODER |= GPIO_MODER_MODER12_0 | GPIO_MODER_MODER13_0 | GPIO_MODER_MODER15_0;
	//GPIOB->MODER |= 0xB000;

	// Initialize the ODR so that NSS (PA12) is high and SCK (PA13) is low
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

	for(int i = 0; i < sizeof(str); i++){
		if(*str != '\0'){
			bb_data(*str);
		}
		str++;
	}
}

void show_counter(short buffer[])
{
    for(int i=0; i<10000; i++) {
        char line[17];
        sprintf(line,"% 16d", i);
        for(int b=0; b<16; b++)
            buffer[1+b] = line[b] | 0x200;
    }
}

void spi_cmd(unsigned int data){
	while(!(SPI1->SR & SPI_SR_TXE)) {}
	SPI1->DR = data;
}

void spi_data(unsigned int data){
	spi_cmd(data | 0x200);
}


void spi_dac1(const char * b){ }


void bb_display2(const char * a){ }
void setup_spi2(){ }
void spi_display2(const char * c){ }


void setup_spi1(void)
{
	hspi1.Instance = SPI1;
	hspi1.Init.Mode = SPI_MODE_MASTER;
	hspi1.Init.Direction = SPI_DIRECTION_2LINES;
	hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi1.Init.NSS = SPI_NSS_SOFT;
	hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
	hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi1.Init.CRCPolynomial = 7;
	hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
	hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;

	if (HAL_SPI_Init(&hspi1) != HAL_OK){
		Error_Handler();
	}
}

void setup_CS(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOA_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);

	/*Configure GPIO pin : PA4 */
	GPIO_InitStruct.Pin = GPIO_PIN_4;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}


//============================================================================
// write_dac()    (Autotest #5)
// Write a sample to the right-aligned 12-bit DHR, and trigger conversion.
// Parameters: sample: value to write to the DHR
//============================================================================

void write_dac(uint16_t value) {

	uint8_t txData[2];

	txData[0] = (value >> 8) & 0xFF;
	txData[1] = value & 0xFF;

    // Set SPI1 NSS (Chip Select) pin to low to select the DAC
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);

    // Send the 16-bit integer to the DAC over SPI1
    HAL_SPI_Transmit(&hspi1, txData, 2, HAL_MAX_DELAY);

    // Set SPI1 NSS (Chip Select) pin back to high to complete the transaction
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
}

HAL_StatusTypeDef spi_write(uint16_t *pData) {
	HAL_StatusTypeDef ret;

	uint8_t byte1 = *pData >> 8;
	uint8_t byte2 = *pData & 0xFF;
	uint8_t sendData[2] = {byte1, byte2};

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 0);
	ret = HAL_SPI_Transmit(&hspi1, sendData, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 1);

	if(ret != HAL_OK){
		return ret;
	}

	return HAL_OK;
}


//============================================================================
// Parameters for the wavetable size and expected DAC rate.
//============================================================================
#define N 1000
#define RATE 20000
short int wavetable[N];

//============================================================================
// init_wavetable()    (Autotest #6)
// Write the pattern for one complete cycle of a sine wave into the
// wavetable[] array.
// Parameters: none
//============================================================================
void init_wavetable(void)
{
	for(int i=0; i < N; i++)
		wavetable[i] = 32767 * sin(2 * M_PI * i / N);

}

//============================================================================
// Global variables used for four-channel synthesis.
//============================================================================
int volume = 2048;
int stepa = 0;
int offseta = 0;

//============================================================================
// set_freq_n()    (Autotest #7)
// Set the four step and four offset variables based on the frequency.
// Parameters: f: The floating-point frequency desired.
//============================================================================
void set_freq_a(float f)
{
    if (f == 0.0f) {
        stepa = 0;
        offseta = 0;
    }
    else {
        stepa = (int32_t)(f * N * (1 << 16) / RATE);
    }
}

void interrupt(void){
	offseta += stepa;

	if (offseta >= (1 << 16)) {
		offseta -= (1 << 16);
	}

	int32_t sample = wavetable[offseta >> 16];

	// Reduce and shift the combined sample to the range of the DAC
	sample >>= 5;      // Right-shift by 5 (dividing by 32)
	sample += 2048;    // Add 2048

	// Clip the adjusted sample to the DAC's range
	if (sample > 4095) {
		sample = 4095;
	}
	if (sample < 0) {
		sample = 0;
	}

	spi_write(sample);

}


void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

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
}

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

// Write your subroutines above

void internal_clock();

int main(void)
{
	HAL_Init();
	SystemClock_Config();

    setup_bb();
    bb_init_oled();
    bb_display1("ASL is cool");
    // bb_display2(login);


    /*short buffer[34] = {
            0x02, // This word sets the cursor to beginning of line 1.
            // Line 1 consists of spaces (0x20)
            0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220,
            0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220,
            0xc0, // This word sets the cursor to beginning of line 2.
            // Line 2 consists of spaces (0x20)
            0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220,
            0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220,
    };*/

    //spi_setup_dma(buffer);
    //enable_dma();
    //show_counter(buffer);


    uint16_t dataOut = 0x0000;
    int iter = 0;

    setup_CS();
    setup_spi1();

    while(1){

    	if (iter==1) {
        	for(int i = 1; i < 16; i++){
        		spi_write(&dataOut);
        		HAL_Delay(100);
        		dataOut += 0x1000;
        	}
			iter = 0;
		}
		else {
	    	for(int i = 16; i > 0; i--){
	    		spi_write(&dataOut);
	    		HAL_Delay(100);
	    		dataOut -= 0x1000;
	    	}
			iter++;
		}
    }


	/*init_wavetable();
	//spi_init_dac();
	//spi_display1("file.wav");
	set_freq_a(261.626); // Middle 'C'
	while(1) {
		for(int out=0; out<4096; out++) {
			interrupt();
			//start_adc_channel(1);
			//int sample = read_adc();
			//float level = 2.95 * sample / 4095;
			//display_float(level);
		}
	}
	int iter = 0;
	while(1) {

		if (iter==1) {
			write_dac(100);
			iter = 0;
		}
		else {
			iter++;
			write_dac(0);
		}
		nano_wait(10000000);
		//for(int out=0; out<4096; out++) { }
	}*/

}
