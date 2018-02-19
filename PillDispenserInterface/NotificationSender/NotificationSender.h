/*
 * NotificationSender.h
 *
 *  Created on: 21 Nov 2016
 *      Author: felix
 */

#ifndef NOTIFICATIONSENDER_H_
#define NOTIFICATIONSENDER_H_

#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <EEPROM.h>

#define DEBUGACTIVE 1
class NotificationSender{

	String sendingE_mail = "pilldispensermdba@gmail.com";
	String sendingName = "PillDispenser"; //the name that will be displayed on receiver side
	String userClear = "pilldispensermdba@gmail.com";
	String userBase64 = "cGlsbGRpc3BlbnNlcm1kYmFAZ21haWwuY29t";
	String smtpPasswordClear = "mdbapilldispenser";
	String smtpPasswordBas64 = "bWRiYXBpbGxkaXNwZW5zZXI=";
	String subject = "Take Your Pill";
	String smtpHost = "mail.smtp2go.com";
	int smtpPort = 2525;
	String receivingE_mail = "felix.mitterer@technikum-wien.at";
	WiFiClient client;


	void efail(); //this method is taken from an other example code: //Adapted from SurferTim's Ethernet email program by Ty Tower May 2015
	byte eRcv();  //this method is taken from an other example code: //Adapted from SurferTim's Ethernet email program by Ty Tower May 2015


	public:

		/*
		 * Costructor
		 * @param: EEPROM start address
		 * @param: EEPROM end address
		 */
		NotificationSender(int start, int end);

		/*
		 * sets another than the default sending e-mail
		 */
		void setSendingE_mail(String email);

		String getSendingE_mail();

		/*
		 * sets another than default sending name
		 * the sending name is the DISPLAYED name on receiving side
		 * e.g Ollaf insted of Ollaf@example.org
		 */
		void setSendingName(String name);

		String getSendingName();

		/*
		 * Sets the Username for the SMTP2Go Host that is Used
		 * also generates a base64 encoded version, which will be transmitted to the Host
		 */
		void setUserForSMTPHost(String user);

		String getUserForSMTPHost();

		/*
		 * Sets the Password for the used SMTP2Go host
		 * this also generates a base64 encoded version of the password, which is used for transmission
		 */
		void setPasswordForSMTPHost(String pw);

		String getPasswordForSMTPHost();

		/*
		 * sets another than the default receiving e-mail
		 */
		void setReceivingE_mail(String);

		String getReceivingE_mail();

		/*
		 * sets another than the default subject for sending the mail
		 */
		void setSendingSubject(String subject);

		String getSendingSubject();

		/*
		 * sets the SMTP2Go service to use
		 * @param: by default this is mail.smtp2go.com
		 * @param: the default port is 2525
		 */
		void setSMTPHost(String host, int port);

		String getSMTPHost();

		/*
		 * sends a message to the configured email over the configured smtp2go
		 * service and the configured sending email.
		 * @return true if success false if not success (e.g wrong config or no network connection)
		 */
		bool sendE_mail(String message);

		/*
		 * saves the set Parameters to EEPROM
		 * @param: StartAdress of EEPROM
		 * @param: EndAdress of EEPROM
		 */
		bool saveParams(int start, int end);
		bool deleteParams(int start, int end);
		bool restoreParams(int start, int end);
};


#endif /* NOTIFICATIONSENDER_H_ */
