/*
 * DispenserUART.h
 *
 *  Created on: 1 Dec 2016
 *      Author: felix
 */

#ifndef UART_DISPENSERUART_H_
#define UART_DISPENSERUART_H_

#include "uart.h"
#include "../Time/DispenserTime.h"

//prototypes:
void uartInit(uint32_t baudrate);
uint16_t uartAvailable();
uint8_t parseCommands();
void printTIME(DispenserTime* time);
void printPillTimes();

void printuint16_t(uint16_t print);
void printint16_t(int16_t print);

#endif /* UART_DISPENSERUART_H_ */
