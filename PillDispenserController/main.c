/*
 * main.c
 *
 *  Created on: 1 Dec 2016
 *      Author: felix
 */


#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "Time/DispenserTime.h"
#include "Time/DispenserTimer.h"
#include "UART/DispenserUART.h"
#include "Steppermotor/DispenserMotor.h"
#include "ADC/DispenserADC.h"
#include "global.h"
//#include "UART/uart.h"
//void uartReceived(char received);


//prototypes:
void checkForEvents();

void eventOccurred();


//variables:
uint8_t minuteCount=0;

int main(){
	//initialize the UART:
	uartInit((uint32_t)115200);
	//initialize the Stepper Driver:
	initStepper();
	//indicator LED:
	DDRB |= (1<<PB5);
	DDRB |= ((1<<POWER_LED) | (1<<NOTIFICATION_LED));
	//initialize the timer for the internal time:
	initTimer();
	//initialize the ADC for detecting the removal of the Pill container and for the stepper motor position
	initADC();
	//restore the dispensing events from the EEPROM:
	restoreTimes();

	//wait for some time, to allow the UserInterface to set everything up and establish a connection:
	_delay_ms(10000);	//delays 10s
	//now Ask for the current Time:
	uart0_puts("AT+TIME");
	//wait until we received the current time:
	uint8_t count=0;

	while(parseCommands()!=1){
		//delay and ask again after parsing a 100 times:
		if(count==100){
			count =0;
			uart0_puts("AT+TIME");
			_delay_ms(4000);
		}
		_delay_ms(1);
		count++;
	}
	//enable the PowerLED when everything is set up
	PORTB |= (1<<POWER_LED);
	sei();

#if DEBUGACTIVE == 1
	uart0_puts("TimesCount:");
	uart0_putc(setTimesCount + (unsigned char)'0');
	uart0_putc('\n');
	printPillTimes();
#endif
	while(1){
		if(uartAvailable()){
			uint8_t result = parseCommands();
			if(result==1){
				//the current Time has been received from the UserInterface Modul
				//it is also already saved to the currentTime var
#if DEBUGACTIVE ==1
				printTIME(&currentTime);
#endif
				//PORTB ^= (1<<PB5);
				//_delay_ms(500);
			}
			else if(result == 2){
				//Dispensing Events have been received
#if DEBUGACTIVE ==1
				printPillTimes();
#endif
				//we should save the PillEvents to the EEPROM:
				saveTimes();
				//PORTB ^= (1<<PB5);
				//_delay_ms(500);
			}
		}
		if(minutePassed){
			//reset minutePassed:
			minutePassed=0;
#if DEBUGACTIVE == 1
			uart0_puts("Time:");
			printTIME(&currentTime);
			uart0_putc('\n');
#endif
			//check if a pill has to be taken:
			checkForEvents();

			minuteCount++;
			if(minuteCount==TIME_TO_WAIT_BEVOR_RETRIEVING_TIME_AGAIN){
				//reset the count, send command to retrieve time again
				minuteCount=0;
				uart0_puts("AT+TIME");	// this is the command for the UserInterface to fetch the time
			}
		}
	}
	/*
	while(1){
		_delay_ms(500);
		nextPill();
	}
		*/
	return 0;
}

/*
 *checks for occurring Dispensing events, and processes them
 */
void checkForEvents(){
	for(uint8_t i =0; i<setTimesCount; i++){
		if(compareTimes(setTimes[i], currentTime)){
			//Pill Dispensing Event!!
			eventOccurred();

#if DEBUGACTIVE ==1
			printTIME(&setTimes[i]);
#endif
			return;
		}
	}
}


void eventOccurred(){
	//dispense the pill:
	nextPill();
	//Notify:
	startNotifications(); //This starts Timer2 which then blinks the LED, generates the Sound and sends the mail
}

