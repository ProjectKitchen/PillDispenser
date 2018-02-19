/*
 * TimeFetcher.cpp
 *
 *  Created on: 28 Nov 2016
 *      Author: felix
 */

#include "TimeFetcher.h"

TimeFetcher::TimeFetcher(){
	if(!Serial){
		Serial.begin(115200);
		while(!Serial){
		}
	}
	udp.begin(localPort);
	//WiFi.hostByName(ntpServerName, timeServerIP);
}

String TimeFetcher::getTime(){
	String ret="";
	WiFi.hostByName(ntpServerName, timeServerIP);
#if DEBUGACTIVE == 1
	Serial.println(timeServerIP);
#endif
	if(WiFi.status() == WL_CONNECTED){
		sendNTPpacket(timeServerIP); // send an NTP packet to a time server
			  // wait to see if a reply is available
			int cb = udp.parsePacket();
			int i=0;
			do{
				cb = udp.parsePacket();
				i++;
				delay(250*i);
			}while(((!cb) && (i<TIMEOUT)));



			  if (!cb) {
#if DEBUGACTIVE == 1
			    Serial.println("no packet yet");
#endif
			  }
			  else {
#if DEBUGACTIVE == 1
			    Serial.print("packet received, length=");
			    Serial.println(cb);
#endif
			    // We've received a packet, read the data from it
			    udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

			    //the timestamp starts at byte 40 of the received packet and is four bytes,
			    // or two words, long. First, esxtract the two words:

			    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
			    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
			    // combine the four bytes (two words) into a long integer
			    // this is NTP time (seconds since Jan 1 1900):
			    unsigned long secsSince1900 = highWord << 16 | lowWord;
#if DEBUGACTIVE == 1
			    Serial.print("Seconds since Jan 1 1900 = " );
			    Serial.println(secsSince1900);

			    // now convert NTP time into everyday time:
			    Serial.print("Unix time = ");
#endif
			    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
			    const unsigned long seventyYears = 2208988800UL;
			    // subtract seventy years:
			    unsigned long epoch = secsSince1900 - seventyYears;
			    // print Unix time:
#if DEBUGACTIVE == 1
			    Serial.println(epoch);
#endif

			    // print the Day, hour, minute and second:
			    Serial.print((((epoch+timeZoneSeconds)/86400L)+4)%7);
			    Serial.print(":");
#if DEBUGACTIVE == 1
			    Serial.print("Day in week:");
			    Serial.println((((epoch+timeZoneSeconds)/86400L)+4)%7);
			    Serial.print("The UTC time is ");       // UTC is the time at Greenwich Meridian (GMT)
			    Serial.print((epoch  % 86400L) / 3600); // print the hour (86400 equals secs per day)
#endif
			    if((((epoch  % 86400L) / 3600) + timeZone)<10) ret+='0';
			    if((((epoch  % 86400L) / 3600) + timeZone)==24) ret+="00";
			    else ret += ((epoch  % 86400L) / 3600) + timeZone;
			    ret += ":";
#if DEBUGACTIVE == 1
			    Serial.print(':');
#endif
			    if ( ((epoch % 3600) / 60) < 10 ) {
			      // In the first 10 minutes of each hour, we'll want a leading '0'
#if DEBUGACTIVE == 1
			      Serial.print('0');
#endif
			      ret += "0";
			    }
#if DEBUGACTIVE == 1
			    Serial.print((epoch  % 3600) / 60); // print the minute (3600 equals secs per minute)
			    Serial.print(':');
#endif
			    ret += (epoch  % 3600) / 60;
			    ret += ":";
			    if ( (epoch % 60) < 10 ) {
			      // In the first 10 seconds of each minute, we'll want a leading '0'
#if DEBUGACTIVE == 1
			      Serial.print('0');
#endif
			      ret += "0";
			    }
#if DEBUGACTIVE == 1
			    Serial.println(epoch % 60); // print the second
#endif
			    ret += epoch % 60;
			  }
	}
	return ret;
}

unsigned long TimeFetcher::sendNTPpacket(IPAddress& address)
{
#if DEBUGACTIVE == 1
  Serial.println("sending NTP packet...");
#endif
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  udp.beginPacket(address, 123); //NTP requests are to port 123
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
}

