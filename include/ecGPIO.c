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


void sevensegment_init(void)
{
	GPIO_init(GPIOA, 5, OUTPUT);           // calls RCC_GPIOA_enable(), and make LED_a as output
	GPIO_init(GPIOA, 6, OUTPUT);           // calls RCC_GPIOA_enable(), and make LED_b as output
	GPIO_init(GPIOA, 7, OUTPUT);           // calls RCC_GPIOA_enable(), and make LED_c as output
	GPIO_init(GPIOB, 6, OUTPUT);           // calls RCC_GPIOA_enable(), and make LED_d as output
	GPIO_init(GPIOC, 7, OUTPUT);           // calls RCC_GPIOA_enable(), and make LED_e as output
	GPIO_init(GPIOA, 9, OUTPUT);           // calls RCC_GPIOA_enable(), and make LED_f as output
	GPIO_init(GPIOA, 8, OUTPUT);           // calls RCC_GPIOA_enable(), and make LED_g as output
	GPIO_init(GPIOB, 10, OUTPUT);          // calls RCC_GPIOA_enable(), and make LED_dp as output

	GPIO_otype(GPIOA, 5, PushPull);        // Make otype of LED_a as Push Pull
	GPIO_otype(GPIOA, 6, PushPull);        // Make otype of LED_b as Push Pull
	GPIO_otype(GPIOA, 7, PushPull);        // Make otype of LED_c as Push Pull
	GPIO_otype(GPIOB, 6, PushPull);        // Make otype of LED_d as Push Pull
	GPIO_otype(GPIOC, 7, PushPull);        // Make otype of LED_e as Push Pull
	GPIO_otype(GPIOA, 9, PushPull);        // Make otype of LED_f as Push Pull
	GPIO_otype(GPIOA, 8, PushPull);        // Make otype of LED_g as Push Pull
	GPIO_otype(GPIOB, 10, PushPull);       // Make otype of LED_dp as Push Pull
	
	GPIO_pudr(GPIOA, 5, NOPUPD);           // Make LED_a Mode as Pull Up
	GPIO_pudr(GPIOA, 6, NOPUPD);           // Make LED_b Mode as Pull Up
	GPIO_pudr(GPIOA, 7, NOPUPD);           // Make LED_c Mode as Pull Up
	GPIO_pudr(GPIOB, 6, NOPUPD);           // Make LED_d Mode as Pull Up
	GPIO_pudr(GPIOC, 7, NOPUPD);           // Make LED_e Mode as Pull Up
	GPIO_pudr(GPIOA, 9, NOPUPD);           // Make LED_f Mode as Pull Up
	GPIO_pudr(GPIOA, 8, NOPUPD);           // Make LED_g Mode as Pull Up
	GPIO_pudr(GPIOB, 10, NOPUPD);          // Make LED_dp Mode as Pull Up
	
	GPIO_ospeed(GPIOA, 5, FastSpeed);      // Set LED_a pin speed as Medium Speed
	GPIO_ospeed(GPIOA, 6, FastSpeed);      // Set LED_b pin speed as Medium Speed
	GPIO_ospeed(GPIOA, 7, FastSpeed);      // Set LED_c pin speed as Medium Speed
	GPIO_ospeed(GPIOB, 6, FastSpeed);      // Set LED_d pin speed as Medium Speed
	GPIO_ospeed(GPIOC, 7, FastSpeed);      // Set LED_e pin speed as Medium Speed
	GPIO_ospeed(GPIOA, 9, FastSpeed);      // Set LED_f pin speed as Medium Speed
	GPIO_ospeed(GPIOA, 8, FastSpeed);      // Set LED_g pin speed as Medium Speed
	GPIO_ospeed(GPIOB, 10, FastSpeed);     // Set LED_dp pin speed as Medium Speed
}


void sevensegment_decode(uint8_t num)
{
		int number[10][8]={
			{0,0,0,0,0,0,1,1},                 // Zero
			{1,0,0,1,1,1,1,1},                 // One
			{0,0,1,0,0,1,0,1},                 // Two
			{0,0,0,0,1,1,0,1},                 // Three
			{1,0,0,1,1,0,0,1},                 // Four
			{0,1,0,0,1,0,0,1},                 // Five
			{0,1,0,0,0,0,0,1},                 // Six
			{0,0,0,1,1,0,1,1},                 // Seven
			{0,0,0,0,0,0,0,1},                 // Eight
			{0,0,0,0,1,0,0,1}                  // Nine
			};
		
		GPIO_write(GPIOA, 5, number[num][0]);                  // Set output depending on the num
		GPIO_write(GPIOA, 6, number[num][1]); 
		GPIO_write(GPIOA, 7, number[num][2]); 
		GPIO_write(GPIOB, 6, number[num][3]); 
		GPIO_write(GPIOC, 7, number[num][4]); 
		GPIO_write(GPIOA, 9, number[num][5]); 
		GPIO_write(GPIOA, 8, number[num][6]); 
		GPIO_write(GPIOB, 10, number[num][7]);
		
}
