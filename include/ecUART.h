#ifndef __EC_UART_H
#define __EC_UART_H

#include "stm32f411xe.h"
#include <stdio.h>


void UART2_init();
void USART_write(USART_TypeDef * USARTx, uint8_t *buffer, uint32_t nBytes);
void USART_delay(uint32_t us);

void USART_IRQHandler(USART_TypeDef * USARTx, uint8_t *buffer, uint32_t * pRx_counter);

#endif