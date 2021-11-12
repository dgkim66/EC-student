/*
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
#include "ecADC_student.h"

//IR parameter//
uint32_t result_L =0;
uint32_t result_R =0;
uint32_t gapVal = 1500;
uint32_t flag = 0;

void setup(void);
	
int main(void) { 
	// Initialiization --------------------------------------------------------
	setup();
	
	ADC_start(ADC1);

	
	// Inifinite Loop ----------------------------------------------------------
	while(1){
		printf("\r\nLeft  = %d\r\n",result_L);
		printf("Right = %d\r\n",result_R);
		
		if(result_L > result_R + gapVal){
			printf("\r\nGo Right\r\n");
		}
		if(result_R > result_L + gapVal){
			printf("\r\nGo Left\r\n");
		}
		delay_ms(1000);
	}
}

// Initialiization 
void setup(void)
{	
	RCC_PLL_init();                         // System Clock = 84MHz
	UART2_init();
	SysTick_init(1);
	
	ADC_init(GPIOB, 0, TRGO);
	ADC_init(GPIOB, 1, TRGO);
	
	ADC->CCR |= (3UL << 16);					// 11: PCLK2 divided by 8	(10.5MHz)
	
	unsigned int seq[2] = {8, 9};
	ADC_sequence(2, seq);
}


void ADC_IRQHandler(void){
	if(is_ADC_OVR()){
		clear_ADC_OVR();
	}
	if(is_ADC_EOC()){       //after finishing sequence
		if(flag == 0){
			result_L = ADC_read();	
		}
		if(flag == 1){
			result_R = ADC_read();	
		}
		flag = !flag;
	}
	
}

