/*
 * NotificationSender.cpp
 *
 *  Created on: 21 Nov 2016
 *      Author: felix
 */

#include "NotificationSender.h"
#include <base64.h>

/*
 * default constructor for NotificationSender
 */
NotificationSender::NotificationSender(int start, int end){
#if DEBUGACTIVE == 1
	  if(!Serial){
		  Serial.begin(115200);
		  while(!Serial){
		  }
	  }
	  Serial.println("init NotificatioSender");
	  Serial.println(start);
	  Serial.println(end);
#endif
	  EEPROM.begin(512);
#if DEBUGACTIVE ==1
	  Serial.println("Beginn EEPROM");
#endif
	  restoreParams(start, end);

}

void NotificationSender::setSendingE_mail(String mail){
	this->sendingE_mail = mail;
}

void NotificationSender::setSendingName(String name){
	this->sendingName = name;
}

void NotificationSender::setUserForSMTPHost(String user){
	this->userClear = user;
	this->userBase64 = base64::encode(user);
}

void NotificationSender::setPasswordForSMTPHost(String pw){
	this->smtpPasswordClear = pw;
	this->smtpPasswordBas64 = base64::encode(pw);
}


void NotificationSender::setReceivingE_mail(String mail){
	this->receivingE_mail=mail;
}

void NotificationSender::setSendingSubject(String subject){
	this->subject = subject;
}

void NotificationSender::setSMTPHost(String host = "mail.smtp2go.com", int port = 2525){
	this->smtpHost = host;
	this->smtpPort = port;
}

bool NotificationSender::sendE_mail(String message = ""){
	if(WiFi.status() != WL_CONNECTED) return false;
	//connect to the smtp service:
	if(client.connect(this->smtpHost.c_str(), this->smtpPort) == 1) {
#if DEBUGACTIVE ==1
	    Serial.println(F("connected smtp2go"));
#endif
	} else {
#if DEBUGACTIVE ==1
	    Serial.println(F("connection failed smtp2go"));
#endif
	    return false;
	}
	if(!eRcv()) {
#if DEBUGACTIVE ==1
		Serial.println("before ehlo");
#endif
		return false ;
	}

#if DEBUGACTIVE ==1
	Serial.println(F("Sending ehlo"));
#endif

	client.println("EHLO ME");
	if(!eRcv()) {
#if DEBUGACTIVE ==1
		Serial.println("ehlo");
#endif
		return false ;
	}

#if DEBUGACTIVE ==1
	Serial.println(F("Sending auth login"));
#endif

	client.println("auth login");
	if(!eRcv()) {
#if DEBUGACTIVE ==1
		Serial.println("auth");
#endif
		return false ;
	}

#if DEBUGACTIVE ==1
	Serial.println(F("Sending User"));
#endif

	client.println(this->userBase64);
	if(!eRcv()) {
#if DEBUGACTIVE ==1
		Serial.println("User");
#endif
		return false ;
	}

#if DEBUGACTIVE ==1
	Serial.println(F("Sending Password"));
#endif

	client.println(this->smtpPasswordBas64);
	if(!eRcv()) {
#if DEBUGACTIVE ==1
		Serial.println("Password");
#endif
		return false ;
	}

#if DEBUGACTIVE ==1
	Serial.println(F("Sending From"));
#endif
	String mailfrom = "mail from:<";
	mailfrom += this->sendingE_mail;
	mailfrom += ">";
	client.println(mailfrom);
	if(!eRcv()) {
#if DEBUGACTIVE ==1
		Serial.println("From");
#endif
		return false ;
	}

#if DEBUGACTIVE ==1
	Serial.println(F("Sending To"));
#endif
	String mailto = "rcpt to:<";
	mailto += this->receivingE_mail;
	mailto += ">";
	client.println(mailto);
	if(!eRcv()) {
#if DEBUGACTIVE ==1
		Serial.println("To");
#endif
		return false ;
	}

#if DEBUGACTIVE ==1
	Serial.println(F("Sending Data"));
#endif

	client.println("DATA");
	if(!eRcv()) {
#if DEBUGACTIVE ==1
		Serial.println("Data");
#endif
		return false ;
	}

	String receiver = "to: ";
	receiver += this->receivingE_mail;
	client.println(receiver);

	client.println("From: " + this->sendingName);

	client.println("Subject: " + this->subject);

	client.println(message);

	client.println(".");

	if(!eRcv()) {
	#if DEBUGACTIVE ==1
			Serial.println("After sending");
	#endif
			return false ;
		}

#if DEBUGACTIVE ==1
	Serial.println(F("Sending QUIT"));
#endif

	client.println("QUIT");
	if(!eRcv()) {
#if DEBUGACTIVE ==1
		Serial.println("after quit");
#endif
		return false ;
	}

	client.stop();

#if DEBUGACTIVE ==1
		Serial.println("disconnected");
#endif

	return true;
}


byte NotificationSender::eRcv(){
	  byte respCode;
	  byte thisByte;
	  int loopCount = 0;

	  while(!client.available()) {
	    delay(1);
	    loopCount++;

	    // if nothing received for 10 seconds, timeout
	    if(loopCount > 10000) {
	      client.stop();
#if DEBUGACTIVE ==1
	      Serial.println(F("10 sec \r\nTimeout"));
#endif
	      return 0;
	    }
	  }

	  respCode = client.peek();

	  while(client.available())
	  {
	    thisByte = client.read();
#if DEBUGACTIVE ==1
	    Serial.write(thisByte);
#endif
	  }

	  if(respCode >= '4')
	  {
	    efail();
	    return 0;
	  }

	  return 1;
}

void NotificationSender::efail(){
	byte thisByte = 0;
	int loopCount = 0;

	client.println(F("QUIT"));

	while(!client.available()) {
		delay(1);
	    loopCount++;

	    // if nothing received for 10 seconds, timeout
	    if(loopCount > 10000) {
	      client.stop();
#if DEBUGACTIVE ==1
	      Serial.println(F("efail \r\nTimeout"));
#endif
	      return;
	    }
	  }

	  while(client.available())
	  {
	    thisByte = client.read();
#if DEBUGACTIVE ==1
	    Serial.write(thisByte);
#endif
	  }

	  client.stop();
#if DEBUGACTIVE ==1
	  Serial.println(F("disconnected"));
#endif
}

String NotificationSender::getSendingE_mail(){
	return this->sendingE_mail;
}

String NotificationSender::getSendingName(){
	return this->sendingName;
}

String NotificationSender::getUserForSMTPHost(){
	return this->userClear;
}

String NotificationSender::getPasswordForSMTPHost(){
	return this->smtpPasswordClear;
}

String NotificationSender::getReceivingE_mail(){
	return this->receivingE_mail;
}

String NotificationSender::getSendingSubject(){
	return this->subject;
}

String NotificationSender::getSMTPHost(){
	return this->smtpHost;
}

bool NotificationSender::saveParams(int start, int end){
	this->deleteParams(start, end);
	int currentEEPROMposition = start;
	if((this->sendingE_mail.length()+ this->sendingName.length() + this->userClear.length() + this->userBase64.length() + this->smtpPasswordClear.length() + this->smtpPasswordBas64.length() + this->subject.length() + this->smtpHost.length() + this->receivingE_mail.length() + 8) > (unsigned int) (end-start)){
#if DEBUGACTIVE ==1
		Serial.println("too large to save");
#endif
		return false;
	}

	//write to EEPROM:
	for(unsigned int i = 0; i < this->sendingE_mail.length(); i++, currentEEPROMposition++){
		EEPROM.write(currentEEPROMposition, (uint8_t) this->sendingE_mail.charAt(i));
	}
	currentEEPROMposition++;

	for(unsigned int i = 0; i<this->sendingName.length(); i++, currentEEPROMposition++){
		EEPROM.write(currentEEPROMposition, (uint8_t) this->sendingName.charAt(i));
	}
	currentEEPROMposition++;

	for(unsigned int i = 0; i<this->userClear.length(); i++, currentEEPROMposition++){
		EEPROM.write(currentEEPROMposition, (uint8_t) this->userClear.charAt(i));
	}
	currentEEPROMposition++;

	for(unsigned int i = 0; i<this->userBase64.length(); i++, currentEEPROMposition++){
		EEPROM.write(currentEEPROMposition, (uint8_t) this->userBase64.charAt(i));
	}
	currentEEPROMposition++;

	for(unsigned int i = 0; i<this->smtpPasswordClear.length(); i++, currentEEPROMposition++){
		EEPROM.write(currentEEPROMposition, (uint8_t) this->smtpPasswordClear.charAt(i));
	}
	currentEEPROMposition++;

	for(unsigned int i = 0; i<this->smtpPasswordBas64.length(); i++, currentEEPROMposition++){
		EEPROM.write(currentEEPROMposition, (uint8_t) this->smtpPasswordBas64.charAt(i));
	}
	currentEEPROMposition++;

	for(unsigned int i = 0; i<this->subject.length(); i++, currentEEPROMposition++){
		EEPROM.write(currentEEPROMposition, (uint8_t) this->subject.charAt(i));
	}
	currentEEPROMposition++;

	for(unsigned int i = 0; i<this->smtpHost.length(); i++, currentEEPROMposition++){
		EEPROM.write(currentEEPROMposition, (uint8_t) this->smtpHost.charAt(i));
	}
	currentEEPROMposition++;

	for(unsigned int i = 0; i<this->receivingE_mail.length(); i++, currentEEPROMposition++){
		EEPROM.write(currentEEPROMposition, (uint8_t) this->receivingE_mail.charAt(i));
	}
	EEPROM.commit();

	return true;
}

bool NotificationSender::deleteParams(int start, int end){
	for (;start<end;start++){
		EEPROM.write(start, 0);
	}
	EEPROM.commit();
	return true;
}

bool NotificationSender::restoreParams(int start, int end){
	int currentEEPROMposition = start;
	if(!EEPROM.read(start)){
#if DEBUGACTIVE == 1
		Serial.println("nothing to read");
#endif
		return false;
	}

#if DEBUGACTIVE == 1
		Serial.println("start reading:");
#endif
	//sendingEmail
	String read="";
	while(EEPROM.read(currentEEPROMposition)){
		read+= (char) EEPROM.read(currentEEPROMposition++);
		if(currentEEPROMposition>=end) return false;
	}
	this->sendingE_mail=read;
	currentEEPROMposition++;
#if DEBUGACTIVE == 1
		Serial.println("sending Email:" + this->sendingE_mail);
#endif

	//sendingName
	read="";
	while(EEPROM.read(currentEEPROMposition)){
		read+= (char) EEPROM.read(currentEEPROMposition++);
		if(currentEEPROMposition>=end) return false;
	}
	this->sendingName=read;
	currentEEPROMposition++;
#if DEBUGACTIVE == 1
		Serial.println("sending Name:" + this->sendingName);
#endif

	//userClear
	read="";
	while(EEPROM.read(currentEEPROMposition)){
		read+= (char) EEPROM.read(currentEEPROMposition++);
		if(currentEEPROMposition>=end) return false;
	}
	this->userClear=read;
	currentEEPROMposition++;
#if DEBUGACTIVE == 1
		Serial.println("user:" + this->userClear);
#endif
	//userBase64
	read="";
	while(EEPROM.read(currentEEPROMposition)){
		read+= (char) EEPROM.read(currentEEPROMposition++);
		if(currentEEPROMposition>=end) return false;
	}
	this->userBase64=read;
	currentEEPROMposition++;
#if DEBUGACTIVE == 1
		Serial.println("user b64:" + this->userBase64);
#endif

	//smtpPasswordClear
	read="";
	while(EEPROM.read(currentEEPROMposition)){
		read+= (char) EEPROM.read(currentEEPROMposition++);
		if(currentEEPROMposition>=end) return false;
	}
	this->smtpPasswordClear=read;
	currentEEPROMposition++;
#if DEBUGACTIVE == 1
		Serial.println("smtp password:" + this->smtpPasswordClear);
#endif
	//smtpPasswordBase64
	read="";
	while(EEPROM.read(currentEEPROMposition)){
		read+= (char) EEPROM.read(currentEEPROMposition++);
		if(currentEEPROMposition>=end) return false;
	}
	this->smtpPasswordBas64=read;
	currentEEPROMposition++;
#if DEBUGACTIVE == 1
		Serial.println("password b64:" + this->smtpPasswordBas64);
#endif

	//subject
	read="";
	while(EEPROM.read(currentEEPROMposition)){
		read+= (char) EEPROM.read(currentEEPROMposition++);
		if(currentEEPROMposition>=end) return false;
	}
	this->subject=read;
	currentEEPROMposition++;
#if DEBUGACTIVE == 1
		Serial.println("subject:" + this->subject);
#endif
	//smtpHost
	read="";
	while(EEPROM.read(currentEEPROMposition)){
		read+= (char) EEPROM.read(currentEEPROMposition++);
		if(currentEEPROMposition>=end) return false;
	}
	this->smtpHost=read;
	currentEEPROMposition++;
#if DEBUGACTIVE == 1
		Serial.println("smtp Host:" + this->smtpHost);
#endif

	//receivingEmail
	read="";
	while(EEPROM.read(currentEEPROMposition)){
		read+= (char) EEPROM.read(currentEEPROMposition++);
		if(currentEEPROMposition>=end) return false;
	}
	this->receivingE_mail=read;
	currentEEPROMposition++;
#if DEBUGACTIVE == 1
		Serial.println("receiving Email:" + this->receivingE_mail);
#endif
#if DEBUGACTIVE ==1
	Serial.println("RestoredParams:");
	Serial.println(this->sendingE_mail);
	Serial.println(this->sendingName);
	Serial.println(this->userClear);
	Serial.println(this->userBase64);
	Serial.println(this->smtpPasswordClear);
	Serial.println(this->smtpPasswordBas64);
	Serial.println(this->subject);
	Serial.println(this->smtpHost);
	Serial.println(this->receivingE_mail);
#endif

	return true;
}
