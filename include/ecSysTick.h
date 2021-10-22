// Distributed for LAB: GPIO

#include "stm32f411xe.h"
#include "ecGPIO.h"

#define MCU_CLK_PLL 84000000


void SysTick_init(uint32_t msec);
void delay_ms(uint32_t msec);
uint32_t SysTick_val(void);
void SysTick_enable(void);
void SysTick_disable(void);
void SysTick_reset(void);

void SysTick_Handler(void);
