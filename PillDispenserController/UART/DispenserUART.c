/*
 * DispenserUART.c
 *
 *  Created on: 1 Dec 2016
 *      Author: felix
 */

#include "DispenserUART.h"
#include "../Time/DispenserTime.h"
#include <util/delay.h>

uint8_t parseATPILL(char comp);
uint8_t parseATTIME(char comp);
uint8_t parseTIME(char comp, DispenserTime* time);
const char* AT_TIME = "AT+CURRENTTIME:";
const char* AT_PILL = "AT+PILL:";

void uartInit(uint32_t baudrate){
	//uart0_init(UART_BAUD_SELECT_DOUBLE_SPEED(baudrate, 16000000L));
	DDRD |= (1<<PD1);	//set the Tx Pin to be used as an output
	uart0_init(((((F_CPU/8)/baudrate)-1)|0x8000));
}

uint16_t uartAvailable(){
	return uart0_available();
}
uint8_t parseCommands(){
	while(uart0_available()){
		char comp = (char)uart0_getc();
		//uart0_putc(comp);
		if(parseATTIME(comp)){
			return 1;	//1 is AT+CURRENTTIME:
		}
		if(parseATPILL(comp)){
			return 2; 	//2 is AT+PILL:
		}
	}
	return 0;
}

uint8_t parseATPILL(char comp){
	static uint8_t index =0;
	static uint8_t commandFlag =0;
	static uint8_t parsed=0; //the amount of PillEvents that have been parsed already
	uint8_t result;
	if(commandFlag){
		//AT+PILL:
		//loop this, while ! END is received
		if(comp == 'E'){	//indicates the End
			commandFlag=0;
			parsed=0;
			return 1;
		}
		else{
			result = parseTIME(comp, &setTimes[parsed]);
			if(result==0){
				commandFlag=0;
				parsed =0;
				return 0;
			}
			else if(result == 1) return 0;
			else if(result == 2){
				parsed++;
				setTimesCount = parsed;
				return 0;
			}
		}
	}
	if(index<8){ //8 is the length of AT+PILL:
		if(comp == AT_PILL[index]){
			index++;
			if(index==8){
				index =0;
				commandFlag=1;
				return 0;
			}
		}
		else{
			index =0;
			return 0;
		}
	}
	return 0;
}
uint8_t parseATTIME(char comp){
	static uint8_t index =0;
	static uint8_t commandFlag =0;
	uint8_t result;
	if(commandFlag){
	//AT+CURRENTTIME:
		result = parseTIME(comp, &currentTime);
		if(result==0){
			commandFlag=0;
			return 0;
		}
		else if(result == 1) return 0;
		else if(result == 2){
			commandFlag=0;
			return 1;
		}
	}

	if(index<15){ //15 is the length of AT+CURRENTTIME:
		if(comp == AT_TIME[index]){
			index++;
			if(index==15){
				index =0;
				commandFlag=1;
				return 0;
			}
		}
		else{
			index =0;
			return 0;
		}
	}

	return 0;
}

uint8_t parseTIME(char comp, DispenserTime* time){
	static uint8_t position=0;
	uint8_t day;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
	switch (position){
	case 0:
		day = comp-'0';
		if(day>7 || day<0){
			return 0;
		}
		else{
			time->day = day;
			position++;
		}
		break;
	case 1:
		if(comp!=':'){
			position=0;
			return 0;
		}
		position++;
		break;
	case 2:
		hour = (comp-'0')*10;
		if(hour>=30 || hour<0){
			position=0;
			return 0;
		}
		else{
			time->hour = hour;
			position++;
		}
		break;
	case 3:
		hour = (comp-'0');
		if(hour>9 || hour<0){
			position=0;
			return 0;
		}
		else{
			time->hour += hour;
			position++;
		}
		break;
	case 4:
		if(comp!=':'){
			position=0;
			return 0;
		}
		position++;
		break;
	case 5:
		minute = (comp-'0')*10;
		if(minute>50 || minute<0){
			position=0;
			return 0;
		}
		else{
			time->minute = minute;
			position++;
		}
		break;
	case 6:
		 minute = (comp-'0');
		if(minute>9 || minute<0){
			position=0;
			return 0;
		}
		else{
			time->minute += minute;
			position++;
		}
		break;
	case 7:
		if(comp!=':'){
			position=0;
			return 0;
		}
		position++;
		break;

	case 8:
		second = (comp-'0')*10;
		if(second>60 || second<0){
			position=0;
			return 0;
		}
		else{
			time->second = second;
			position++;
		}
		break;
	case 9:
		second = (comp-'0');
		if(second>9 || second<0){
			position=0;
			return 0;
		}
		else{
			time->second += second;
			position++;
			//position=0;
			//return 2;
		}
		break;
	case 10:
		if(comp!=':'){
			position=0;
			return 0;
		}
		position=0;
		return 2;

	}


	return 1;
}


void printTIME(DispenserTime* time){
	uart0_putc(time->day + '0');
	uart0_putc(':');
	uart0_putc(time->hour/10 + '0');
	uart0_putc(time->hour%10 + '0');
	uart0_putc(':');
	uart0_putc(time->minute/10 + '0');
	uart0_putc(time->minute%10 + '0');
	uart0_putc(':');
	uart0_putc(time->second/10 + '0');
	uart0_putc(time->second%10 + '0');
}

void printPillTimes(){
	for(uint8_t i =0; i<setTimesCount; i++){
		printTIME(&setTimes[i]);
		uart0_putc('\n');
	}
}


char string[7];
void printuint16_t(uint16_t print){
	uinttostr(print, string);
	uart0_puts(string);
}
void printint16_t(int16_t print){
	inttostr(print, string);
	uart0_puts(string);
}
void uinttostr (uint16_t val, char * result)
{
	uint16_t remainder;

	int i;

	for (i = 5; i >= 0; i--)
	{
		remainder = val % 10 ;
		result[i] = remainder + '0';   // this gives you the ASCII code for the digit !
		val = val / 10;
	}
	result [6] = '\n';   // string end marker
}

void inttostr (int16_t val, char * result)
{

	int i;
	if(val <0){
		result[0] ='-';
		val *= -1;
	}
	else result[0] = '+';

	for (i = 5; i >= 1; i--)
	{
		result[i] = val % 10 + '0';   // this gives you the ASCII code for the digit !
		val = val / 10;
	}
	result [6] = '\n';   // string end marker
}


/*
uint8_t parseTIME(){
	while(!(uint8_t)uart0_available()){

	}
	currentTime.day += uart0_getc()-'0';
	while(!(uint8_t)uart0_available()){

	}
	uart0_getc();
	while(!(uint8_t)uart0_available()){

	}
	currentTime.hour += (uart0_getc()-'0')*10;
	while(!(uint8_t)uart0_available()){

	}
	currentTime.hour += uart0_getc()-'0';
	uart0_getc();
	while(!uart0_available()){

	}
	currentTime.minute += (uart0_getc()-'0')*10;
	while(!uart0_available()){

	}
	currentTime.minute += uart0_getc()-'0';
	uart0_getc();
	while(!uart0_available()){

	}
	currentTime.second += (uart0_getc()-'0')*10;
	while(!uart0_available()){

	}
	currentTime.second += uart0_getc()-'0';

	return 1;
}
*/


/*
 * if((uint8_t)uart0_available()){
		currentTime.day = uart0_getc()-'0';
	}
	else return 0;
	if((uint8_t)uart0_available()){
		uart0_getc();
	}
	else return 0;
	if((uint8_t)uart0_available()){
		currentTime.hour += (uart0_getc()-'0')*10;
	}
	else return 0;
	if((uint8_t)uart0_available()){
		currentTime.hour += uart0_getc()-'0';
	}
	else return 0;
	if((uint8_t)uart0_available()){
		uart0_getc();
	}
	else return 0;
	if((uint8_t)uart0_available()){
		currentTime.minute += (uart0_getc()-'0')*10;
	}
	else return 0;
	if((uint8_t)uart0_available()){
		currentTime.minute += (uart0_getc()-'0')*10;
	}
	else return 0;
 *
 */
