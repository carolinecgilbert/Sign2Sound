
//=============================================================================
// ECE 362 lab experiment 6 -- Analog Input/Output
//=============================================================================

#include "stm32f0xx_hal.h"
#include "stm32f0xx.h"
#include <math.h>


// Be sure to change this to your login...
const char login[] = "xyz";
float xg, yg, zg;
int16_t x,y,z;
uint8_t RxData[6];

void internal_clock(void);
void display_float(float);
void control(void);

//============================================================================
// setup_adc()    (Autotest #1)
// Configure the ADC peripheral and analog input pins.
// Parameters: none
//============================================================================
void setup_adc(void)
{

}

//============================================================================
// start_adc_channel()    (Autotest #2)
// Select an ADC channel, and initiate an A-to-D conversion.
// Parameters: n: channel number
//============================================================================
void start_adc_channel(int n)
{

}

//============================================================================
// read_adc()    (Autotest #3)
// Wait for A-to-D conversion to complete, and return the result.
// Parameters: none
// Return value: converted result
//============================================================================
int read_adc(void)
{
    return /* replace with something meaningful */ 0;
}

//============================================================================
// setup_dac()    (Autotest #4)
// Configure the DAC peripheral and analog output pin.
// Parameters: none
//============================================================================
void setup_dac(void)
{
  // Enable the clock to GPIO Port A
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

  // Change the configuration only for pin 4 for digital operation
  GPIOA->MODER &= ~(GPIO_MODER_MODER4); // Clear the mode bits for pin 4
  GPIOA->MODER |= GPIO_MODER_MODER4_0;  // Set pin 4 to digital output mode


  // Enable the RCC clock for the DAC
  __HAL_RCC_DAC1_CLK_ENABLE();

  // Select a software trigger for the pin
  HAL_GPIOEx_ConfigPinLock(GPIOA, GPIO_PIN_4);

  // Enable the trigger for the pin
  HAL_GPIO_LockPin(GPIOA, GPIO_PIN_4);

  // Enable the pin (optional, you can remove this line if you want to enable it later)
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);



  // Enable RCC clock for the pin (if necessary, some pins may not require this step)
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

  // Select a software trigger for the pin (if necessary, configure as needed)
  //GPIOA->ODR |= GPIO_PIN_4;

  // Enable the trigger for the pin (if necessary, configure as needed)
  // ...

  // Enable the pin
  GPIOA->BSRR = GPIO_BSRR_BS_4;
}


//============================================================================
// write_dac()    (Autotest #5)
// Write a sample to the right-aligned 12-bit DHR, and trigger conversion.
// Parameters: sample: value to write to the DHR
//============================================================================
void write_dac(int sample)
{

}



void SPIConfig (void)
{
  RCC->APB2ENR |= (1<<12);  // Enable SPI1 CLock

  SPI1->CR1 |= (1<<0)|(1<<1);   // CPOL=1, CPHA=1

  SPI1->CR1 |= (1<<2);  // Master Mode

  SPI1->CR1 |= (3<<3);  // BR[2:0] = 011: fPCLK/16, PCLK2 = 80MHz, SPI clk = 5MHz

  SPI1->CR1 &= ~(1<<7);  // LSBFIRST = 0, MSB first

  SPI1->CR1 |= (1<<8) | (1<<9);  // SSM=1, SSi=1 -> Software Slave Management

  SPI1->CR1 &= ~(1<<10);  // RXONLY = 0, full-duplex

  SPI1->CR1 &= ~(1<<11);  // DFF=0, 8 bit data

  SPI1->CR2 = 0;
}


void SPI_Transmit (uint8_t *data, int size)
{

	/************** STEPS TO FOLLOW *****************
	1. Wait for the TXE bit to set in the Status Register
	2. Write the data to the Data Register
	3. After the data has been transmitted, wait for the BSY bit to reset in Status Register
	4. Clear the Overrun flag by reading DR and SR
	************************************************/

	int i=0;
	while (i<size)
	{
	   while (!((SPI1->SR)&(1<<1))) {};  // wait for TXE bit to set -> This will indicate that the buffer is empty
	   SPI1->DR = data[i];  // load the data into the Data Register
	   i++;
	}


/*During discontinuous communications, there is a 2 APB clock period delay between the
write operation to the SPI_DR register and BSY bit setting. As a consequence it is
mandatory to wait first until TXE is set and then until BSY is cleared after writing the last
data.
*/
	while (!((SPI1->SR)&(1<<1))) {};  // wait for TXE bit to set -> This will indicate that the buffer is empty
	while (((SPI1->SR)&(1<<7))) {};  // wait for BSY bit to Reset -> This will indicate that SPI is not busy in communication

	//  Clear the Overrun flag by reading DR and SR
	uint8_t temp = SPI1->DR;
					temp = SPI1->SR;

}


void GPIOConfig (void)
{
	RCC->AHB1ENR |= (1<<0);  // Enable GPIO Clock

	GPIOA->MODER |= (2<<10)|(2<<14)|(1<<18);  // Alternate functions for PA5, PA7 and Output for PA9

	GPIOA->OSPEEDR |= (3<<10)|(3<<14)|(3<<18);  // HIGH Speed for PA5, PA7, PA9

	GPIOA->AFR[0] |= (5<<20)|(5<<28);   // AF5(SPI1) for PA5 and PA7
}


void SPI_Disable (void)
{
	SPI1->CR1 &= ~(1<<6);   // SPE=0, Peripheral Disabled
}

void CS_Enable (void)
{
	GPIOA->BSRR |= (1<<9)<<16;
}

void CS_Disable (void)
{
	GPIOA->BSRR |= (1<<9);
}


void adxl_write (uint8_t address, uint8_t value)
{
	uint8_t data[2];
	data[0] = address|0x40;  // multibyte write
	data[1] = value;
	CS_Enable ();  // pull the cs pin low
	SPI_Transmit (data, 2);  // write data to register
	CS_Disable ();  // pull the cs pin high
}

void adxl_init (void)
{
	adxl_write (0x31, 0x01);  // data_format range= +- 4g
	adxl_write (0x2d, 0x00);  // reset all bits
	adxl_write (0x2d, 0x08);  // power_cntl measure and wake up 8hz
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

}

//============================================================================
// Global variables used for four-channel synthesis.
//============================================================================
int volume = 2048;
int stepa = 0;
int stepb = 0;
int stepc = 0;
int stepd = 0;
int offseta = 0;
int offsetb = 0;
int offsetc = 0;
int offsetd = 0;

//============================================================================
// set_freq_n()    (Autotest #7)
// Set the four step and four offset variables based on the frequency.
// Parameters: f: The floating-point frequency desired.
//============================================================================
void set_freq_a(float f)
{

}

void set_freq_b(float f)
{

}

void set_freq_c(float f)
{

}

void set_freq_d(float f)
{

}

//============================================================================
// Timer 6 ISR    (Autotest #8)
// The ISR for Timer 6 which computes the DAC samples.
// Parameters: none
// (Write the entire subroutine below.)
//============================================================================


//============================================================================
// setup_tim6()    (Autotest #9)
// Configure Timer 6 to raise an interrupt RATE times per second.
// Parameters: none
//============================================================================
void setup_tim6(void)
{

}

int main(void)
{
	SysClockConfig ();
	GPIOConfig ();
	TIM6Config ();
	SPIConfig ();

	SPI_Enable ();

	adxl_init ();

	while (1)
	{
		adxl_read (0x32);
		x = ((RxData[1]<<8)|RxData[0]);
		y = ((RxData[3]<<8)|RxData[2]);
		z = ((RxData[5]<<8)|RxData[4]);

		xg = x*.0078;
		yg = y*.0078;
		zg = z*.0078;

		Delay_ms (500);
	}


    //internal_clock(); // Use the internal oscillator if you need it
    //autotest(); // test all of the subroutines you wrote
    init_wavetable();
    setup_dac();
    setup_adc();
    setup_tim6();
    set_freq_a(261.626); // Middle 'C'
    set_freq_b(329.628); // The 'E' above middle 'C'
    //control(); // uncomment to enable user interface
    while(1) {
        for(int out=0; out<4096; out++) {
            if ((TIM6->CR1 & TIM_CR1_CEN) == 0)
                write_dac(out);
            start_adc_channel(1);
            int sample = read_adc();
            float level = 2.95 * sample / 4095;
            display_float(level);
        }
    }
}
