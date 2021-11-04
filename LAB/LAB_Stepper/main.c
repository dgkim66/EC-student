/*
******************************************************************************
* @author  SSSLAB
* @Mod		 2021-11-04 by Daegi Kim  	
* @brief   Embedded Controller:  LAB_Stepper Motor
*
* 
******************************************************************************
*/
#include "stm32f411xe.h"
#include "ecGPIO.h"
#include "ecRCC.h"
#include "ecTIM.h"
#include "ecEXTI.h"
#include "ecSysTick.h"
#include "ecStepper.h"

void setup(void);

void EXTI15_10_IRQHandler(void);
	
int main(void) { 
	// Initialiization --------------------------------------------------------
	setup();
	
	Stepper_step(20480, 0, FULL);  // (Step : 2048 step per rev(Full step), Direction : 0 or 1, Mode : FULL or HALF)
	
	// Inifinite Loop ----------------------------------------------------------
	while(1) {;}
}

// Initialiization 
void setup(void)
{	
	
	RCC_PLL_init();                                 // System Clock = 84MHz
	SysTick_init(1);                                // Systick init
	
	EXTI_init(GPIOC,BUTTON_PIN,FALL,0);             // External Interrupt Setting
	GPIO_init(GPIOC, BUTTON_PIN, INPUT);            // GPIOC pin13 initialization

	Stepper_init(GPIOB,10,GPIOB,4,GPIOB,5,GPIOB,3); // Stepper GPIO pin initialization
	Stepper_setSpeed(10, FULL);                          // set stepper motor speed
}

void EXTI15_10_IRQHandler(void) {  
	if (is_pending_EXTI(BUTTON_PIN)) {
		Stepper_stop();
		clear_pending_EXTI(BUTTON_PIN); // cleared by writing '1'
	}
}
