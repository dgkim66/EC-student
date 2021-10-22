// Distributed for LAB: SysTick

#include "stm32f4xx.h"
#include "stm32f411xe.h"
#include "ecSysTick.h"

volatile uint32_t TimeDelay;

void SysTick_init(uint32_t msec){
	
	SysTick_disable();
	
	SysTick->LOAD = (msec * 0.001 * MCU_CLK_PLL) - 1;
	
	NVIC_SetPriority(SysTick_IRQn,  0);
	
	SysTick->VAL = 0;
	
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
	
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
	
	SysTick_enable();
	
}

void delay_ms(uint32_t msec){
	
	TimeDelay = msec;
	
	while(TimeDelay != 0);
	
}

uint32_t SysTick_val(void){
	
	uint32_t start_time = SysTick->VAL;
	
	uint32_t stop_time = SysTick->VAL;
	
	uint32_t cycle_count = start_time - stop_time;
	
	return cycle_count;
	
	
}

void SysTick_enable(void){
	
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	
}

void SysTick_disable(void){
	
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
	
}

void SysTick_reset(void){
	
	SysTick->VAL = 0;
	
}


void SysTick_Handler(void){
	
	if(TimeDelay > 0)  TimeDelay--;
	
}

