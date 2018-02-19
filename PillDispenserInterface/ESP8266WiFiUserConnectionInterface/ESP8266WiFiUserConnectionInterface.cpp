/*
 * ESP8266WiFiConnectioInterface.h
 *
 *  Created on: 18 Nov 2016
 *      Author: felix mitterer
 *      e-mail: felix.mitterer@technikum-wien.at
 */

#include "../ESP8266WiFiUserConnectionInterface/ESP8266WiFiUserConnectionInterface.h"


ESP8266WiFiUserConnectionInterface::ESP8266WiFiUserConnectionInterface(){
	this->server = new ESP8266WebServer(80);
	this->dnsserver = new DNSServer;
	this->setup();
#if DEBUGACTIVE ==1
	Serial.println("WebServer initialized");
#endif
	this->startUserConnectionInterface();
	this->server->begin();
}

ESP8266WiFiUserConnectionInterface::ESP8266WiFiUserConnectionInterface(ESP8266WebServer server){
	this->dnsserver = new DNSServer;
	this->server = &server;
	this->setup();
	this->startUserConnectionInterface();
	this->server->begin(); //TODO: this must be tested, if it works
}

ESP8266WiFiUserConnectionInterface::ESP8266WiFiUserConnectionInterface(ESP8266WebServer server, String uri){
	this->dnsserver = new DNSServer;
	this->server = &server;
	this->setup();
	this->defaultURI = uri;
	this->startUserConnectionInterface();
	this->server->begin(); //TODO: this must be tested, if it works
}

ESP8266WiFiUserConnectionInterface::ESP8266WiFiUserConnectionInterface(DNSServer dnsserver){
	this->server = new ESP8266WebServer(80);
	this->dnsserver = &dnsserver;
	this->setup();
#if DEBUGACTIVE ==1
	Serial.println("WebServer initialized");
#endif
	this->startUserConnectionInterface();
	this->server->begin();
}

ESP8266WiFiUserConnectionInterface::ESP8266WiFiUserConnectionInterface(ESP8266WebServer server, DNSServer dnsserver){
	this->dnsserver = &dnsserver;
	this->server = &server;
	this->setup();
	this->startUserConnectionInterface();
	this->server->begin(); //TODO: this must be tested, if it works
}

ESP8266WiFiUserConnectionInterface::ESP8266WiFiUserConnectionInterface(ESP8266WebServer server, String uri, DNSServer dnsserver){
	this->dnsserver = &dnsserver;
	this->server = &server;
	this->setup();
	this->defaultURI = uri;
	this->startUserConnectionInterface();
	this->server->begin(); //TODO: this must be tested, if it works
}

void ESP8266WiFiUserConnectionInterface::setup(){
#if DEBUGACTIVE == 1
	  //start serial:
	  if(!Serial){
		  Serial.begin(115200);
		  while(!Serial){
		  }
	  }
#endif
	  //Start EEPROM:
	  EEPROM.begin(512);
#if DEBUGACTIVE ==1
	  Serial.println("Beginn EEPROM");
#endif

	  //Setup as AccesPoint and Station
	  WiFi.mode(WIFI_AP_STA);
	  delay(100);	//needed for scanning for networks
	  //Set AccesPoint settings:
	  WiFi.softAPConfig(apIPAddress,apDefaultGateway, apSubNetmask);
	  WiFi.softAP(apSSID.c_str(), apPassword.c_str());

	  this->dnsserver->setTTL(300);
	  this->dnsserver->setErrorReplyCode(DNSReplyCode::ServerFailure);

	if(this->dnsserver->start(this->dnsport, this->dns.c_str(), WiFi.softAPIP())){
#if DEBUGACTIVE ==1
	Serial.println("DNSServer initialized:");
	Serial.println(dns + "at IP:" + WiFi.softAPIP().toString());
#endif
	}else {
#if DEBUGACTIVE ==1
	Serial.println("DNSServer NOT initialized");
#endif
	}

#if DEBUGACTIVE == 1
	Serial.println("Acces Point IP-Address:");
	Serial.println(WiFi.softAPIP());
#endif
	this->server->onNotFound([&]() {
	    String message = "Page Not Found!\n\n";
	    message += "URI: ";
	    message += this->server->uri();
	    server->send(404, "text/plain", message);
	});

#if DELETENETWORKSONSTARTUP ==1
	this->deleteKnownSSIDs();
#endif

	//here I would like to connect to available networks, but this is unfortunately not possible,
	//because the WiFi instance needs some time to initialize
	//workaround is the start method, which now instead contains the code to connect to available, known networks
}


void ESP8266WiFiUserConnectionInterface::setURI(String uri){
	this->defaultURI = uri;
#if DEBUGACTIVE == 1
	Serial.println(this->defaultURI);
#endif
	this->server->on(this->defaultURI.c_str(), [&](){
#if DEBUGACTIVE == 1
		Serial.println("/ESPUserInterface requested");
#endif
	    server->send(200, "text/html",composeWebSiteForDropDownMenue(generateDropDownOptions()));
	});
}

void ESP8266WiFiUserConnectionInterface::setURI(const char* uri){
	this->defaultURI = String(uri);
#if DEBUGACTIVE == 1
	Serial.println(this->defaultURI);
#endif
	this->server->on(this->defaultURI.c_str(), [&](){
#if DEBUGACTIVE == 1
		Serial.println("/ESPUserInterface requested");
#endif
	    server->send(200, "text/html",composeWebSiteForDropDownMenue(generateDropDownOptions()));
	});
}

void ESP8266WiFiUserConnectionInterface::startUserConnectionInterface(){
	//this->server.on(this->defaultURI.c_str(), ESP8266WiFiUserConnectioInterface::showAvailableNetworks);
#if DEBUGACTIVE ==1
	//Serial.println(composeWebSiteForDropDownMenue(generateDropDownOptions()));
#endif

	this->server->on(this->defaultURI.c_str(), [&](){
#if DEBUGACTIVE == 1
		Serial.println("/ESPUserInterface requested");
#endif
		this->connectingTimeout=20;
	    server->send(200, "text/html",composeWebSiteForDropDownMenue(generateDropDownOptions()));
	});

	this->server->on("/wifiselected", [&](){
		if(this->server->args()!=3){
			//error occured:
#if DEBUGACTIVE ==1
			Serial.println("Wrong amount of args to operate wifiselected");
			this->server->send(200, "text/plain", "Wrong amount of arguments please retry");
#endif
		}else{
			//parse the server args:
			if(this->server->arg(0)!=NULL){
				if(this->ssid.compareTo(this->server->arg(0))!=0){
					//if we have a new ssid than change the ssid & reset timeout
					this->ssid = this->server->arg(0);
					this->connectingTimeout=UCI_DEFAULTCONNECTINGTIMEOUT;
#if DEBUGACTIVE == 1
					Serial.println(String("New SSID to connect to: ") + this->ssid);
#endif
				}
			}
			if(this->server->arg(1)!=NULL){
				if(this->password.compareTo(this->server->arg(1))!=0){
					//if we have a new password than change the password & reset timeout
					this->password = this->server->arg(1);
					this->connectingTimeout=UCI_DEFAULTCONNECTINGTIMEOUT;
#if DEBUGACTIVE == 1
					Serial.println(String("New Password for connecting: ") + this->password);
#endif
				}
			}
			if(WiFi.isConnected()){
#if DEBUGACTIVE == 1
				Serial.println("Connected");
#endif
				//check if we are connected to the network we want to connect to:
				if(WiFi.SSID()!=NULL){
					if(this->ssid.compareTo(WiFi.SSID())==0){
#if DEBUGACTIVE == 1
						Serial.println(String("Connected to") + WiFi.SSID());
#endif
						//save the Network to known networks: //TODO test this
						WiFiSSIDPASSWORD save;
						save.setSSID(this->ssid);
						save.setPassword(this->password);
						this->setKnownSSID(save);

						//we don't have to reconnect
						this->connectingTimeout=UCI_DEFAULTCONNECTINGTIMEOUT;
						this->server->send(200, "text/html", this->connectedResponse);
					}
					else{
						//we need to connect to the network:
						this->connectToNetwork();
					}
				}
				else{
					//we need to connect to the network:
					this->connectToNetwork();
				}
			}
			else{
				//we need to connect to the network:
				this->connectToNetwork();
			}
		}
	});

}

void ESP8266WiFiUserConnectionInterface::start(){
	//read saved networks:
	int nwifis;
	delay(100);
	nwifis = WiFi.scanNetworks();	//Fixme: this is not working ?!
	WiFiSSIDPASSWORD saved;
	while(this->getKnownSSIDs(saved)){
#if DEBUGACTIVE == 1
		Serial.println(saved.getSSID());
#endif
		//compare to found networks (if match: connect)
#if DEBUGACTIVE == 1
//		Serial.println(nwifis);
#endif
		for(int i=0;i<nwifis;i++){
#if DEBUGACTIVE == 1
			//Serial.println(saved.getSSID() + "=?" + WiFi.SSID(i));
#endif
			if(saved.getSSID().compareTo(WiFi.SSID(i))==0){
				//connect
#if DEBUGACTIVE == 1
				Serial.println("Matching network found");
#endif
				WiFi.begin(saved.getSSID().c_str(), saved.getPassword().c_str());
				return;
			}
		}
	}
}

void ESP8266WiFiUserConnectionInterface::run(){
	this->dnsserver->processNextRequest();
	this->server->handleClient();
	if(this->connectingTimeout < UCI_DEFAULTCONNECTINGTIMEOUT && this->isConnected()){
		this->connectingTimeout = UCI_DEFAULTCONNECTINGTIMEOUT;
		this->server->send(200, "text/html", this->connectedResponse);	//workaround for connection failure
	}
}

String ESP8266WiFiUserConnectionInterface::composeWebSiteForDropDownMenue(String dropDownMenueElements){
	String myWebSite = "<!DOCTYPE html><html><head><title>";
	myWebSite += pageTitle + "</title>";
	myWebSite += "</head><body><h1>Please select a WiFi to connect to</h1>" + this->connectionStatusHTML() + "<form action=\"/wifiselected\"><label for=\"selectWiFi\">SelectWiFi</label><select name=\"selectWiFi\">";
	myWebSite += dropDownMenueElements;
	myWebSite += "</select><br><label for=\"Password\">Password</label><input type=\"text\" name=\"Password\"><br><input type=\"submit\" name=\"Connect\" value=\"Connect\"></form></body></html>";
	return myWebSite;
}

String ESP8266WiFiUserConnectionInterface::generateDropDownOptions(){
	String options;
	int nwifis;
	nwifis = WiFi.scanNetworks();
	for(int i=0;i<nwifis;i++){
		options+= "<option>" + WiFi.SSID(i) + "</option>";
	}
	return options;
}

void ESP8266WiFiUserConnectionInterface:: serverAdd(const char* uri, ESP8266WebServer::THandlerFunction handler){
	this->server->on(uri, handler);
}

void ESP8266WiFiUserConnectionInterface::connectToNetwork(){
	if(this->connectingTimeout==UCI_DEFAULTCONNECTINGTIMEOUT){
		this->connectingTimeout--;
		//Start connecting to the Network:
		WiFi.begin(this->ssid.c_str(), this->password.c_str());		//Fixme: from time to time when connecting the connection to connected devices seems to drop
		delay(50);	//this is needed to prevent a network disconnection. No idea why this happens, but this delay fixes it
#if DEBUGACTIVE == 1
		Serial.print("Connecting.");
#endif
		this->server->send(200, "text/html", this->composeWaitWhileConnecting());
	}
	else{
#if DEBUGACTIVE == 1
		Serial.print(".");
#endif
		if(this->connectingTimeout>0){
			this->connectingTimeout--;
			delay(50);	//this is needed to prevent a network disconnection. No idea why this happens, but this delay fixes it
			this->server->send(200, "text/html", this->composeWaitWhileConnecting());
		}
		else{
			this->connectingTimeout=UCI_DEFAULTCONNECTINGTIMEOUT;
			String message = "Could not conect to: ";
			message += this->ssid;
			message += " please try again and check the password";
			this->server->send(200, "text/plain", message);
		}
	}
}


void ESP8266WiFiUserConnectionInterface::setWebsiteWhenConnected(String website){
	this->connectedResponse = website;
}


bool ESP8266WiFiUserConnectionInterface::isConnected(){
	if(WiFi.isConnected()){
		if(this->ssid.compareTo(WiFi.SSID())==0){
			return true;
		}
		return false;
	}
	return false;
}

String ESP8266WiFiUserConnectionInterface::connectionStatusHTML(){
	String str ="<div name= \"ConnectionStatus\"> <p> Connection Status: ";
	if(WiFi.isConnected()) str += "Connected";
	else str += "Not Connected";
	str += "</p></div>";
	return str;
}

ESP8266WebServer* ESP8266WiFiUserConnectionInterface::getWebServer(){
	return  this->server;
}

String ESP8266WiFiUserConnectionInterface::getConnectionForm(){
	String form = this->connectionStatusHTML();
	form += "<form action=\"/wifiselected\"><label for=\"selectWiFi\">SelectWiFi</label><select name=\"selectWiFi\">";
	form += generateDropDownOptions();
	form += "</select><br><label for=\"Password\">Password</label><input type=\"text\" name=\"Password\"><br><input type=\"submit\" name=\"Connect\" value=\"Connect\"></form>";
	return form;
}

void ESP8266WiFiUserConnectionInterface::setStyle(String style = "body{background-color: #ccccb3;}", String spinnercolor = "border-top: 16px solid #333"){
	this->styleAtributes = style;
	this->spinnerStyle = spinnercolor;
}

String ESP8266WiFiUserConnectionInterface::composeWaitWhileConnecting(){
	String wait = WAITWHILECONNECTINGSTART;
	wait += this->styleAtributes;
	wait += WAITWHILECONNECTINGINTERMEDIATE;
	wait += this->spinnerStyle;
	wait += WAITWHILECONNECTINGEND;
	return wait;
}

void ESP8266WiFiUserConnectionInterface::configAP(String ssid, String password){
	this->apSSID = ssid;
	this->apPassword = password;
	WiFi.softAP(apSSID.c_str(), apPassword.c_str());
}



bool ESP8266WiFiUserConnectionInterface::setKnownSSID(WiFiSSIDPASSWORD save){
	WiFiSSIDPASSWORD known;
	this->currentEEPROMposition = KNOWNWIFIEEPROMSTART;
	while(this->getKnownSSIDs(known)){
		if((known.getSSID().compareTo(save.getSSID())==0) && (known.getPassword().compareTo(save.getPassword())==0)){
#if DEBUGACTIVE ==1
			Serial.println("Already know this network");
#endif
			return true;
		}
	}
	const String ssid = save.getSSID();
	const String password = save.getPassword();
	if((ssid.length() + password.length() +1) > (unsigned int)(KNOWNWIFIEEPROMSTOP-currentEEPROMposition)) return false;
	//else we can write the wifi to our saved ones:
#if DEBUGACTIVE ==1
	Serial.println("Writing:" + ssid);
#endif
	for (unsigned int i = 0; i<ssid.length(); i++, currentEEPROMposition++){
		EEPROM.write(currentEEPROMposition, (uint8_t) ssid.charAt(i));
	}
	currentEEPROMposition++;		//set it one further to write password

#if DEBUGACTIVE ==1
	Serial.println("Writing:" + password);
#endif

	for (unsigned int i = 0; i<password.length(); i++, currentEEPROMposition++){
		EEPROM.write(currentEEPROMposition, (uint8_t) password.charAt(i));
	}
	currentEEPROMposition++;		//set it one further to write or read next element
	EEPROM.commit();
	return true;

}

bool ESP8266WiFiUserConnectionInterface::deleteKnownSSIDs(){
#if DEBUGACTIVE ==1
	Serial.println("delete saved Wifis");
#endif
	for(int i = KNOWNWIFIEEPROMSTART; i<KNOWNWIFIEEPROMSTOP; i++){
		EEPROM.write(i,0);
	}
	EEPROM.commit();
	return true;
}

bool ESP8266WiFiUserConnectionInterface::getKnownSSIDs(WiFiSSIDPASSWORD& knownWiFI){
	if(currentEEPROMposition>=KNOWNWIFIEEPROMSTOP){
		currentEEPROMposition= KNOWNWIFIEEPROMSTART;
		return false;
	}
	String ssid ="";
	String password="";
	//read SSID
	while(EEPROM.read(currentEEPROMposition)){
		ssid+= (char) EEPROM.read(currentEEPROMposition++);
		if(currentEEPROMposition==KNOWNWIFIEEPROMSTOP) return false;
	}
	if(ssid.compareTo("")!=0)currentEEPROMposition++;	//set it one further to read password
#if DEBUGACTIVE ==1
	Serial.println("Wifi read: " + ssid);
#endif
	if(currentEEPROMposition==KNOWNWIFIEEPROMSTOP) return false;
	//read Password
	while(EEPROM.read(currentEEPROMposition)){
		password+= (char) EEPROM.read(currentEEPROMposition++);
		if(currentEEPROMposition==KNOWNWIFIEEPROMSTOP) return false;
	}
	if((ssid.compareTo("")!=0))currentEEPROMposition++;	//set it one further to be able to read or write next element
#if DEBUGACTIVE ==1
	Serial.println("Password read: " + password);
#endif
	if(ssid.compareTo("")==0) return false;
	knownWiFI.setSSID(ssid);
	knownWiFI.setPassword(password);
	return true;
}

void ESP8266WiFiUserConnectionInterface::readAllEEPROM(){
	for(int i = KNOWNWIFIEEPROMSTART; i<KNOWNWIFIEEPROMSTOP; i++){
		Serial.print((char)EEPROM.read(i));
	}
}
