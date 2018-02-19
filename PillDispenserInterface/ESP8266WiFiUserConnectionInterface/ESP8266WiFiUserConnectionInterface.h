/*
 * ESP8266WiFiConnectioInterface.h
 *
 *  Created on: 18 Nov 2016
 *      Author: felix mitterer
 *      e-mail: felix.mitterer@technikum-wien.at
 */

#ifndef ESP8266WIFIUSERCONNECTIONINTERFACE_ESP8266WIFIUSERCONNECTIONINTERFACE_H_
#define ESP8266WIFIUSERCONNECTIONINTERFACE_ESP8266WIFIUSERCONNECTIONINTERFACE_H_
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include "waitWhileConnecting.html.h"
#include "WiFiSSIDPASSWORD.h"
#include <EEPROM.h>

#define DEBUGACTIVE 1
#define UCI_DEFAULTCONNECTINGTIMEOUT 20

	//EEPROM Settings:
#define KNOWNWIFIEEPROMSTART 0
#define KNOWNWIFIEEPROMSTOP 200
#define KNOWNWIFIINTERMEDIATECHAR 0	//this is the character that is saved between ssid & password & next ssid
#define DELETENETWORKSONSTARTUP 0

class ESP8266WiFiUserConnectionInterface{
	ESP8266WebServer* server;

	DNSServer* dnsserver;
	uint16_t dnsport = 53;
	//replace dns with either a Location like: www.mylocation.net
	//or to forward every request to the ESPUserInterface use *
	String dns = "*";

	String apSSID = "ESPUserInterface";
	String apPassword = "";
	IPAddress apIPAddress = IPAddress(192, 168, 0, 1);
	IPAddress apDefaultGateway = IPAddress(192, 168, 0, 1);
	IPAddress apSubNetmask = IPAddress(255, 255, 255, 0);

	String defaultURI = "/ESPUserConnectionInterface";
	String webPage = "";
	String pageTitle = "ESPUserConnectionInterface";

	//to connect to the preferred Network:
	String ssid ="";
	String password ="";
	int connectingTimeout = UCI_DEFAULTCONNECTINGTIMEOUT;
	String connectedResponse = "Connected";

	//style settings for the spinner:
	String styleAtributes= "body{background-color: #ccccb3;}";
	String spinnerStyle = "border-top: 16px solid #333";

	//variable for Known WiFi reading
	int currentEEPROMposition = KNOWNWIFIEEPROMSTART;


	//private methods:

	String composeWebSiteForDropDownMenue(String dropDownMenueElements);
	String generateDropDownOptions();
	String connectionStatusHTML();
	void startUserConnectionInterface();
	void setup();
	void connectToNetwork();
	String composeWaitWhileConnecting();

	//-------------------------------------------------------------------------------------//
	//Now comes methods for saving known WiFi's and reading known WiFis
	bool getKnownSSIDs(WiFiSSIDPASSWORD& save);
	bool setKnownSSID(WiFiSSIDPASSWORD save);
	//bool deleteKnownSSIDs(); // this one is public, so that the user can delete the known Wifis
	void readAllEEPROM();

	public:
		ESP8266WiFiUserConnectionInterface();

		/*
		 * use this Constructor if you already have a server initialized
		 * @param: the server that you are using
		 */
		ESP8266WiFiUserConnectionInterface(ESP8266WebServer server);

		/*
		 * use this Constructor if you already have a server initialized and want a certain uri
		 * @param: the server that you are using
		 * @param: the uri, the Interface listens on
		 */
		ESP8266WiFiUserConnectionInterface(ESP8266WebServer server, String uri);


		//the same Constructors, but with DNSServer:
		ESP8266WiFiUserConnectionInterface(DNSServer dnsserver);

		/*
		 * use this Constructor if you already have a server initialized
		 * @param: the server that you are using
		 */
		ESP8266WiFiUserConnectionInterface(ESP8266WebServer server, DNSServer dnsserver);

		/*
		 * use this Constructor if you already have a server initialized and want a certain uri
		 * @param: the server that you are using
		 * @param: the uri, the Interface listens on
		 */
		ESP8266WiFiUserConnectionInterface(ESP8266WebServer server, String uri, DNSServer dnsserver);

		/*
		 * starts the UserConnectionInterface
		 * in precise, it scans for available networks and compares it with known ones
		 */
		void start();

		/*
		 * after initializing and setting up the ESP8266WiFiUserConnectionInterface
		 * call this run method from within your main loop
		 */
		void run();

		/*
		 * Set the default URI for the Interface
		 * CAUTION: if this URI already exists on the Webserver, the existent will NOT
		 * be overwritten or replaced by the new one
		 */
		void setURI(String uri);
		void setURI(const char* uri);

		/*
		 * manually add webpages to the server
		 * use this method exactly like the ESP8266WebServer.on(); method
		 */
		void serverAdd(const char* uri, ESP8266WebServer::THandlerFunction handler);

		ESP8266WebServer* getWebServer();

		/*
		 * configure the AP mode. Set a different SSID and password than the default
		 */
		void configAP(String ssid, String password);

		/*
		 * set the website that should appear when connected to a Network
		 * @param: this should be valid html
		 */
		void setWebsiteWhenConnected(String website);

		/*
		 * returns true if connected to a Network
		 */
		bool isConnected();

		/*
		 * returns the Form that is used for connecting, so that it can be embedded into a website
		 * the returned string is valid html format, that can be embedded into the <body></body> tag
		 */
		String getConnectionForm();

		/*
		 * this method is used to specify css styles for the waitWhileConnecting site
		 * @param The string handed over should be vaild css
		 * @param sets the color of the spinning circle (also needs to be valid css)
		 */
		void setStyle(String style , String spinnercolor);

		bool deleteKnownSSIDs();

};


#endif /* ESP8266WIFICONNECTIOINTERFACE_ESP8266WIFIUSERCONNECTIOINTERFACE_H_ */
