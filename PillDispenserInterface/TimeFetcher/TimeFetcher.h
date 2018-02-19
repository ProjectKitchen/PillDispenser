/*
 * TimeFetcher.h
 *
 *  Created on: 28 Nov 2016
 *      Author: felix
 */

#ifndef TIMEFETCHER_TIMEFETCHER_H_
#define TIMEFETCHER_TIMEFETCHER_H_
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#define DEBUGACTIVE 0
#define TIMEOUT 5

class TimeFetcher{
	//class variables
	IPAddress timeServerIP;
	const char* ntpServerName = "time.nist.gov";
	static const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message
	byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets
	// A UDP instance to let us send and receive packets over UDP
	WiFiUDP udp;
	unsigned int localPort = 2390;
	int timeZone = +1;
	int timeZoneSeconds = 3600 * timeZone;
	//methods:
	unsigned long sendNTPpacket(IPAddress& address);
public:
	TimeFetcher();
	/*
	 *returns the time of the set TimeZone
	 *return Format is HH:MM:SS
	 */
	String getTime();
};



#endif /* TIMEFETCHER_TIMEFETCHER_H_ */
