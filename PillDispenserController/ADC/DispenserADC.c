/*
 * DispenserADC.c
 *
 *  Created on: 6 Dec 2016
 *      Author: felix
 */




#include "DispenserADC.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "../Time/DispenserTimer.h"
#include "../UART/DispenserUART.h"

#define COMPARTMENTTHRESHOLD 100

void initADC(){
	//AVCC with external capacitor at AREF pin & Select channel 4
	ADMUX = ((1<<REFS0) | (1<<MUX2));    //Avcc(+5v) as voltage reference
	//Prescaler:
	ADCSRA = ((1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0));	//128
	//Enable the ADC:
	ADCSRA |= (1<<ADEN);


}

void initADCfreerunning(){
	//AVCC with external capacitor at AREF pin & Select channel 5
	ADMUX = ((1<<REFS0) | (1<<MUX2) | (1<<MUX0));    //Avcc(+5v) as voltage reference
	//Prescaler:
	ADCSRA = ((1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0)); //TODO: this might be to slow
	//Enable the ADC:
	ADCSRA |= (1<<ADEN);
	//Enable AutoTrigger:
	ADCSRA |= (1<<ADATE);
	//Enable ADC Interrupts:
	ADCSRA |= (1<<ADIE);
	//Select Trigger source: (Timer0 compareMatch)
	ADCSRB |= ((1<<ADTS1) | (1<<ADTS0));
	//Start First conversion:
	ADCSRA |= (1<<ADSC);
}

uint16_t ADC_Read(uint16_t channel)
{
	ADMUX = (1<<REFS0) + channel;  // select channel MUX
	ADCSRA |= (1<<ADSC);                // single conversion
	while (ADCSRA & (1<<ADSC) )     // wait for completion
	;
	/*
	uint16_t ret = ADCL;
	ret += (ADCH << 8);
	return ret;
	*/
	return ADCW;                                 // return result (16 bit )
}



ISR(ADC_vect){
	cli();
	static uint16_t oldval = UINT16_MAX;
	static int16_t compareto=0;
	static uint8_t count = 10;
	if(oldval==UINT16_MAX){	//this is the condition for the first read
		if(count ==0){	// the first read is not valid, thats why we drop the first 10 reads
			oldval=ADCW;
			count =10;
		}
		count--;
		//printuint16_t(oldval);
	}
	else{
		compareto= oldval-ADCW;

		if((compareto)<0){
			if(compareto< -COMPARTMENTTHRESHOLD){		//Fixme: this is causing problems
				//printint16_t(compareto);
				//printint16_t(-COMPARTMENTTHRESHOLD);
				 oldval = UINT16_MAX;
				 //printuint16_t(oldval);
				stopNotifications();
			}
		}

		else{
			if(compareto> COMPARTMENTTHRESHOLD){
				//printint16_t(compareto);
				 oldval = UINT16_MAX;
				 //printuint16_t(oldval);
				stopNotifications();
			}
		}
	}

	//cli();	//This ISR is not so important, we can interrupt it
	//printuint16_t(ADCW);
/*
	if(ADCW>COMPARTMENTTHRESHOLD){
		//PORTB ^=(1<<PB5);
		stopNotifications();
	}
*/
	sei();

}

