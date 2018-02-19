/*
 * index.html.h
 *
 *  Created on: 20 Nov 2016
 *      Author: felix
 */

#ifndef INDEX_HTML_H_
#define INDEX_HTML_H_
//TODO: replace with style.css.h STYLE
#define INDEXSTART "<!DOCTYPE html> \
<html> \
	<head> \
		<title>Pill Dispenser</title> \
		<style> \
			body{ \
				background-color: #ccccb3; \
			} \
			ul{ \
				list-style-type: none; \
				margin: 0; \
				padding: 0; \
				overflow: hidden; \
				background-color: #444; \
			} \n \
			li{ \
				float: left; \
			} \
			li a{ \
				display: block; \
				color: white; \
				text-align: center; \
				padding: 14px 16px; \
				text-decoration: none; \
			} \
			li a:hover{ \
				background-color: #000; \
			} \
			li a:active{ \
				background-color: green; \
			} \
		</style> \
	</head>"

#define INDEXBODY	"<body> \
		<ul> \
			<li class='navbarelement'> <a href = '/' title='Connect to a Network'>Connect to a Network </a></li> \
			<li> <a href = '/notify' title='Set Notification properties'>Set Notification properties </a></li> \
			<li> <a href = '/timesettings'>Set dispensing time events</a></li> \
		</ul>"

#define INDEXEND	"</body> \
</html>"



#endif /* INDEX_HTML_H_ */
