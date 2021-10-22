/**
******************************************************************************
* @author  SSSLAB
* @Mod		 2021-10-22 by Daegi Kim
* @brief   Embedded Controller:  LAB_EXTI_SysTick
*
*
******************************************************************************
*/


#include "ecRCC.h"
#include "ecGPIO.h"
#include "ecEXTI.h"
#include "ecSysTick.h"

#define LED_PIN    	5
#define BUTTON_PIN  13


void setup(void);

void EXTI15_10_IRQHandler(void);

int count = 0;// Initialiization 

int main(void) { 	
	
	
	// Initialiization --------------------------------------------------------		
	setup();		
	
	EXTI15_10_IRQHandler();
	SysTick_Handler();
	
	// Inifinite Loop ----------------------------------------------------------	
	while(1){		
		sevensegment_decode(count);
		delay_ms(1000);
		count++;
		if (count == 10) count = 0;
		SysTick_reset();	
	}

}


void setup(void){	
	RCC_PLL_init();	
	SysTick_init(1);	
	EXTI_init(GPIOC, BUTTON_PIN, FALL, 0);
	GPIO_init(GPIOC, BUTTON_PIN, INPUT);
	GPIO_pudr(GPIOC, BUTTON_PIN, PullUp);
	sevensegment_init();
}


void EXTI15_10_IRQHandler(void) {  
	if (is_pending_EXTI(BUTTON_PIN)) {
		count = 0;
		clear_pending_EXTI(BUTTON_PIN); // cleared by writing '1'
	}
}

