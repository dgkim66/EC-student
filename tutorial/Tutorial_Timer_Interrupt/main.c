/**
******************************************************************************
* @author  SSSLAB
* @Mod		 2021-8-12 by YKKIM  	
* @brief   Embedded Controller:  Tutorial ___
*					 - _________________________________
* 
******************************************************************************
*/
#include "stm32f411xe.h"
#include "ecGPIO.h"
#include "ecRCC.h"
//#include "ecTIM.h"

#define LED_PIN 	5

uint32_t _count=0;

void setup(void);

void TIM2_IRQHandler(void);

void LED_toggle(void);
	
int main(void) { 
	// Initialiization --------------------------------------------------------
	setup();
	TIM_TypeDef* timerx;
	timerx = TIM2;
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	
	timerx->PSC = 840-1;							         // Timer counter clock: 1MHz(1us)
	timerx->ARR = 100-1;					 	   	    // Set auto reload register to maximum (count up to 65535)
	timerx->DIER |= TIM_DIER_UIE;                    // Enable Interrupt
	timerx->CR1 |= TIM_CR1_CEN;                     // Enable counter
	
	NVIC_EnableIRQ(TIM2_IRQn);				            // TIM2_IRQHandler Enable
	NVIC_SetPriority(TIM2_IRQn, 2);               // TIM2_IRQHandler Set priority as 2
	
	// Inifinite Loop ----------------------------------------------------------
	while(1){}
}

// Initialiization 
void setup(void)
{	
	RCC_PLL_init();                       // System Clock = 84MHz
	GPIO_init(GPIOA, LED_PIN, OUTPUT);    // calls RCC_GPIOA_enable()	
}

void TIM2_IRQHandler(void){
	if((TIM2->SR & TIM_SR_UIF) == TIM_SR_UIF){ // update interrupt flag
		//Create the code to toggle LED by 1000ms
		_count++;
		if(_count > 1000){
			LED_toggle();
			_count=0;
		}
		TIM2->SR &= ~TIM_SR_UIF;         // clear by writing 0
	}
	
}

void LED_toggle(void){
	
	//GPIO_write(GPIOA, LED_PIN, !GPIO_read(GPIOA, LED_PIN));
	
	GPIOA->ODR ^= 1UL << LED_PIN;
	
}