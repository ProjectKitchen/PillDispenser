#include "Arduino.h"

#include "ESP8266WiFiUserConnectionInterface/ESP8266WiFiUserConnectionInterface.h"
#include "html/index.html.h"
#include "html/redirecttonotificationproperties.html.h"
#include "html/notificationproperties.html.h"
#include "html/redirectnotificationproperties.html.h"
#include "html/timesettings.html.h"
#include "html/redirecttimesettings.html.h"
#include "NotificationSender/NotificationSender.h"
#include "TimeFetcher/TimeFetcher.h"

#define NOTIFYEEPROMSTART 200
#define NOTIFYEEPROMSTOP 450


//The setup function is called once at startup of the sketch
ESP8266WiFiUserConnectionInterface userInterface;
NotificationSender notifySender(NOTIFYEEPROMSTART, NOTIFYEEPROMSTOP);
void setup()
{
	// preparing GPIOs
	pinMode(LED_BUILTIN,OUTPUT);
	pinMode(D1, INPUT);
	//uncomment this for clearing the corresponding EEPROM
	//notifySender.deleteParams(NOTIFYEEPROMSTART, NOTIFYEEPROMSTOP);

	userInterface.configAP("PillDispenser", "");
	userInterface.start();

	ESP8266WebServer* server = userInterface.getWebServer();

	server->on("/", [server]{
		String website= INDEXSTART;
		website += INDEXBODY;
		website += userInterface.getConnectionForm();
		website += INDEXEND;
#if DEBUGACTIVE == 1
		Serial.println(website);
#endif
		server->send(200, "text/html", website);
	});
	server->on("/notify", [server]{
		String webPage = NOTIFICATIONPROPERTIES1;
		webPage+=notifySender.getReceivingE_mail();
		webPage+= NOTIFICATIONPROPERTIES2;
		webPage+=notifySender.getSendingSubject();
		webPage+= NOTIFICATIONPROPERTIES3;
		webPage+=notifySender.getSendingE_mail();
		webPage+= NOTIFICATIONPROPERTIES4;
		webPage+=notifySender.getSendingName();
		webPage+= NOTIFICATIONPROPERTIES5;
		webPage+=notifySender.getSMTPHost();
		webPage+= NOTIFICATIONPROPERTIES6;
		webPage+=notifySender.getUserForSMTPHost();
		webPage+= NOTIFICATIONPROPERTIES7;
		webPage+=notifySender.getPasswordForSMTPHost();
		webPage+= NOTIFICATIONPROPERTIES8;
#if DEBUGACTIVE == 1
		Serial.println(webPage);
#endif
		server->send(200, "text/html", webPage);
	});


	server->on("/notifypropsselected", [server]{
		/*
		String response ="";
		for(int i =0; i<server->args();i++){	//TODO: change this with a redirection html page
			response += server->argName(i) + ": " + server->arg(i) + "<br>";
		}
		*/
		notifySender.setReceivingE_mail(server->arg("emailto"));
#if DEBUGACTIVE == 1
		Serial.println(server->arg("emailto"));
#endif
		notifySender.setSendingSubject(server->arg("subject"));
#if DEBUGACTIVE == 1
		Serial.println(server->arg("subject"));
#endif
		notifySender.setSendingE_mail(server->arg("emailfrom"));
#if DEBUGACTIVE == 1
		Serial.println(server->arg("emailfrom"));
#endif
		notifySender.setSendingName(server->arg("displayname"));
#if DEBUGACTIVE == 1
		Serial.println(server->arg("displayname"));
#endif
		notifySender.setSMTPHost(server->arg("selectSMTP"), 2525);	//TODO: why is the default value not working?
#if DEBUGACTIVE == 1
		Serial.println(server->arg("selectSMTP"));
#endif
		notifySender.setUserForSMTPHost(server->arg("username"));
#if DEBUGACTIVE == 1
		Serial.println(server->arg("username"));
#endif
		notifySender.setPasswordForSMTPHost(server->arg("password"));
#if DEBUGACTIVE == 1
		Serial.println(server->arg("password"));
#endif
		//save the received params
		notifySender.saveParams(NOTIFYEEPROMSTART, NOTIFYEEPROMSTOP);
		server->send(200 , "text/html", REDIRECTNOTIFY);
	});
/*
	server->on("/send", [server]{
		notifySender.sendE_mail("Damn it, take your Pill!");
		server->send(200, "text/plain", "Pill taken");
	});
*/
	server->on("/timesettings", [server]{
		//String webpage = TIMESETTINGS1;
		//webpage += TIMESETTINGS2;
		//server->send(200, "text/html", webpage);
		server->send(200, "text/html", TIMESETTINGS);
	});

	server->on("/timeselected", [server]{
		//parse ARGS and send them over UART to ARDUINO UNO:
		Serial.print("AT+PILL:");	//this is used to tell the receiver, that now Pill taking times are send
		for(int i =0; i<server->args(); i++){

			if(server->arg(i).compareTo("RepeatDaily")==0){
				Serial.print('0');
			}
			else if(server->arg(i).compareTo("Monday")==0){
				Serial.print('1');
			}
			else if(server->arg(i).compareTo("Tuesday")==0){
				Serial.print('2');
			}
			else if(server->arg(i).compareTo("Wednesday")==0){
				Serial.print('3');
			}
			else if(server->arg(i).compareTo("Thursday")==0){
				Serial.print('4');
			}
			else if(server->arg(i).compareTo("Friday")==0){
				Serial.print('5');
			}
			else if(server->arg(i).compareTo("Saturday")==0){
				Serial.print('6');
			}
			else if(server->arg(i).compareTo("Sunday")==0){
				Serial.print('7');
			}
			else Serial.print(server->arg(i));
			if((i+1)%3==0) Serial.print(":00");
			Serial.print(':');
		}
		Serial.println("END");
		server->send(200, "text/html", REDIRECTTIME);
	});

	userInterface.setWebsiteWhenConnected(REDIRECT);

}

// The loop function is called in an endless loop
String serialread;
TimeFetcher timeFetch;
void loop()
{
	userInterface.run();
	if(Serial.available()){
		serialread = Serial.readString();
		if(serialread.compareTo("AT+SEND")==0) notifySender.sendE_mail("Take your Pill");
		if(serialread.compareTo("AT+TIME")==0){
			Serial.print("AT+CURRENTTIME:");
			Serial.println( timeFetch.getTime() + ":");
		}
	}

	if(digitalRead(D1)==HIGH){
		userInterface.deleteKnownSSIDs();
		for(int i=0; i<10;i++){
			digitalWrite(LED_BUILTIN,HIGH);
			delay(50);
			digitalWrite(LED_BUILTIN, LOW);
			delay(50);
		}
	}

	digitalWrite(LED_BUILTIN,HIGH);
	delay(500);
	digitalWrite(LED_BUILTIN, LOW);
	delay(500);
}
