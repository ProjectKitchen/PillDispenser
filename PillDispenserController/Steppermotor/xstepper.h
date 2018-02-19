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
	PARTIES PROVIDE THE PROGRAM �AS IS� WITHOUT WARRANTY OF ANY KIND, EITHER 
	EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
	OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. 
	THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE PROGRAM IS WITH YOU. 
	SHOULD THE PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY 
	SERVICING, REPAIR OR CORRECTION.

******************************************************************************/

#ifndef XSTEEPER_H
#define XSTEEPER_H

/*****************************************
Configuration area
******************************************/

/*
Port where stepper is connected
*/
#define STEPPER_PORT 	C



/*
Position where 1st coil is connected.
All other coils like 2nd,3rd and 4th
must be connected in sequence after the 1st.

For this configuration the connection is 
like:-

PC0 -> Coil 1
PC1 -> Coil 2
PC2 -> Coil 3
PC3 -> Coil 4

Don't connect port pins directly to coil,
use a driver like ULN2003A etc.

*/
#define STEPPER_POS		0	
							


/*
Don't touch the line below
*/

#define _CONCAT(a,b) a##b
 #define PORT(x) _CONCAT(PORT,x)
 #define PIN(x) _CONCAT(PIN,x)
 #define DDR(x) _CONCAT(DDR,x)

#define __STEPPER_PORT 	PORT(STEPPER_PORT)
#define __STEPPER_DDR	DDR(STEPPER_PORT)

void StepperInit();
void StepperStepCW();
void StepperStepCCW();

#endif
