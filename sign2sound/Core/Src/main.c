
//===========================================================================
// ECE 362 lab experiment 8 -- SPI and DMA
//===========================================================================

#include "stm32f0xx.h"
#include "lcd.h"
#include <stdio.h> // for sprintf()

// Be sure to change this to your login...
const char login[] = "xyz";

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

void bb_display2(const char * a){ }
void setup_spi1(){ }
void setup_spi2(){ }
void spi_display1(const char * b){ }
void spi_display2(const char * c){ }

// Write your subroutines above

void internal_clock();
void demo();
void autotest();

extern const Picture *image;

int main(void)
{
    //internal_clock();
    //demo();
    //autotest();

    setup_bb();
    bb_init_oled();
    bb_display1("ASL is cool");
    //bb_display2(login);

    //setup_spi2();
    //spi_init_oled();
    //spi_display1("Hello again,");
    //spi_display2(login);

    short buffer[34] = {
            0x02, // This word sets the cursor to beginning of line 1.
            // Line 1 consists of spaces (0x20)
            0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220,
            0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220,
            0xc0, // This word sets the cursor to beginning of line 2.
            // Line 2 consists of spaces (0x20)
            0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220,
            0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220,
    };

    //spi_setup_dma(buffer);
    //enable_dma();
    show_counter(buffer);

    //setup_spi1();
    LCD_Init(0,0,0);
    LCD_Clear(BLACK);
    LCD_DrawLine(10,20,100,200, WHITE);
    LCD_DrawRectangle(10,20,100,200, GREEN);
    LCD_DrawFillRectangle(120,20,220,200, RED);
    LCD_Circle(50, 260, 50, 1, BLUE);
    LCD_DrawFillTriangle(130,130, 130,200, 190,160, YELLOW);
    LCD_DrawChar(150,155, BLACK, WHITE, 'X', 16, 1);
    LCD_DrawString(140,60,  WHITE, BLACK, "ECE 362", 16, 0);
    LCD_DrawString(140,80,  WHITE, BLACK, "has the", 16, 1);
    LCD_DrawString(130,100, BLACK, GREEN, "best toys", 16, 0);
    LCD_DrawPicture(110,220,(const Picture *)&image);
}
