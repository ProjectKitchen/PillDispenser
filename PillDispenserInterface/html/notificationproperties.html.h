/*
 * notificationproperties.html.h
 *
 *  Created on: 20 Nov 2016
 *      Author: felix
 */

#ifndef NOTIFICATIONPROPERTIES_HTML_H_
#define NOTIFICATIONPROPERTIES_HTML_H_

#include "style.css.h"

/*

#define NOTIFICATIONPROPERTIES "<!DOCTYPE html> <html> <head> <title> Notification Properties </title> " STYLE " \
		<ul> \
			<li> <a href = '/' title='Connect to a Network'>Connect to a Network </a></li> \
			<li> <a href = '/notify' title='Set Notification properties'>Set Notification properties </a></li> \
			<li> <a href = '/dispens'>Set dispensing time events</a></li> \
		</ul> \
		<p><strong>Please make your changes here:</strong></p> \
		<form action='/notifypropsselected'> \
		<label for='emailto'>send notifications to:</label> \
		<input type='text' name='emailto' value='myemail@example.tut' /> \
		<br /> \n \
		<label for='subject'>subject:</label> \
		<input type='text' name='subject' value='Take Your Pill' /> \
		<br /> \
		<p><strong>this should not be changed if you don't know what you are doing:</strong></p> \
		<label for='selectSMTP'>select a SMTP service</label><select name='selectSMTP'><option>mail.smtp2go.com</option><option>other</option> \
		</select> \
		<br /> \n \
		<label for='emailfrom'>send from:</label> \
		<input type='text' name='emailfrom' value='pilldispensermdba@gmail.com' /> \
		<br /> \
		<label for='base64encodedusername'>base64encodedusername</label><input type='text' name='base64encodedusername' value='cGlsbGRpc3BlbnNlcm1kYmFAZ21haWwuY29t'> \
		<br> \n \
		<label for='base64encodedpassword'>base64encodedpassword</label><input type='text' name='base64encodedpassword' value='bWRiYXBpbGxkaXNwZW5zZXI='> \
		<br> \
		<input type='submit' name='Submit' value='Submit'> \
		</form> \
</head> </html>"


*/

#define NOTIFICATIONPROPERTIES1 "<!DOCTYPE html> <html> <head> <title> Notification Properties </title> " STYLE " \
	<body> \
		<ul> \
			<li> <a href = '/' title='Connect to a Network'>Connect to a Network </a></li> \
			<li> <a href = '/notify' title='Set Notification properties'>Set Notification properties </a></li> \
			<li> <a href = '/timesettings'>Set dispensing time events</a></li> \
		</ul> \
		<p><strong>Please make your changes here:</strong></p> \
		<form action=\"/notifypropsselected\"> \
        <label for=\"emailto\">send notifications to:</label> \
		<input type=\"text\" name=\"emailto\" value=\""
//email to send to
#define NOTIFICATIONPROPERTIES2 "\" /> \
		<br /> \
		<label for=\"subject\">subject:</label> \
		<input type=\"text\" name=\"subject\" value=\""
//subject
#define NOTIFICATIONPROPERTIES3 "\" /> \
		<br /> \
		<p><strong>this should not be changed if you don't know what you are doing:</strong></p> \
        <label for=\"emailfrom\">sending email:</label> \
		<input type=\"text\" name=\"emailfrom\" value=\""
//email from which is sent
#define NOTIFICATIONPROPERTIES4 "\" /> \
		<br /> \
        <label for=\"displayname\">Displayed Name:</label> \
		<input type=\"text\" name=\"displayname\" value=\""
//displayed name
#define NOTIFICATIONPROPERTIES5 "\" title=\"the name shown to the receiver\"/> \
        <br/> \
		<label for=\"selectSMTP\">SMTP service:</label><select name=\"selectSMTP\" title=\"SMTP service used for sending mails\"><option>"
//selected SMTP Host
#define NOTIFICATIONPROPERTIES6 "</option><option>other</option> \
		</select> \
		<br /> \
		<label for=\"username\">SMTP Username:</label><input type=\"text\" name=\"username\" value=\""
//smtpUserName
#define NOTIFICATIONPROPERTIES7 "\" title=\"Username at the SMTP service\"> \
		<br> \
		<label for=\"password\">SMTP Password</label><input type=\"text\" name=\"password\" value=\""
//smtppassword
#define NOTIFICATIONPROPERTIES8 "\"> \
		<br> \
		<input type=\"submit\" name=\"Submit\" value=\"Submit\"> \
		</form> \
	</body> \
</html>" 

#endif /* NOTIFICATIONPROPERTIES_HTML_H_ */
