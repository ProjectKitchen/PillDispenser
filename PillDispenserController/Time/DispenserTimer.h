/*
 * DispenserTimer.h
 *
 *  Created on: 4 Dec 2016
 *      Author: felix
 */

#include <stdint.h>

#define SECONDS_TO_WAIT_SOUND	60
#define SECONDS_TO_WAIT_EMAIL 70
//#define COUNTS_TO_MAKE (SECONDS_TO_WAIT_SOUND*2000)	//*2000 because SecondsToWait/(500us) -> SecondsToWait*2000
//#define COUNTS_TO_MAKE2	(SECONDS_TO_WAIT_EMAIL*2000)

void initTimer();
void startNotifications();
void stopNotifications();

//global variable that indicates a minute has passed:
extern volatile uint8_t minutePassed;

