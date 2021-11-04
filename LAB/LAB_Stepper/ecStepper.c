#include "stm32f4xx.h"
#include "ecStepper.h"
#include "ecSysTick.h"

//State number 
#define S0 0
#define S1 1
#define S2 2
#define S3 3
#define S4 4
#define S5 5
#define S6 6
#define S7 7


// Stepper Motor function
uint32_t direction = 1; 
uint32_t step_delay = 100; 
uint32_t ustep_delay = 100000; 
uint32_t step_per_rev = 64;
uint32_t gear_ratio = 32;

	 

// Stepper Motor variable
volatile Stepper_t myStepper; 
volatile uint32_t step_mode;


//FULL stepping sequence  - FSM
typedef struct {
	uint8_t out;
  uint32_t next[2];
} State_full_t;

State_full_t FSM_full[4] = {  
 {0xC,{S1,S3}},
 {0x6,{S2,S0}},
 {0x3,{S3,S1}},
 {0x9,{S0,S2}}
};

//HALF stepping sequence
typedef struct {
	uint8_t out;
  uint32_t next[8];
} State_half_t;

State_half_t FSM_half[8] = { 
 {0x08,{S1,S7}},
 {0x0C,{S2,S0}},
 {0x04,{S3,S1}},
 {0x06,{S4,S2}},
 {0x02,{S5,S3}},
 {0x03,{S6,S4}},
 {0x01,{S7,S5}},
 {0x09,{S0,S6}}
};



void Stepper_init(GPIO_TypeDef* port1, int pin1, GPIO_TypeDef* port2, int pin2, GPIO_TypeDef* port3, int pin3, GPIO_TypeDef* port4, int pin4){
	 
//  GPIO Digital Out Initiation
	 myStepper.port1 = port1;
   myStepper.pin1  = pin1;
	
	 myStepper.port2 = port2;
   myStepper.pin2  = pin2;
	
	 myStepper.port3 = port3;
   myStepper.pin3  = pin3;
	
	 myStepper.port4 = port4;
   myStepper.pin4  = pin4;
	
	
	
//  GPIO Digital Out Initiation
		// No pull-up Pull-down , Push-Pull, Fast	
		// Port1,Pin1 ~ Port4,Pin4
	  GPIO_init(port1, pin1, OUTPUT);
		GPIO_init(port2, pin2, OUTPUT);
		GPIO_init(port3, pin3, OUTPUT);
		GPIO_init(port4, pin4, OUTPUT);
		
		GPIO_pudr(port1, pin1, NOPUPD);
		GPIO_pudr(port2, pin2, NOPUPD);
		GPIO_pudr(port3, pin3, NOPUPD);
		GPIO_pudr(port4, pin4, NOPUPD);
		
		GPIO_otype(port1, pin1, PushPull);
		GPIO_otype(port2, pin2, PushPull);
		GPIO_otype(port3, pin3, PushPull);
		GPIO_otype(port4, pin4, PushPull);
	
		
		GPIO_ospeed(port1, pin1, FastSpeed);
		GPIO_ospeed(port2, pin2, FastSpeed);
		GPIO_ospeed(port3, pin3, FastSpeed);
		GPIO_ospeed(port4, pin4, FastSpeed);

	
}

void Stepper_pinOut (uint32_t state, int mode){
	
	   if (mode ==FULL){         // FULL mode
			GPIO_write(myStepper.port1, myStepper.pin1, ((FSM_full[state].out & 8UL) >> 3));
  		GPIO_write(myStepper.port2, myStepper.pin2, ((FSM_full[state].out & 4UL) >> 2));
			GPIO_write(myStepper.port3, myStepper.pin3, ((FSM_full[state].out & 2UL) >> 1));
			GPIO_write(myStepper.port4, myStepper.pin4, ((FSM_full[state].out & 1UL) >> 0));
			
			}	 
		 else if (mode ==HALF){    // HALF mode
			GPIO_write(myStepper.port1, myStepper.pin1, ((FSM_half[state].out & 8UL) >> 3));
  		GPIO_write(myStepper.port2, myStepper.pin2, ((FSM_half[state].out & 4UL) >> 2));
			GPIO_write(myStepper.port3, myStepper.pin3, ((FSM_half[state].out & 2UL) >> 1));
			GPIO_write(myStepper.port4, myStepper.pin4, ((FSM_half[state].out & 1UL) >> 0));
			}
}


void Stepper_setSpeed (long whatSpeed, int mode){      // rppm
	
		int step_mode = 0;
		if(mode == FULL) step_mode = 1;
		else if(mode == HALF) step_mode = 2;
	
		step_delay = (uint32_t)(60000)/(step_per_rev * gear_ratio * step_mode * whatSpeed); // Convert rpm to milli sec
		ustep_delay = (uint32_t)(60000000)/(step_per_rev * gear_ratio * step_mode * whatSpeed); // Convert rpm to micro sec
}


void Stepper_step(int steps, int direction, int mode){

	 myStepper._step_num = steps;
	 uint32_t state_number = 0;
	
	 for(;myStepper._step_num>0;myStepper._step_num--){ // run for step size
				//delay_ms(step_delay);                         // delay (step_delay); 
				delay_us(ustep_delay);                        // delay (ustep_delay);
		 
				if (mode == FULL) state_number = FSM_full[state_number].next[!direction];
				else if (mode == HALF) state_number = FSM_half[state_number].next[!direction]; 
				
				Stepper_pinOut(state_number, mode);
   }
}


void Stepper_stop (void){ 
     
    	myStepper._step_num = 0;    
			// All pins(Port1~4, Pin1~4) set as DigitalOut '0'
			GPIO_write(myStepper.port1, myStepper.pin1, 0);
  		GPIO_write(myStepper.port2, myStepper.pin2, 0);
			GPIO_write(myStepper.port3, myStepper.pin3, 0);
			GPIO_write(myStepper.port4, myStepper.pin4, 0);
}
