/**
******************************************************************************
* @author  SSSLAB
* @Mod		 2021-10-15 by Daegi Kim
* @brief   Embedded Controller:  Tutorial_EXTI_SysTick
*					 - _________________________________
*
******************************************************************************
*/




#include "ecRCC.h"
#include "ecGPIO.h"

#define LED_PIN 	5
#define BUTTON_PIN 13

void setup(void);

void EXTI15_10_IRQHandler(void);

void LED_toggle(void);

int main(void) {

	// System CLOCK, GPIO Initialiization ----------------------------------------
	setup();


	// EXTI Initialiization ------------------------------------------------------	

	// SYSCFG peripheral clock enable
	RCC->APB2ENR |= 1<<14;

	// Connect External Line to the GPIO
	
	
	// Button: PC_13 -> EXTICR3(EXTI13)
	SYSCFG->EXTICR[3] &= ~SYSCFG_EXTICR4_EXTI13; // Clear
	SYSCFG->EXTICR[3] |= 2UL << 4;                 // 

	// Falling trigger enable (Button: pull-up)
	EXTI->FTSR |= 1UL << 13;

	// Unmask (Enable) EXT interrupt
	EXTI->IMR |= 1UL << 13;

	// Interrupt IRQn, Priority
	NVIC_SetPriority(EXTI15_10_IRQn, 0);  		// Set EXTI priority as 0	
	NVIC_EnableIRQ(EXTI15_10_IRQn); 			// Enable EXTI 


	EXTI15_10_IRQHandler();
	
	while (1);
}


void EXTI15_10_IRQHandler(void) {
	if ((EXTI->PR & EXTI_PR_PR13) == EXTI_PR_PR13) {
		LED_toggle();
		EXTI->PR |= EXTI_PR_PR13; // cleared by writing '1'
	}
}


// Initialiization 
void setup(void)
{
	RCC_PLL_init();                         // System Clock = 84MHz
	// Initialize GPIOA_5 for Output
	GPIO_init(GPIOA, LED_PIN, OUTPUT);    // calls RCC_GPIOA_enable()	
	// Initialize GPIOC_13 for Input Button
	GPIO_init(GPIOC, BUTTON_PIN, INPUT);  // calls RCC_GPIOC_enable()
	GPIO_pudr(GPIOC, BUTTON_PIN, PullDown);
}

void LED_toggle(void){
	
	//GPIO_write(GPIOA, LED_PIN, !GPIO_read(GPIOA, LED_PIN));
	
	GPIOA->ODR ^= 1UL << LED_PIN;
	
}
