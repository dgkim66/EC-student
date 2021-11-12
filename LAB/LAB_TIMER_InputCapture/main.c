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
#include "ecPWM.h"
#include "ecUART.h"
#include "ecSysTick.h"

uint32_t ovf_cnt = 0;
float distance = 0;
float timeInterval = 0;
float timeSt = 0;
float timeEnd= 0;
//int print_flag = 0;

void setup(void);

void TIM2_IRQHandler(void);

int main(void){
	
	setup();

	
	while(1){
		
		distance = ((timeInterval * 340) / (2 * 10000000) ) * 100; // Ultrasonic speed[m/s] * echo pulse duration[us]
		printf("%f [cm]\r\n", distance);  
    		
		delay_ms(1000);
		
	}
}

void TIM2_IRQHandler(void){
	if(is_UIF(TIM2)){                     // Update interrupt
		ovf_cnt++;      										// overflow count
		//printf("OverFlow = %d \r\n", ovf_cnt); 
		clear_UIF(TIM2);  							    // clear update interrupt flag
	}
	if(is_CCIF(TIM2,3)){ 									// TIM2_Ch3 (IC3) Capture Flag. Rising Edge Detect
		timeSt = TIM2->CCR3;			          // Capture TimeStart from CC3
		//printf("RISING Edge\r\n");
		clear_CCIF(TIM2,3);                 // clear capture/compare interrupt flag 
	}								                      
	else if(is_CCIF(TIM2,4)){ 				  	// TIM2_Ch3 (IC4) Capture Flag. Falling Edge Detect
		timeEnd = TIM2->CCR4;					      // Capture TimeEnd from CC4
    timeInterval = 10 * (timeEnd - timeSt + (ovf_cnt * (TIM2->ARR + 1))); 		// Total time of echo pulse
		ovf_cnt = 0;                        // overflow reset
		//printf("FALLING Edge\r\n");
		clear_CCIF(TIM2,4);								  // clear capture/compare interrupt flag 
	}
}

void setup(){

	RCC_PLL_init(); 
	SysTick_init(1);
	UART2_init();
  
// PWM configuration ---------------------------------------------------------------------	
	PWM_t trig;											// PWM1 for trig
	PWM_init(&trig, GPIOA, 6);			// PWM init as PA_6: Ultrasonic trig pulse
	PWM_period_us(&trig,50000);    	// PWM of 50ms period. Use period_us()
	PWM_pulsewidth_us(&trig,10);   	// PWM pulse width of 10us
	
// Input Capture configuration -----------------------------------------------------------------------	
	IC_t echo;											// Input Capture for echo
	ICAP_init(&echo, GPIOB, 10);    // ICAP init as PB10 as input caputre
 	ICAP_counter_us(&echo, 10);   	// ICAP counter step time as 10us
	ICAP_setup(&echo, 3, RISE_TIM); // TIM2_CH3 as IC3 , rising edge detect
	ICAP_setup(&echo, 4, FALL_TIM); // TIM2_CH3 as IC4 , falling edge detect
	
// Enable TIMx interrupt			-----------------------------------------------------------------------	
	TIM2->CR1 |= TIM_CR1_CEN;
	TIM_INT_enable(TIM2);  					// TIM2 Interrupt Enable

}

/*
#include "stm32f411xe.h"
#include "math.h"
#include "ecGPIO.h"
#include "ecRCC.h"
#include "ecTIM.h"
#include "ecPWM.h"
#include "ecEXTI.h"
#include "ecUART.h"
#include "ecSysTick.h"

uint32_t EXTI_flag = 0;
uint32_t ovf_cnt = 0;

double distance = 0;
double timeInterval = 0;
double timeSt = 0;
double timeEnd= 0;


void setup(void);

int main(void){
   
   setup();

   while(1){       
		 
      //Using EXTI_flag, I prevent printing of garbage value
    //  if(EXTI_flag >= 3){
    //     distance = ( timeInterval*340/(2*1000000) ) * 100; // Ultrasonic speed[m/s] * echo pulse duration[us]
    //     printf("%f [cm]\r\n", distance);
    //     
    //     EXTI_flag = 3;
    //     
    //     delay_ms(500);            
    //  }
      
   }
}

void TIM2_IRQHandler(void){
   if(is_UIF(TIM2)){                          // Update interrupt
      ovf_cnt++;                              // overflow count
      clear_UIF(TIM2);                        // clear update interrupt flag
   }
   if(is_CCIF(TIM2, 3)){                      // TIM2_Ch3 (IC3) Capture Flag. Rising Edge Detect
      EXTI_flag++;
      timeSt = TIM2->CCR3;                    // Capture TimeStart from CC3
			//printf("%f [cm]\r\n", distance);
      clear_CCIF(TIM2, 3);                    // clear capture/compare interrupt flag 
   }                                              
   else if(is_CCIF(TIM2, 4)){                 // TIM2_Ch3 (IC4) Capture Flag. Falling Edge Detect
      EXTI_flag++;
      timeEnd = TIM2->CCR4;                   // Capture TimeEnd from CC4

      timeInterval = ( (timeEnd - timeSt) + ( (TIM2->ARR) + 1 )*ovf_cnt ) * 10;       // Total time of echo pulse
      //printf("%f [cm]\r\n", distance);
      ovf_cnt = 0;                             // overflow reset
      clear_CCIF(TIM2, 4);                     // clear capture/compare interrupt flag 
   }
}

void setup(){

   RCC_PLL_init(); 
   SysTick_init(1);
   UART2_init();
  
// PWM configuration ---------------------------------------------------------------------   
   PWM_t trig;                                 // PWM1 for trig
   PWM_init(&trig, GPIOA, 6);            // PWM init as PA_6: Ultrasonic trig pulse
   PWM_period_us(&trig, 50000);       // PWM of 50ms period. Use period_us()
   PWM_pulsewidth_us(&trig, 10);      // PWM pulse width of 10us
   
// Input Capture configuration -----------------------------------------------------------------------   
   IC_t echo;                                 // Input Capture for echo
   ICAP_init(&echo, GPIOB, 10);    // ICAP init as PB10 as input caputre
    ICAP_counter_us(&echo, 10);      // ICAP counter step time as 10us
   ICAP_setup(&echo, 3, RISE_TIM);// TIM2_CH3 as IC3 , rising edge detect
   ICAP_setup(&echo, 4, FALL_TIM);// TIM2_CH3 as IC4 , falling edge detect

// Enable TIMx interrupt         -----------------------------------------------------------------------   
   TIM2->CR1 |= TIM_CR1_CEN;    //   (1<<0)
   TIM_INT_enable(TIM2);
   
}*/

