/*
******************************************************************************
* @author	Daegi Kim
* @Mod		2021-10-13   	
* @brief	Embedded Controller:  LAB 7-segment 
*					- 7 segment decoder
* 				- PA5, PA6, PA7, PB6, PC7, PA9, PA8, PB10  for DOUT
******************************************************************************
*/

#include "stm32f4xx.h"
#include "ecRCC.h"
#include "ecGPIO.h"

#define LED_PIN    5
#define BUTTON_PIN 13

void setup(void);
	
int main(void) { 
	
	// Initialiization --------------------------------------------------------	
	setup();	
	unsigned int cnt = 0;		
	int LEDflag = 1;
	
	// Inifinite Loop ----------------------------------------------------------	
	while(1){		
		if(LEDflag == 1){
			sevensegment_decode(cnt % 10);		
			LEDflag = 0;
		}
		if(GPIO_read(GPIOC, BUTTON_PIN) == 0) {
			cnt++;
			LEDflag = 1;
			while(GPIO_read(GPIOC, BUTTON_PIN) == 0) {;}
		}
		if (cnt > 9) cnt = 0;		
		for(int i = 0; i < 500000;i++){}	
	}
	
	
}


// Initialiization 
void setup(void) 
{
	RCC_HSI_init();		                         // Set Clock
	GPIO_init(GPIOC, BUTTON_PIN, INPUT);       // calls RCC_GPIOC_enable()
	GPIO_pudr(GPIOC, BUTTON_PIN, PullUp);      // Make Button Mode as Pull Up
	sevensegment_init();
}



