---
description: EC_HAL API
---

# Embedded Controller HAL

Written by:  Daegi Kim



Program: 		C/C++

IDE/Compiler: Keil uVision 5

OS: 					WIn10

MCU:  				STM32F411RE, Nucleo-64





## GPIO Digital In/Out 

### Header File

 `#include "ecGPIO.h"`



```c++
#include "stm32f411xe.h"

#ifndef __EC_GPIO_H
#define __EC_GPIO_H

#define INPUT  0x00
#define OUTPUT 0x01
#define AF     0x02
#define ANALOG 0x03

#define HIGH 1
#define LOW  0

#define LED_PIN 	5
#define BUTTON_PIN 13


void GPIO_init(GPIO_TypeDef *Port, int pin, int mode);
void GPIO_write(GPIO_TypeDef *Port, int pin, int Output);
int  GPIO_read(GPIO_TypeDef *Port, int pin);
void GPIO_mode(GPIO_TypeDef* Port, int pin, int mode);
void GPIO_ospeed(GPIO_TypeDef* Port, int pin, int speed);
void GPIO_otype(GPIO_TypeDef* Port, int pin, int type);
void GPIO_pupdr(GPIO_TypeDef* Port, int pin, int pudr);

#endif

```




### GPIO_init\(\)

Initializes GPIO pins with default setting and Enables GPIO Clock. Mode: In/Out/AF/Analog

```c++
void GPIO_init(GPIO_TypeDef *Port, int pin, int mode);
```

**Parameters**

* **Port:**  Port Number,  GPIOA~GPIOH

* **pin**:  pin number (int) 0~15

* **mode**:   INPUT (0), OUTPUT (1),  AF(02), ANALOG (03)

  

**Example code**

```c++
GPIO_init(GPIOA, 5, OUTPUT);
GPIO_init(GPIOC, 13, INPUT); //GPIO_init(GPIOC, 13, 0);
```



### GPIO_mode\(\)

Configures  GPIO pin modes: In/Out/AF/Analog

```c++
void GPIO_init(GPIO_TypeDef *Port, int pin, int mode);
```

**Parameters**

* **Port:**  Port Number,  GPIOA~GPIOH

* **pin**:  pin number (int) 0~15

* **mode**:   INPUT (0), OUTPUT (1),  AF(02), ANALOG (03)

  

**Example code**

```c++
GPIO_mode(GPIOA, 5, OUTPUT);
```



------



## GPIO_pupdr()

### Function Name

```c++
void GPIO_pupdr(GPIO_TypeDef *Port, int pin, int pupd);
```

**Parameters**

* Port: 
* pin
* pupd



**Example code**

```text

```



`header.h`



The header file is `ecGPIO.h`



```c
void main(void)
{
	
}
```





