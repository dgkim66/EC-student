// Distributed for LAB: GPIO

#include "stm32f411xe.h"
#include "ecRCC.h"

#ifndef __ECGPIO_H
#define __ECGPIO_H

#define INPUT  0x00
#define OUTPUT 0x01
#define AF     0x02
#define ANALOG 0x03

#define NOPUPD   0x00
#define PullUp   0x01
#define PullDown 0x02
#define Reserved 0x03

#define LowSpeed    0x00
#define MediumSpeed 0x01
#define FastSpeed   0x02
#define HighSpeed   0x03

#define PushPull  0x00
#define OpenDrain 0x01

#define HIGH 1
#define LOW  0

#define LED_PIN 	5
#define BUTTON_PIN 13

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */
	 
void GPIO_init(GPIO_TypeDef *Port, int pin, int mode);
void GPIO_mode(GPIO_TypeDef* Port, int pin, int mode);

void GPIO_write(GPIO_TypeDef *Port, int pin, int Output);
int  GPIO_read(GPIO_TypeDef *Port, int pin);
void GPIO_ospeed(GPIO_TypeDef* Port, int pin, int speed);
void GPIO_otype(GPIO_TypeDef* Port, int pin, int type);
void GPIO_pudr(GPIO_TypeDef* Port, int pin, int pudr);

void sevensegment_init(void);
void sevensegment_decode(uint8_t	num);
	
 
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
