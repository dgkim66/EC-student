#include "stm32f411xe.h"
#include "ecSysTick.h"
#include "ecADC.h"
#include "ecGPIO.h"
#include "ecTIM.h"
#include <stdint.h>

uint32_t result;




void ADC_init(GPIO_TypeDef* port, int pin, int mode) {  //mode 0 : SW, 1 : TRGO
// 0. Match Port and Pin for ADC channel	
	uint32_t CHn = ADC_pinmap(port, pin);			// ADC Channel <->Port/Pin mapping

// GPIO configuration ---------------------------------------------------------------------	
// 1. Initialize GPIO port and pin as ANALOG, no pull up / pull down
	GPIO_init(port, pin, ANALOG);  				// ANALOG = 3
	GPIO_pudr(port, pin, EC_NONE);  			// EC_NONE = 0

// ADC configuration	---------------------------------------------------------------------			
// 1. Total time of conversion setting
	// Enable ADC pheripheral clock
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN; 		// Enable the clock of RCC_APB2ENR_ADC1EN

	// Configure ADC clock pre-scaler
	ADC->CCR &= ~(3UL << 16);					// 0001: PCLK2 divided by 4	(42MHz)
	ADC->CCR |=  (1UL << 16);

	// Configure ADC resolution 
	ADC1->CR1 &= ~(3UL << 24);     		// 00: 12-bit resolution (15cycle+)

	// Configure channel sampling time of conversion.	
	// Software is allowed to write these bits only when ADSTART=0 and JADSTART=0	!!
	// ADC clock cycles @42MHz = 2us
	ADC1->SMPR2  &= ~(7UL << 3);		// Sampling Time:  clear bits
	if (CHn < 10) ADC1->SMPR2 |= (4U << 3);					// sampling time conversion : 84  			
	else		      ADC1->SMPR1 |= (4U << 3);

// 2. Regular / Injection Group 
		//Regular: SQRx, Injection: JSQx

// 3. Repetition: Single or Continuous conversion
	ADC1->CR2 &= ~ADC_CR2_CONT;      			// Enable Continuous conversion mode	

// 4. Single Channel or Scan mode
	//  - Single Channel: scan mode, right alignment
	ADC1->CR1 |= ADC_CR1_SCAN;					// 1: Scan mode enable 
	//ADC1->CR2 &= ___________;   				// 0: Right alignment	--> Use Defalt

	// Configure the sequence length
	ADC1->SQR1 &= ~(15UL << 20); 					// 0000: 1 conversion in the regular channel conversion sequence

	// Configure the channel sequence 
	ADC1->SQR3 &= ~(0x1FUL << 0);				 	// SQ1 clear bits
	ADC1->SQR3 |=  (1UL << 0); 	         	// Choose the channel to convert firstly


// 5. Interrupt Enable
	// Enable EOC(conversion) interrupt. 
	ADC1->CR1 &= ~(1UL << 5);          	// Interrupt reset
	ADC1->CR1 |=  (1UL << 5);           // Interrupt enable

	// Enable ADC_IRQn 
	NVIC_SetPriority(ADC_IRQn, 2); 			// Set Priority to 2
	NVIC_EnableIRQ(ADC_IRQn);      	// Enable interrupt form ACD1 peripheral	

}



void ADC_start(ADC_TypeDef *ADCx) {
	// Enable ADON, SW Trigger-------------------------------------------------------------------------------
	ADCx->CR2 |= ADC_CR2_ADON;
	ADCx->CR2 |= ADC_CR2_SWSTART;
}


uint32_t is_ADC_EOC(ADC_TypeDef *ADCx){
	return ((ADCx->SR & ADC_SR_EOC) == ADC_SR_EOC);
}


uint32_t ADC_read(ADC_TypeDef *ADCx) {
	return ADCx->DR;
}

uint32_t ADC_pinmap(GPIO_TypeDef* Port, int Pin) {
	if (Port == GPIOA) {
		if (Pin == 0) 			return 0;
		else if (Pin == 1) return 1;
		else if (Pin == 4) return 4;
		else if (Pin == 5) return 5;
		else if (Pin == 6) return 6;
		else if (Pin == 7) return 7;
		else 							while (1);
	}
	else if (Port == GPIOB) {
		if (Pin == 0) 			return 8;
		else if (Pin == 1)	return 9;
		else 							while (1);
	}
	else if (Port == GPIOC) {
		if (Pin == 0)			return 10;
		else if (Pin == 1)	return 11;
		else if (Pin == 2)	return 12;
		else if (Pin == 3)	return 13;
		else if (Pin == 4)	return 14;
		else if (Pin == 5)	return 15;
		else							while (1);
	}
}

