/*
******************************************************************************
* @author	Daegi Kim
* @Mod		2021-10-22
* @brief	Embedded Controller:  LAB External Interrupt 
*	
* 		
******************************************************************************
*/

#include "stm32f4xx.h"
#include "ecRCC.h"
#include "ecGPIO.h"
#include "ecEXTI.h"

#define LED_PIN     5
#define BUTTON_PIN  13
#define FALL				0x1UL
#define RISE				0x0UL

void setup(void);
void EXTIx_IRQHandler(int pin);
void EXTI15_10_IRQHandler(void);

void LED_toggle(void);
void LED_init(void);


int main(void) { 
	// Initialiization --------------------------------------------------------
	setup();
	
	EXTIx_IRQHandler(BUTTON_PIN);
	
	// Inifinite Loop ----------------------------------------------------------
	while(1){}
}

// Initialiization 
void setup(void) 
{	
	RCC_HSI_init();
	LED_init();	
	EXTI_init(GPIOC, BUTTON_PIN, FALL, 0);
	GPIO_init(GPIOC, BUTTON_PIN, INPUT);
	GPIO_pudr(GPIOC, BUTTON_PIN, PullDown);
}

void EXTI15_10_IRQHandler(void) {  
	if (is_pending_EXTI(BUTTON_PIN)) {
		LED_toggle();
		clear_pending_EXTI(BUTTON_PIN); // cleared by writing '1'
	}
}

void EXTIx_IRQHandler(int pin){
	
	if(pin <= 15 && pin >= 10) EXTI15_10_IRQHandler();
	//else if(pin == 0) EXTI0_IRQHandler();
	//else if(pin == 1) EXTI1_IRQHandler();
	//else if(pin == 2) EXTI2_IRQHandler();
	//else if(pin == 3) EXTI3_IRQHandler();
	//else if(pin == 4) EXTI4_IRQHandler();
	
}


void LED_init(){
	
	GPIO_init(GPIOA, LED_PIN, OUTPUT);
	GPIO_ospeed(GPIOA, LED_PIN, HighSpeed);
	GPIO_otype(GPIOA, LED_PIN, PushPull);
	GPIO_pudr(GPIOA, LED_PIN, PullUp);
	
}

void LED_toggle(void){
	
	//GPIO_write(GPIOA, LED_PIN, !GPIO_read(GPIOA, LED_PIN));
	
	GPIOA->ODR ^= 1UL << LED_PIN;
	
}


