#include "stm32f411xe.h"
#include "ecGPIO.h"
#include "ecSysTick.h"
#include "ecRCC.h"
#include "ecTIM.h"
#include "ecPWM.h"
#include "ecEXTI.h"

#define PWM_PIN    1
#define LED_PIN    5
#define BUTTON_PIN 13
#define FALL		 	 0x1UL
#define RISE			 0x0UL



PWM_t  pwm;

unsigned int count = 0;

void EXTI15_10_IRQHandler(void);

void setup(void)
{
	RCC_PLL_init();
	SysTick_init(1);
	
	GPIO_init(GPIOC, BUTTON_PIN, INPUT);
	GPIO_pudr(GPIOC, BUTTON_PIN, PullUp);
	EXTI_init(GPIOC, BUTTON_PIN, FALL, 0);
	
	GPIO_init(GPIOA, PWM_PIN, AF);
	GPIO_otype(GPIOA, PWM_PIN, PushPull);
	GPIO_pudr(GPIOA, PWM_PIN, NOPUPD);
	GPIO_ospeed(GPIOA, PWM_PIN, FastSpeed);
	
	pwm.pin = PWM_PIN;
	PWM_pinmap(&pwm);
	PWM_init(&pwm, GPIOA, PWM_PIN);
	PWM_period_ms(&pwm, 20);
}

int main(void)
{
	// Initialization --------------------------------------------
	setup();
	
	float dutyVal;
	
	// Inifinite Loop --------------------------------------------
	while(1){
		
		dutyVal = ((0.5 + (0.2 * count))/20);
	
		PWM_duty(&pwm, dutyVal);
		delay_ms(100);
		if(count >= 10){
			delay_ms(1000);
			count = 0;
		}
	}
/*
		switch(count){
			case 0: PWM_duty(&pwm,0.5/20); delay_ms(10); break;
			case 1: PWM_duty(&pwm,0.7/20); delay_ms(10); break;
			case 2: PWM_duty(&pwm,0.9/20); delay_ms(10); break;
			case 3: PWM_duty(&pwm,1.1/20); delay_ms(10); break;
			case 4: PWM_duty(&pwm,1.3/20); delay_ms(10); break;
			case 5: PWM_duty(&pwm,1.5/20); delay_ms(10); break;
			case 6: PWM_duty(&pwm,1.7/20); delay_ms(10); break;
			case 7: PWM_duty(&pwm,1.9/20); delay_ms(10); break;
			case 8: PWM_duty(&pwm,2.1/20); delay_ms(10); break;
			case 9: PWM_duty(&pwm,2.3/20); delay_ms(10); break;
			case 10:PWM_duty(&pwm,2.5/20); delay_ms(1010); count = 0; break;
		}
	}*/
}


void EXTI15_10_IRQHandler(void) {  
	if (is_pending_EXTI(BUTTON_PIN)) {
		count++;
		clear_pending_EXTI(BUTTON_PIN); // cleared by writing '1'
	}
}




