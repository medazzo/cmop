/**
 *  \copyright Copyright (c) 2011, 2012 All Right Reserved
 *  \brief this file contains defines constants and all needed includes.
 *  \file  HTTPUtility.h
 *  \date 11 juin 2013
 *  \author Azzouni Mohamed
*/

#ifndef UTILITY_H_
#define UTILITY_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <netdb.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*----------------------------------------------------------------------
|   constants
+---------------------------------------------------------------------*/
const char* const NPT_HTTP_DEFAULT_403_HTML = "<html><head><title>403 Forbidden</title></head><body><h1>Forbidden</h1><p>Access to this URL is forbidden.</p></html>";/*!< default HTTPServer response  in  case of 403 Error  */
const char* const NPT_HTTP_DEFAULT_404_HTML = "<html><head><title>404 Not Found</title></head><body><h1>Not Found</h1><p>The requested URL was not found on this server.</p></html>";/*!< default HTTPServer response  in  case of 404 Error  */
const char* const NPT_HTTP_DEFAULT_500_HTML = "<html><head><title>500 Internal Error</title></head><body><h1>Internal Error</h1><p>The server encountered an unexpected condition which prevented it from fulfilling the request.</p></html>";/*!< default HTTPServer response  in  case of 500 Error  */

#define HTTP_DEFAULT_MAX_THREADS_WORKERS	10										/*!< define default MAX Thread Workers  */
#define HTTP_SERVER_PORT_NUMBER 			8080									/*!< define default HTTP Serve Port     */
#define HTTP_SERVER_NAME_VERSION_NUMBER		"CMOP/V0.0.1"							/*!< define HTTP Server Version visible in Headers Response*/
#define APP_SERVER_NAME						"CMOP-SRV"								/*!< define HTTP Server Name visible in Headers Response*/
#define check_support(mask,flag)			(((mask & flag) == flag) ? true : false )/*!< define default MAX Thread Workers  */

#define CHECK_MY_RET(x)				{if(x!= NPT_SUCCESS)log_error("Returning ERROR %s ",NPT_ResultText(x));}
#define CHECK_ERROR_LOG(x,a, ...)	{if(x!= NPT_SUCCESS) log_error("%s:"a ,NPT_ResultText(x),##__VA_ARGS__);}
#define UNUSED(x)					(void)(x)

/**
 * \enum METHODS
 * \brief Contains the Value of the Http Method that can be supported by different types of handlers
*/
typedef enum {
SUPPORT_NONE 	= 0x0,		/*!< Do not support Any Methods. */
SUPPORT_PUT   	= 0x1000,	/*!< support for PUT Method. */
SUPPORT_GET   	= 0x0100,	/*!< support for GET Method. */
SUPPORT_POST   	= 0x0010,	/*!< support for POST Method. */
SUPPORT_DELETE  = 0x0001,	/*!< support for DELETE Method. */
SUPPORT_ALL     = SUPPORT_PUT|SUPPORT_GET|SUPPORT_POST|SUPPORT_DELETE	/*!< support for ALL Method. */
}METHODS;

/**
 * \enum HANDLERSTYPES
 * \brief Contains the Value of theHandlers Types : Static , Eventing or Asterisk .
*/
typedef enum  {
HADNLER_STATIC 		= 0x0,  	/*!< define Static Handler */
HANDLER_EVENT  		= 0x1000,   /*!< define the Eventing Handler. */
HANDLER_ASTERISK   	= 0x0100, 	/*!< define the Asterisk Handler. */
}HANDLERSTYPES;


#endif /* UTILITY_H_ */
