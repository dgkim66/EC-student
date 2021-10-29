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
//#include "ecPWM.h"
#include "ecSysTick.h"
//#include "ecEXTI.h"

#define LED_PIN 	1

void setup(void);
	
int main(void) { 
	// Initialiization --------------------------------------------------------
	RCC_PLL_init();                         // System Clock = 84MHz
	SysTick_init(1);                         // for delay_ms()
	GPIO_init(GPIOA, LED_PIN, AF);     // GPIOA 5 ALTERNATE function
	GPIO_ospeed(GPIOA, LED_PIN, HighSpeed);   // GPIOA 5 HIGH SPEED
	
	// TEMP: TIMER Register Initialiization --------------------------------------------------------		
	TIM_TypeDef *TIMx;
	TIMx = TIM2;
	
	uint32_t TIM2_Pos = 0;
	
	// GPIO: ALTERNATIVE function setting
	GPIOA->AFR[0] = 1UL << (LED_PIN * 4); 	              // AF1 at PA5 = TIM2_CH1 (p.150)
	// AFRL ===== AFR[0]
	// AFRH ===== AFR[1]
	
	// TIMER: PWM setting
	RCC->APB1ENR |= 1UL<<TIM2_Pos;                               // Enable TIMER clock
	
	TIMx->CR1 &= ~(1<<TIM2_Pos);                               // Direction Up-count
	
	TIMx->PSC = 840-1;			                            // f_cnt = 10kHz
	
	TIMx->ARR = 100-1;    				                      // Auto-reload: Upcounting (0...ARR).    
	
	TIMx->CCMR1 &= ~TIM_CCMR1_OC1M;                     // Clear ouput compare mode bits for channel 1
	TIMx->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2; // OC1M = 110 for PWM Mode 1 output on ch1
	TIMx->CCMR1	|= TIM_CCMR1_OC1PE;                     // Output 1 preload enable (make CCR1 value changable)
			
	TIMx->CCER &= ~TIM_CCER_CC1P;                       // select output polarity: active high	
	TIMx->CCER |= TIM_CCER_CC1E;        	              // Enable output for ch1
	TIMx->CCR1 = (TIMx->ARR)/2;									        // Output Compare Register for channel 1 
	
	TIMx->CR1  |= TIM_CR1_CEN;                          // Enable counter

	
	// Inifinite Loop ----------------------------------------------------------
	while(1){
			//Create the code to change the brightness of LED as 10kHZ (use "delay(100)")
		for(int i = 0; i < 3; i++){
				TIMx->CCR1 = 0.5 * i;
				delay_ms(100);
		}
	}
}
// Initialiization 
void setup(void)
{	
	RCC_PLL_init();                         // System Clock = 84MHz
	SysTick_init(1);                        // for delay_ms()
}


