/*
 * waitWhileConnecting.html.h
 *
 *  Created on: 17 Nov 2016
 *      Author: felix
 */

#ifndef WAITWHILECONNECTING_HTML_H_
#define WAITWHILECONNECTING_HTML_H_

#define WAITWHILECONNECTINGSTART "<!DOCTYPE html><html><head><meta http-equiv=\"refresh\" content=\"4\"><style>"
//#style
#define WAITWHILECONNECTINGINTERMEDIATE ".loader {border: 16px solid #f3f3f3;border-radius: 50%;border-top: 16px solid #3498db;width: 120px;height: 120px;-webkit-animation: spin 2s linear infinite;animation: spin 2s linear infinite; "
//#spinnercolor
#define WAITWHILECONNECTINGEND "}@-webkit-keyframes spin {0% { -webkit-transform: rotate(0deg); }100% { -webkit-transform: rotate(360deg); }}@keyframes spin {  0% { transform: rotate(0deg); }  100% { transform: rotate(360deg); }}</style></head><body><h2>Please wait while connecting to the Network</h2><div class=\"loader\"></div></body></html>"




#endif /* WAITWHILECONNECTING_HTML_H_ */
