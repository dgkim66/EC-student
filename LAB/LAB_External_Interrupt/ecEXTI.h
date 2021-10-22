// Distributed for LAB: GPIO

#include "stm32f411xe.h"
#include "ecGPIO.h"


void EXTI_init(GPIO_TypeDef *port, int pin, int trig_type, int priority);
void EXTI_enable(uint32_t pin);
void EXTI_disable(uint32_t pin);
uint32_t is_pending_EXTI(uint32_t pin);
void clear_pending_EXTI(uint32_t pin);
uint32_t portNumber(GPIO_TypeDef *port);

