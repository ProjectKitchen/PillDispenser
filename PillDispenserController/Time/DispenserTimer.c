/*
 * DispenserTimer.c
 *
 *  Created on: 4 Dec 2016
 *      Author: felix
 */

#include "DispenserTimer.h"
#include "DispenserTime.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include "../global.h"
#include "../ADC/DispenserADC.h"
#include "../UART/uart.h"

volatile uint8_t minutePassed=0;

static uint32_t count=0;

const uint32_t COUNTS_TO_MAKE = ((uint32_t)SECONDS_TO_WAIT_SOUND* (uint32_t)2000);
const uint32_t COUNTS_TO_MAKE2 = ((uint32_t)SECONDS_TO_WAIT_EMAIL* (uint32_t)2000);

//prototypes:
void initTimer0();

void initTimer(){
	//initialize the timer so that an interrupt is triggerd exactly every 4 seconds
	//triggering every 4 seconds can be done very precise, and if it takes 3 seconds longer than it
	//should, to trigger an dispensing event, that does not harm, because thats so small compared to the
	//time it will take to take the pill.

	//Normal port operation, OC1A/OC1B disconnected: nothing to do.

	// Waveform Generation Mode: CTC:
	TCCR1B |= (1<<WGM12);

	//clkI/O/1024 (From prescaler):
	TCCR1B |= ((1<<CS12) | (1<<CS10));

	//set OCR1A so that every 4s occurs an interrupt:
	OCR1A = 62500;

	//enable output compare interrupt:
	TIMSK1 |= (1<<OCIE1A);

	//also configure Timer2 which will Drive the Notification LED & Piezo
	//using a prescaler of 64 will result in a tick, every 4us.
	//counting to 125 will result in 500us an interrupt. Driving the Piezo with that,
	//it will output a frequenzy of 1/(2*500us) = 1/0.001s = 1kHz

	//set CTC operation mode:
	TCCR2A |= (1<<WGM21);
	//Prescaler 64:
	TCCR2B |= (1<<CS22);
	//set OutputCompare to 125:
	OCR2A = 125;
	//Do not yet enable the interrupt, that is done by startNotifications Function


}

void initTimer0(){
	//Tooggle Port on compare match:
	//CTC mode:
	TCCR0A |= ((1<<WGM01) | (1<<COM0A0));
	DDRD &= ~(1<<PD6); //don't toggle it as an output
	//prescaler 1024:
	TCCR0B |= ((1<<CS02) | (1<<CS00));
	//Interrupt every 8ms:
	OCR0A = 125;
	//enable the interrupt:
	TIMSK0 |= (1<<OCIE0A);
}

void startNotifications(){
	//Enable Timer2 Interrupt:
	TIMSK2 |= (1<<OCIE2A);
	//Enable the PinPB3 as an output, because we want to drive the Piezo from it
	DDRB |= (1<<PB3);
	//Start ADC to check for emptying pill compartment:
	initADCfreerunning();
	//It also needs the Timer0 as a trigger source, so also start this:
	initTimer0();
}
void stopNotifications(){
	count =0;
	//Disable Toggle Pin on Compare match:
	TCCR2A &= ~(1<<COM2A0); 	//toggles pin on compare match
	//Disable Timer2 Interrupt:
	TIMSK2 &= ~(1<<OCIE2A);
	//and reset the Toggled PortPins:
	PORTB &= ~((1<<NOTIFICATION_LED) | (1<<PIEZOPORT));
	//Disable the PinPB3 as an output
	DDRB &= ~(1<<PB3);

	//Disable Timer0 Interrupt:
	TIMSK0 &= ~(1<<OCIE0A);
	//re init The ADC in normal mode:
	initADC();

}


ISR(TIMER1_COMPA_vect){
	cli();
	PORTB ^=(1<<PB5);
	currentTime.second+=4;
	if(currentTime.second>59){
		//one minute has passed, reset the seconds:
		currentTime.second = currentTime.second-60;
		//increment the minute:
		currentTime.minute++;
		//set the global compare Flag, which tells the main, that a minute has passed, and the Dispensing
		//times can be checked for matches
		minutePassed=1;
		if(currentTime.minute > 59){
			//one hour has passed, reset minutes:
			currentTime.minute =0;
			//increment the hour:
			currentTime.hour++;
			if(currentTime.hour > 23){
				//one day passed, reset hours:
				currentTime.hour =0;
				//increment the day:
				currentTime.day++;
				if(currentTime.day > 7){
					currentTime.day=1;
				}
			}
		}
	}
	sei();
}

/*
 * The ISR will blink the LED every 1 second
 * after a certain amount of overflows, the ISR for timer2 will then start to
 * also trigger the sound for the Piezo.
 * after another amount of time, the ISR then will once trigger the sending of a email.
 * It will be turned off, as soon as the ADC which is started simultaneously detects a
 * removal of the dispensing container
 */


ISR(TIMER2_COMPA_vect){	//this ISR is not important, we can interrupt it, so no cli();

	count++;
	if(count == COUNTS_TO_MAKE){
		//we waited for the specified time, now turn Piezo on:
		TCCR2A |= (1<<COM2A0); 	//toggles pin on compare match
	}
	if(count==COUNTS_TO_MAKE2){
		//we waited for specified time, now send the email:
		//set the global bit, to send the mail:s
		cli();
		uart0_puts("AT+SEND");
		sei();
	}

	//toggle the NotificationLED every 500ms
	if((count%1000)==0){
		PORTB ^= (1<<NOTIFICATION_LED);
	}
}

ISR(TIMER0_COMPA_vect){

}
