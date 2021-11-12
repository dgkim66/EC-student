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
	setup();
// GPIO configuration ---------------------------------------------------------------------	
// 1. Initialize GPIO port and pin as ANALOG, no pull up / pull down
	GPIO_init(GPIOA, 1, ANALOG);				// ANALOG = 3
	GPIO_pudr(GPIOA, 1, EC_NONE);			// EC_NONE = 0
	
// ADC configuration	---------------------------------------------------------------------			
// 1. Total time of conversion setting
	// Enable ADC pheripheral clock	
	RCC->APB2ENR  |= RCC_APB2ENR_ADC1EN;	// Enable the clock of RCC_APB2ENR_ADC1EN
	
	// Configure ADC clock pre-scaler
	ADC->CCR &= ~(3UL << 16);							// 0000: PCLK2 divided by 2	(42MHz)
	ADC->CCR |= 3UL << 16;
	
	// Configure ADC resolution 
	ADC1->CR1 &= ~(3UL << 24);           	// 00: 12-bit resolution (15cycle+)
	
	// Configure channel sampling time of conversion.	
	// Software is allowed to write these bits only when ADSTART=0 and JADSTART=0	!!
	// ADC clock cycles @42MHz = 2us
	ADC1->SMPR2  &= ~(7UL << 3);		// Sampling Time:  clear bits
	ADC1->SMPR2  |=  (4UL << 3); 		// Sampling Time:  84cycles/42Mhz)=2us
	   
// 2. Regular / Injection Group 
	//Regular: SQRx, Injection: JSQx
	
// 3. Repetition: Single or Continuous conversion
	ADC1->CR2 |= ADC_CR2_CONT;      // Enable Continuous conversion mode	
 
// 4. Single Channel or Scan mode
	//  - Single Channel: scan mode, right alignment	
	ADC1->CR1 |=  ADC_CR1_SCAN;	    // 1: Scan mode enable 
	ADC1->CR2 &= ~ADC_CR2_ALIGN;	  // 0: Right alignment	
	
	// Configure the sequence length
	ADC1->SQR1 &= ~(15UL << 20);    // 0000: 1 conversion in the regular channel conversion sequence
	
	// Configure the channel sequence 
	ADC1->SQR3 &= ~(0x1FUL << 0);			// Choose CH of 1st Conversion: clear bits
	ADC1->SQR3 |=  (1UL << 0);	      // Choose CH of 1st Conversion: Select CH (0~15)	
	
// 5. Interrupt Enable
	// Enable EOC(conversion) interrupt. 
	ADC1->CR1 &= ~(1UL << 5);    			// Interrupt reset
	ADC1->CR1 |=  (1UL << 5);    			// Interrupt enable
	
	// Enable ADC_IRQn 
	NVIC_SetPriority(ADC_IRQn, 2);		// Set Priority to 2
	NVIC_EnableIRQ(ADC_IRQn);     		// Enable interrupt form ACD1 peripheral	

// 6. Enable ADC
	ADC1->CR2 |= ADC_CR2_ADON;

// ADC SW configuration	---------------------------------------------------------------------	    
  ADC1->CR2 |= ADC_CR2_SWSTART;
	
  /* Infinite Loop    -----------------------------------------------------------------------*/
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
	GPIO_init(GPIOA, 1, ANALOG);    // GPIOA pin1 enable
	GPIO_pudr(GPIOA, 1, EC_NONE);
	SysTick_init(1);
}

void ADC_IRQHandler(void){
	if(is_ADC_EOC(ADC1)){
		result_v = ADC_read(ADC1);
		print_flag = 1;
 }
}
