// Distributed for LAB: EXTI

#include "stm32f4xx.h"
#include "stm32f411xe.h"
#include "ecEXTI.h"



uint32_t portNumber(GPIO_TypeDef *port){
	
	unsigned int portNum=0;
	if(port == GPIOA) portNum = 0UL;
	else if(port == GPIOB) portNum = 1UL;
	else if(port == GPIOC) portNum = 2UL;
	else if(port == GPIOD) portNum = 3UL;
	else if(port == GPIOE) portNum = 4UL;
	else if(port == GPIOH) portNum = 7UL;
	
	return portNum;
}


void EXTI_init(GPIO_TypeDef *port, int pin, int trig_type, int priority){
	
	
	// EXTI Initialiization ------------------------------------------------------	
	
	uint32_t numb = portNumber(port);
	
	
	// SYSCFG peripheral clock enable
  RCC->APB2ENR |= 1<<14;

	// Connect External Line to the GPIO
	
	
	// Button: PC_13 -> EXTICR3(EXTI13)
	SYSCFG->EXTICR[pin/4] &= ~(0xFUL<<((pin/4)*4)); // Clear
	SYSCFG->EXTICR[pin/4] |= (numb<<((pin-12)*4));     // 

	// Falling trigger enable (Button: pull-up)
	EXTI->RTSR &= ~(1 << pin);
	EXTI->FTSR &= ~(1 << pin);
	EXTI->RTSR |= ~trig_type << pin;
	EXTI->FTSR |= trig_type << pin;
	
	EXTI_enable(pin);

	// Interrupt IRQn, Priority
	if(pin >= 5 && pin <= 9){
		NVIC_EnableIRQ(EXTI9_5_IRQn); 			// Enable EXTI
		NVIC_SetPriority(EXTI9_5_IRQn, priority);  		// Set EXTI priority
	}
	else if(pin >= 10 && pin <= 15){
		NVIC_EnableIRQ(EXTI15_10_IRQn); 			// Enable EXTI
		NVIC_SetPriority(EXTI15_10_IRQn, priority);  		// Set EXTI priority
	}
	 
	
	
}

void EXTI_enable(uint32_t pin){   // mask in IMR
	
	// Unmask (Enable) EXT interrupt
	EXTI->IMR |= 1UL << pin;
	
}

void EXTI_disable(uint32_t pin){   // unmask in IMR
	
	EXTI->IMR &= ~(1UL << pin);
	
}

uint32_t is_pending_EXTI(uint32_t pin){
	
	return ((EXTI->PR & (1UL<<pin)) == 1UL<<pin);
	
}

void clear_pending_EXTI(uint32_t pin){
	
	EXTI->PR |= 1UL<<pin; // cleared by writing '1'
	
}