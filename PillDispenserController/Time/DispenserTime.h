/*
 * DispenserTime.h
 *
 *  Created on: 1 Dec 2016
 *      Author: felix
 */

#ifndef TIME_DISPENSERTIME_H_
#define TIME_DISPENSERTIME_H_

#include <stdint.h>


#define DISPENSETIMEEEPROMSTART 0
#define DISPENSETIMEEEPROMSTOP 89 //22*4Bytes (size of DispenserTime) + 1 (setTimesCount)


typedef struct DispenserTime{

	uint8_t day;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
} DispenserTime;

extern volatile DispenserTime currentTime;
extern uint8_t setTimesCount;	// counts, how many Times for dispensing have been set
extern DispenserTime setTimes[22];

	uint8_t getDay(DispenserTime* time);

	void setDay(uint8_t day, DispenserTime* time);

	uint8_t getHour(DispenserTime* time);

	void setHour(uint8_t hour, DispenserTime* time);

	uint8_t getMinute(DispenserTime* time);

	void setMinute(uint8_t minute, DispenserTime* time);

	uint8_t getSecond(DispenserTime* time);

	void setSecond(uint8_t second, DispenserTime* time);

	/*
	 * saves the PillEvents & the setTimeCount to the EEPROM
	 */

	void saveTimes();

	/*
	 * restores the Times & TimeCount from the EEPROM
	 */
	void restoreTimes();

	/*
	 * returns 1 if the compared times match,
	 * else 0
	 * notice, it does not compare the seconds
	 */
	uint8_t compareTimes(DispenserTime a, DispenserTime b);

#endif /* TIME_DISPENSERTIME_H_ */
