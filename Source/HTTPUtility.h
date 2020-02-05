/**
 *  \copyright Copyright (c) 2011, 2012 All Right Reserved
 *  \brief this file contains defines constants and all needed includes.
 *  \file  HTTPUtility.h
 *  \date 11 juin 2013
 *  \author Azzouni Mohamed
 */

#ifndef UTILITY_H_
#define UTILITY_H_

#include "CMopServer.h"
#include "Neptune.h"

namespace cmop {

#define UNUSED(x)	 (void)(x)

/**
 * \class HTTPServerTaskData
 * \brief The server Task Manager Data Class .
 */
class HTTPUtility {

 public:
  HTTPUtility();
  virtual ~HTTPUtility();
  /*----------------------------------------------------------------------
   |   constants
   +---------------------------------------------------------------------*/
  /*!< define default MAX Thread Workers  */
  static const int HTTP_DEFAULT_MAX_THREADS_WORKERS;
  /*!< define default HTTP Serve Port     */
  static const int HTTP_SERVER_PORT_NUMBER;
  /*!< define HTTP Server Version visible in Headers Response*/
  static const char *HTTP_SERVER_NAME_VERSION_NUMBER;
  /*!< define HTTP Server Name visible in Headers Response*/
  static const char *APP_SERVER_NAME;
  /*!< default HTTPServer response  in  case of 403 Error  */
  static const char *HTTP_DEFAULT_403_HTML;
  /*!< default HTTPServer response  in  case of 404 Error  */
  static const char *HTTP_DEFAULT_404_HTML;
  /*!< default HTTPServer response  in  case of 500 Error  */
  static const char *HTTP_DEFAULT_500_HTML;

  static inline bool check_support(int mask, int flag) {
    return (((mask & flag) == flag) ? true : false);
  }
  static Result MapNPTResult(NPT_Result result);
};
}
#endif /* UTILITY_H_ */
