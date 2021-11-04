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
#include "math.h"
#include "ecGPIO.h"
#include "ecRCC.h"
#include "ecTIM.h"
#include "ecUART.h"

uint32_t ovf_cnt = 0;
uint32_t ccr2 = 0;
double   period = 0;

int main(void){
	
	RCC_PLL_init(); 
	UART2_init();
  
	// GPIO configuration ---------------------------------------------------------------------
	GPIO_init(GPIOA, 5, AF);	// PB10: Alternate Function Mode
	// TIM configuration -----------------------------------------------------------------------
	GPIOA->AFR[0] &= ~(15UL << 0);
	GPIOA->AFR[0] |=  (1UL << 0);                //AF1(TIM2)
	RCC->APB1ENR |= 1 << 0; 		                 //TIM2 Clock enabled

	TIM2->PSC = 84; 													   // Timer counter clock: 1M`Hz(1us)
	TIM2->ARR = TIM2->PSC - 1;							     // Set auto reload register to maximum (count up to 65535)
	TIM2->CR1 &= ~(1UL << 4); 			             // CR1 DIR enabled
	TIM2->CCMR1 &= ~(3UL << 0);
	TIM2->CCMR1 |=  (1UL << 0);                  // Capture/Compare Selection: CC1 is mapped on TI1 
	TIM2->CCMR1 &= ~(15UL << 4);                 // Clear IC1F
	TIM2->CCMR1 |=  (2UL << 4);                  // Set filter N=4
	TIM2->CCER  &=  ~(5UL << 1);                 // Clear CCER
	TIM2->CCER  |= 	(1UL << 1);                  // Capture rising edge
	TIM2->CCER |= (1UL << 0); 			             // Capture enabled
	TIM2->DIER |= (1UL << 1); 						       // Interrupt enabled
	TIM2->DIER |= (1UL << 0);				             // Update interrupt enable	
	TIM2->CR1  |= (1UL << 0);				             // Counter enable

	
	NVIC_SetPriority(TIM2_IRQn, 2);						     // Set the priority of TIM2 interrupt request
	NVIC_EnableIRQ(TIM2_IRQn);									   // TIM2 interrupt request enable

	while(1);
}

void TIM2_IRQHandler(void){
	if(TIM2->SR & TIM_SR_UIF){                     // Update interrupt
		TIM2->SR &= ~(TIM_SR_UIF);		               // clear update interrupt flag
		//User code to handle overflow
		// ...
	}
	if((TIM2->SR & TIM_SR_CC1IF) != 0){ 
		// User code to calculate the period of 1Hz pulse
		//...
		printf("%f[sec]\r\n", period);               // print out the period on TeraTerm
		TIM2->SR &= ~(TIM_SR_CC1IF);				         // clear capture/compare interrupt flag ( it is also cleared by reading TIM2_CCR1)
	}
}