

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
#include "ecTIM.h"
#include "ecSysTick.h"
#include "ecUART.h"
#include "ecADC.h"

uint32_t result_v =0;
uint32_t print_flag = 0;


void setup(void);
	
int main(void) { 
	// Initialiization --------------------------------------------------------
	setup();
	
	/* ADC_initiation --------------------------------------------------------------------*/
  // 1) Enable the clock of ADC
  // 2) ...
	// 3) ...
  
// HW Trigger configuration -------------------------------------------------------------
// 1. TIMx Trigger Output Config
	// Enable TIMx Clock
	TIM_TypeDef *timx = TIM3;			// Default TRGO : TIM3
	uint32_t msec = 1;									// Default msec : 1 msec
	TIM_init(timx, msec);					// TIM3 init (use user defined HAL)
	timx->CR1 &= ~(1UL); 							// counter disable
	
	// Set PSC, ARR
  TIM_period_ms(timx, msec);
	
  // Master Mode Selection MMS[2:0]: Trigger output (TRGO)
  timx->CR2 &= ~(3UL << 4);		// reset MMS
  timx->CR2 |=  (4UL << 4);		//100: Compare - OC1REF signal is used as trigger output (TRGO)
   
	// Output Compare Mode
  timx->CCMR1 &= ~(7UL << 4);	// OC1M : output compare 1 Mode 
  timx->CCMR1 |=  (6UL << 4); // OC1M = 110 for compare 1 Mode ch1 
	
  // OC1 signal 
  timx->CCER |= TIM_CCER_CC1E;// CC1E Capture enabled
	timx->CCR1  = 							// CCR set
   
  // Enable TIMx 
  timx->CR1 |= 1; 						//counter enable
	
// 2. HW Trigger
	// Select Trigger Source
	ADC1->CR2 &= ~(0x0FUL << 24);  // reset EXTSEL
	ADC1->CR2 |= ADC_CR2_EXTSEL_3; // TIMx TRGO event (ADC : TIM2, TIM3 TRGO) default : TIM3   = 1000
	
	//Select Trigger Polarity 
	ADC1->CR2 &= ~ADC_CR2_EXTEN;    // reset EXTEN, default
	ADC1->CR2 |= 	ADC_CR2_EXTEN_0;	// trigger detection rising edge
	
	// Enable ADC 
	ADC1->CR2 |= ADC_CR2_ADON;
	
  // ADC Start (SW, once)	  
  ADC1->CR2 |= ADC_CR2_SWSTART;

	/* Inifinite Loop --------------------------------------------------------------------*/
	while(1){
		if(print_flag == 1){
			printf("voltage = %.3f\r\n",result_v*3.3/4095);
			delay_ms(1000);
			print_flag = 0;
		}
	}
}

// Initialiization 
void setup(void)
{	
	RCC_PLL_init();                 // System Clock = 84MHz
	UART2_init();
	GPIO_init(GPIOA, 1, ANALOG);    // calls RCC_GPIOA_enable()
	GPIO_pudr(GPIOA, 1, EC_NONE);
}

void ADC_IRQHandler(void){		
	if((ADC1->SR & ADC_SR_OVR) == ADC_SR_OVR){
		ADC1->SR &= ~ADC_SR_OVR;
	}
	
	if(is_ADC_EOC(ADC1)){       			
	  result_v = ADC_read(ADC1);
		print_flag = 1;
	
 }
}