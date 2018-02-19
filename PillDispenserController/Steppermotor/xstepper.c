/******************************************************************************

Title:
	Very basic stepper library for AVR MCUs

Description:
	A unipolar stepper motor connected to PORTC0,1,2,3 via
	driver IC ULN2003A.



Author:
 	Avinash Gupta.
	avinash@eXtremeElectronics.co.in

Copyright:
	eXtreme Electronics, India 2008- 2011

Notice:
	No part of this work can be copied or published in electronic or
	printed form without proper permission from the Original Creators.

	ONLY INTENDED FOR EDUCATIONAL, HOBBY AND PERSONAL USE.
	COMMERCIAL USE IS STRICTLY PROHIBITED.


Disclaimer of Warranty.

	THERE IS NO WARRANTY FOR THE PROGRAM, TO THE EXTENT PERMITTED BY APPLICABLE LAW.
	EXCEPT WHEN OTHERWISE STATED IN WRITING THE COPYRIGHT HOLDERS AND/OR OTHER
	PARTIES PROVIDE THE PROGRAM “AS IS” WITHOUT WARRANTY OF ANY KIND, EITHER 
	EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
	OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. 
	THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE PROGRAM IS WITH YOU. 
	SHOULD THE PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY 
	SERVICING, REPAIR OR CORRECTION.

******************************************************************************/


#include <avr/io.h>

#include "xstepper.h"

void StepperInit()
{
	__STEPPER_DDR|=(0X0F<<STEPPER_POS);
	__STEPPER_PORT|=(1<<STEPPER_POS);
}

void StepperStepCCW()
{
	uint8_t temp=(__STEPPER_PORT & (0x0F<<STEPPER_POS));
	temp=temp>>(STEPPER_POS);
	temp=temp<<1;

	if(temp>(8))
		temp=1;
	
	__STEPPER_PORT=(__STEPPER_PORT & ~(0x0F<<STEPPER_POS))|(temp<<STEPPER_POS);
}

void StepperStepCW()
{
	uint8_t temp=(__STEPPER_PORT & (0x0F<<STEPPER_POS));
	temp=temp>>(STEPPER_POS);
	temp=temp>>1;

	if(temp==0)
		temp=8;
	
	__STEPPER_PORT=(__STEPPER_PORT & ~(0x0F<<STEPPER_POS))|(temp<<STEPPER_POS);
}
