/*
 * DispenserTime.cpp
 *
 *  Created on: 1 Dec 2016
 *      Author: felix
 */

#include "DispenserTime.h"
#include <string.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>

#define DEBUGACTIVE 0

#if DEBUGACTIVE ==1
#include "../UART/uart.h"
#endif

volatile DispenserTime currentTime;	//volatile, because it is modified by Interrupt
uint8_t setTimesCount=0;	// counts, how many Times for dispensing have been set
DispenserTime setTimes[22];

//prototypes:
/*
 * saves the given DispenserTime to the given pos in EEPROM
 * returns the next available Pos in EEPROM
 */
uint8_t saveTimeToEEPROM(DispenserTime* Time, uint8_t eepromPos);

/*
 * reads the Time from the EEPROM Pos to the DispenserTime
 * returns the next EEPROM Pos to read from
 */
uint8_t getTimeFromEEPROM(DispenserTime* Time, uint8_t eepromPos);

/*
 * saves setTimesCount variable to the EEPROM
 * returns the next available Pos in EEPROM
 */
uint8_t saveTimesCountToEEPROM();


/*
 * reads the number of TimeCounts to setTimeCounts (PillDispensingEvents) from the EEPROM
 * returns next EEPROM Position to read/ write from
 */
uint8_t getTimesCountFromEEPROM();



//---------------------------------------------------------------------------------------------//

uint8_t getDay(DispenserTime* time){
	return time->day;
}

void setDay(uint8_t day, DispenserTime* time){
	time->day=day;
}

uint8_t getHour(DispenserTime* time){
	return time->hour;
}

void setHour(uint8_t hour, DispenserTime* time){
	time->hour=hour;
}

uint8_t getMinute(DispenserTime* time){
	return time->minute;
}

void setMinute(uint8_t minute, DispenserTime* time){
	time->minute = minute;
}

uint8_t getSecond(DispenserTime* time){
	return time->second;
}

void setSecond(uint8_t second, DispenserTime* time){
	time->second=second;
}

void saveTimes(){
	uint8_t eepromPos = DISPENSETIMEEEPROMSTART;
	eepromPos = saveTimesCountToEEPROM();

	for(int i=0; i< setTimesCount; i++){
		//save times:
		eepromPos=saveTimeToEEPROM(&setTimes[i], eepromPos);
	}

}


uint8_t saveTimesCountToEEPROM(){
	cli();
	eeprom_busy_wait();
	uint8_t eepromPos = DISPENSETIMEEEPROMSTART;
	eeprom_write_byte((uint8_t*)eepromPos,setTimesCount);
	sei();
	return eepromPos+1;
}


uint8_t saveTimeToEEPROM(DispenserTime* Time, uint8_t eepromPos){
	cli();
	eeprom_busy_wait();
	eeprom_write_byte((uint8_t*)eepromPos, Time->day);
	eepromPos++;
	eeprom_busy_wait();
	eeprom_write_byte((uint8_t*)eepromPos, Time->hour);
	eepromPos++;
	eeprom_busy_wait();
	eeprom_write_byte((uint8_t*)eepromPos, Time->minute);
	eepromPos++;
	eeprom_busy_wait();
	eeprom_write_byte((uint8_t*)eepromPos, Time->second);
	eepromPos++;
	sei();
	return eepromPos;
}

void restoreTimes(){
	uint8_t eepromPos = getTimesCountFromEEPROM();
	for (uint8_t i =0; i<setTimesCount; i++){
		eepromPos = getTimeFromEEPROM(&setTimes[i],eepromPos);
	}
}

uint8_t getTimesCountFromEEPROM(){		//TODO: rename me to restoreTimesCount
	cli();
	uint8_t eepromPos = DISPENSETIMEEEPROMSTART;
	eeprom_busy_wait();
	setTimesCount = eeprom_read_byte((uint8_t*) eepromPos);
	sei();
	return eepromPos+1;
}

uint8_t getTimeFromEEPROM(DispenserTime* Time, uint8_t eepromPos){
	cli();
	eeprom_busy_wait();
	Time->day = eeprom_read_byte((uint8_t*)eepromPos);
	eepromPos++;
	eeprom_busy_wait();
	Time->hour = eeprom_read_byte((uint8_t*)eepromPos);
	eepromPos++;
	eeprom_busy_wait();
	Time->minute = eeprom_read_byte((uint8_t*)eepromPos);
	eepromPos++;
	eeprom_busy_wait();
	Time->second = eeprom_read_byte((uint8_t*)eepromPos);
	eepromPos++;
	sei();
	return eepromPos;
}

uint8_t compareTimes(DispenserTime a, DispenserTime b){
	if(a.day==0 || b.day==0 || a.day==b.day ){
#if DEBUGACTIVE ==1
		uart0_puts("Day match\n");
#endif
		//the days match. (0 means every day, which matches in every case)
		if(a.hour==b.hour){
#if DEBUGACTIVE ==1
			uart0_puts("hour match\n");
#endif
			if(a.minute==b.minute) return 1;
			else return 0;
		}
		else{
#if DEBUGACTIVE ==1
			uart0_puts("No hour match\n");
#endif
			return 0;
		}
	}
	else {
#if DEBUGACTIVE ==1
		uart0_puts("No Day match\n");
#endif
		return 0;
	}
}
