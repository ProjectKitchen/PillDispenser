/*
 * DispenserMotor.c
 *
 *  Created on: 6 Dec 2016
 *      Author: felix
 */



#include "DispenserMotor.h"
#include <util/delay.h>
#include "xstepper.h"
#include "../ADC/DispenserADC.h"

//prototypes:
float lowpassFilter(float value);
void stepOneCompartment();

//defines:
#define DELTA 50 		//used for detecting maximum in the wave
#define INITIALSTEPS 15	//used for detecting falling or rising amplitude in the wave
#define OFFSET 0


void nextPill(){
	stepOneCompartment();
/*
	for(uint8_t i =0; i<93; i++){
		StepperStepCCW();
		_delay_ms(10);
	}
*/
}

void initStepper(){
	StepperInit();
	uint16_t val = ADC_Read(4);
	val = (uint16_t)lowpassFilter((float)val); //initializes the Filter
	val = (uint16_t)lowpassFilter((float)val);
}


void stepOneCompartment(){
	int16_t dir = 0;
	uint16_t val = ADC_Read(4);
	//reading several times the ADC will ensure proper values
	val = (uint16_t)lowpassFilter((float)val);
	val = ADC_Read(4);
	val = (uint16_t)lowpassFilter((float)val);
	val = ADC_Read(4);
	val = (uint16_t)lowpassFilter((float)val);
	val = ADC_Read(4);
	val = (uint16_t)lowpassFilter((float)val);
	val = ADC_Read(4);
	val = (uint16_t)lowpassFilter((float)val);
	uint16_t val2=0;
	//first we need to determine the direction of the "wave" (up or down)
	for(uint8_t i=0; i<INITIALSTEPS; i++){
		printuint16_t(val);
		StepperStepCCW();
		_delay_ms(10);
		val2 = ADC_Read(4);
		val2 = (uint16_t)lowpassFilter((float)val2);
		dir += (int16_t) (val-val2);
		val = val2;
	}

	while(dir==0){
		StepperStepCCW();
		printuint16_t(val);
		_delay_ms(10);
		val2 = ADC_Read(4);
		val2 = (uint16_t)lowpassFilter((float)val2);
		dir += (int16_t) (val-val2);
		val = val2;
	}

	if(dir<0){
		uint16_t max = 0;
		//search for maximum:
		while(1){
			//search for maximum:
			max = (val>max)?val:max;
			//check if we are decreasing:
			if((val+DELTA) < max) break;
			//stepp further:
			StepperStepCCW();
			_delay_ms(10);
			//read new value:
			val = ADC_Read(4);
			val = (uint16_t)lowpassFilter((float)val);
			printuint16_t(val);
		}
	}
	else{
		uint16_t min = UINT16_MAX;
		//search for minimum:
		while(1){
			//search for minimum:
			min = (val<min)?val:min;
			//check if we are increasing:
			if((val-DELTA) > min) break;
			//stepp further:
			StepperStepCCW();
			_delay_ms(10);
			//read new value:
			val = ADC_Read(4);
			val = (uint16_t)lowpassFilter((float)val);
			printuint16_t(val);
		}
	}
	//if needed, we can now eliminate some offset:
	for(uint8_t i=0; i<OFFSET; i++){
		StepperStepCCW();
		_delay_ms(10);
		val = ADC_Read(4);
		val = (uint16_t)lowpassFilter((float)val);
		printuint16_t(val);
	}
}

float b0 = 0.7,b1 = 0.3, b2 =0.4;
float lowpassFilter(float value){
	static float x_k=0, x_k1 =0, x_k2=0;
	static float ret =0;
	ret = x_k*b0+x_k1*b1+x_k2*b2;
	x_k2 = x_k1;
	x_k1 = x_k;
	x_k=value;
	return ret;
}


