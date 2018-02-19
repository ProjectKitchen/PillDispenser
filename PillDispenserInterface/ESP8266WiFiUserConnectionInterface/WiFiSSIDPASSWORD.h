/*
 * WiFiSSIDPASSWORD.h
 *
 *  Created on: 25 Nov 2016
 *      Author: felix
 */

#ifndef WIFISSIDPASSWORD_H_
#define WIFISSIDPASSWORD_H_


class WiFiSSIDPASSWORD{
public:
	const String& getPassword() const {
		return password;
	}

	void setPassword(const String& password) {
		this->password = password;
	}

	const String& getSSID() const {
		return ssid;
	}

	void setSSID(const String& ssid) {
		this->ssid = ssid;
	}

private:
	String ssid ="";
	String password ="";
};


#endif /* WIFISSIDPASSWORD_H_ */
