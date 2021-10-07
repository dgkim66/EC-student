// Distributed for LAB: GPIO

#include "stm32f4xx.h"
#include "stm32f411xe.h"
#include "ecGPIO.h"

void GPIO_init(GPIO_TypeDef *Port, int pin, int mode){     
   
	if (Port == GPIOA)
		RCC_GPIOA_enable();
	if (Port == GPIOB)
		RCC_GPIOB_enable();
	if (Port == GPIOC)
		RCC_GPIOC_enable();
	if (Port == GPIOD)
		RCC_GPIOD_enable();
	// Make it for GPIOB, GPIOD..GPIOH

	// You can make a more general function of
	// void RCC_GPIO_enable(GPIO_TypeDef *Port); 

	GPIO_mode(Port, pin, mode);
	// The rest are default values
}


// GPIO Mode          : Input(00), Output(01), AlterFunc(10), Analog(11, reset)
void GPIO_mode(GPIO_TypeDef *Port, int pin, int mode){
  Port->MODER &= ~(3UL<<(2*pin));     
  Port->MODER |= mode<<(2*pin);    
}


void GPIO_write(GPIO_TypeDef *Port, int pin, int Output){
	Port->ODR &= ~(1UL << pin);
	Port->ODR |= Output<<pin;
}


int  GPIO_read(GPIO_TypeDef *Port, int pin){
	return ((Port->IDR) & (1UL << pin));
}


void GPIO_ospeed(GPIO_TypeDef* Port, int pin, int speed){
	Port->OSPEEDR &= ~(3UL << (2*pin));
	Port->OSPEEDR |= speed << (2*pin);
}


void GPIO_otype(GPIO_TypeDef* Port, int pin, int type){
	Port->OTYPER &= ~(1UL<<pin);
	Port->OTYPER |= type<<pin;
}


void GPIO_pudr(GPIO_TypeDef* Port, int pin, int pudr){
	Port->PUPDR &= ~(3UL<<(2*pin));
	Port->PUPDR |= pudr<<(2*pin);
}
