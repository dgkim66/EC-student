# Embedded Controller HAL

Written by:  Daegi Kim



Program: 		C/C++

IDE/Compiler: Keil uVision 5

OS: 				   Win10

MCU:  			  STM32F411RE, Nucleo-64





## GPIO Digital In/Out 

### Header File

 `#include "ecGPIO.h"`



```c++
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
 
#ifdef __cplusplus
}
#endif /* __cplusplus */

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

* **mode**:   INPUT (00), OUTPUT (01),  AF(02), ANALOG (03)

  

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

* **mode**:   INPUT (00), OUTPUT (01),  AF(02), ANALOG (03)

  

**Example code**

```c++
GPIO_mode(GPIOA, 5, OUTPUT);
```





### GPIO_write()

Configures GPIO pin Output: Low / High

```c++
void GPIO_write(GPIO_TypeDef *Port, int pin, int Output);
```

**Parameters**

* **Port:**  Port Number,  GPIOA~GPIOH
* **pin**:  pin number (int) 0~15
* **Output**:   LOW (0), HIGH (1)



**Example code**

```text
GPIO_write(GPIOA, LED_PIN, HIGH);
```





### GPIO_read()

Return the statement of GPIO pin : 0 / 1

```c++
int  GPIO_read(GPIO_TypeDef *Port, int pin);
```

**Parameters**

* **Port:**  Port Number,  GPIOA~GPIOH
* **pin**:  pin number (int) 0~15



**Example code**

```text
GPIO_read(GPIOC, BUTTON_PIN);
```





### GPIO_ospeed()

Configures GPIO pin output speed: Low / Medium / Fast / High

```c++
void GPIO_ospeed(GPIO_TypeDef* Port, int pin, int speed);
```

**Parameters**

* **Port:**  Port Number,  GPIOA~GPIOH
* **pin**:  pin number (int) 0~15
* **speed**:   LowSpeed (00), MediumSpeed (01), FastSpeed (02), HighSpeed (03)



**Example code**

```text
GPIO_ospeed(GPIOA, LED_PIN, MediumSpeed);
```





### GPIO_otype()

Configures GPIO pin output type: PushPull / OpenDrain

```c++
void GPIO_otype(GPIO_TypeDef* Port, int pin, int type);
```

**Parameters**

* **Port:**  Port Number,  GPIOA~GPIOH
* **pin**:  pin number (int) 0~15
* **type**:  PushPull (00), OpenDrain (01)



**Example code**

```text
GPIO_otype(GPIOA, LED_PIN, PushPull);
```





### GPIO_pudr()

Configures GPIO pin Pull up / Pull down Mode: NoPUPD / Pull Up / Pull Down / Reserved

```c++
void GPIO_pudr(GPIO_TypeDef* Port, int pin, int pudr);
```

**Parameters**

* **Port:**  Port Number,  GPIOA~GPIOH
* **pin**:  pin number (int) 0~15
* **pudr**:  NOPUPD (00), PullUp (01), PullDown (02), Reserved (03)



**Example code**

```text
GPIO_pudr(GPIOA, LED_PIN, PullUp);
```





### sevensegment_init()

Initializes pins with default setting(Output) and Enables GPIO Clock.

Pin : PA5, PA6, PA7, PB6, PC7, PA9, PA8, PB10

```c++
void sevensegment_init(void);
```



**Example code**

```text
sevensegment_init();
```







### sevensegment_decode();

Configures Seven Segment pins Output: Low / High

```c++
void sevensegment_decode(uint8_t num);
```

**Parameters**

* **num:** Number that want to display (smaller than 10)



**Example code**

```text
void sevensegment_decode(cnt % 10);
```









### EXTI_init();

Configures External Interrupt (Clock, Connecting GPIO pin, Trigger type, Priority, Enable)

```c++
void EXTI_init(GPIO_TypeDef *port, int pin, int trig_type, int priority);
```

**Parameters**

* **port:** GPIO port
* **pin:** Pin number
* **trig_type:** Trigger type (RISE = 0, FALL = 1)
* **priority:** Interrupt priority



**Example code**

```text
EXTI_init(GPIOC, BUTTON_PIN, FALL, 0);
```









### EXTI_enable();

Set EXTI IMR to enable External Interrupt

```c++
void EXTI_enable(uint32_t pin);
```

**Parameters**

* **pin:** Pin number



**Example code**

```text
EXTI_enable(BUTTON_PIN);
```







### EXTI_disable();

Clear EXTI IMR to disable External Interrupt

```c++
void EXTI_disable(uint32_t pin);
```

**Parameters**

* **pin:** Pin number



**Example code**

```text
EXTI_disable(BUTTON_PIN);
```







### is_pending_EXTI();

Return the checked value of interrupt pending

```c++
uint32_t is_pending_EXTI(uint32_t pin);
```

**Parameters**

* **pin:** Pin number



**Example code**

```text
if(is_pending_EXTI(BUTTONPIN)){}
```







### clear_pending_EXTI();

Clear the pending flag by writing '1'

```c++
void clear_pending_EXTI(uint32_t pin);
```

**Parameters**

* **pin:** Pin number



**Example code**

```text
clear_pending_EXTI(BUTTON_PIN);
```









### EXTIx_IRQHandler();

Run the function when external interrupting

```c++
void EXTIx_IRQHandler(int pin);
```

**Parameters**

* **pin:** Pin number



**Example code**

```text
EXTIx_IRQHandler(BUTTON_PIN);
```









### SysTick_init();

Set the SysTick (Load, Initial Value, Control Value, Enable)

```c++
void SysTick_init(uint32_t msec);
```

**Parameters**

* **msec:** interrupting period (ms)



**Example code**

```text
SysTick_init(1);
```









### delay_ms();

Delay the time (ms)

```c++
void delay_ms(uint32_t msec);
```

**Parameters**

* **msec:** Delaying time (ms)



**Example code**

```text
delay_ms(1000);
```









### SysTick_val();

Return the time value of the (function end - function start)

```c++
uint32_t SysTick_val(void);
```



**Example code**

```text
SysTick_val(void);
```







### SysTick_enable();

Set the SysTick Control to make SysTick enable

```c++
void SysTick_enable(void);
```



**Example code**

```text
SysTick_enable();
```







### SysTick_disable();

Clear the SysTick Control to make SysTick disable

```c++
void SysTick_disable(void);
```



**Example code**

```text
SysTick_disable();
```







### SysTick_reset();

Reset the SysTick Value

```c++
void SysTick_reset(void);
```



**Example code**

```text
SysTick_reset();
```







### SysTick_Handler();

Function operate when SysTick is interrupting

```c++
void SysTick_Handler(void);
```



**Example code**

```text
SysTick_Handler();
```





