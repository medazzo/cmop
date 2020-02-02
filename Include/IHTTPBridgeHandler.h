/**
 *  \copyright Copyright (c) 2011, 2012 All Right Reserved
 *  \brief implementation of the Bridge Http Handler
 *  \file  IHTTPBridgeHandler.h
 *  \date 11 juin 2013
 *  \author Azzouni Mohamed
*/

#ifndef IHTTPBRIDGEHANDLER_H_
#define IHTTPBRIDGEHANDLER_H_

#include "IHTTPHandler.h"


/**
 * \class IHTTPBridgeHandler
 * \brief The interface of a bridge HTTP Handler .
*/
class IHTTPBridgeHandler: public IHTTPHandler {
public:
	/**
	 * \brief   instantiate a static HTTP Handler.
	 * \param   segment  The String segment responsible for this handler .
	 * \param   methodsSupportMask The mask containing supported Http Method
	*/
	IHTTPBridgeHandler(const char* segment):IHTTPHandler(segment,SUPPORT_NONE){};
	/**
	 *  \brief destructor
	 */
	~IHTTPBridgeHandler(){};

public:
	/**
	 * \brief  this function is called when a PUT request is arrived and when PUT is enabled
	 * :: for Eventing type Handler this function is called whe nthe event is sent .
	 * \param   request : the client query
	 * \param   context : the request http context.
	 * \param   response : the response
	 * \return  NPT_SUCCES else Neptune code error .
	*/
	void OnCreate(NPT_HttpRequest& request,
			const NPT_HttpRequestContext& context,
			NPT_HttpResponse& response) {/* NOT Supported */};

	/**
	 * \brief  this function is called when a GET request is arrived and when GET is enabled .
	 * :: for Eventing type Handler this function is called whe nthe event is sent .
	 * \param   request : the client query
	 * \param   context : the request http context.
	 * \param   response : the response
	 * \return  NPT_SUCCES else Neptune code error .
	*/
	void OnRead(NPT_HttpRequest& request,
			const NPT_HttpRequestContext& context,
			NPT_HttpResponse& response)  {/* NOT Supported */};

	/**
	 * \brief  this function is called when a POST request is arrived and when POST is enabled .
	 * :: for Eventing type Handler this function is called whe nthe event is sent .
	 * \param   request : the client query
	 * \param   context : the request http context.
	 * \param   response : the response
	 * \return  NPT_SUCCES else Neptune code error .
	*/
	void OnUpdate(NPT_HttpRequest& request,
			const NPT_HttpRequestContext& context,
			NPT_HttpResponse& response)  {/* NOT Supported */};

	/**
	 * \brief  this function is called when a DELETE request is arrived and when DELETE is enabled .
	 * :: for Eventing type Handler this function is called whe nthe event is sent .
	 * \param   request : the client query
	 * \param   context : the request http context.
	 * \param   response : the response
	 * \return  NPT_SUCCES else Neptune code error .
	*/
	void OnDelete(NPT_HttpRequest& request,
			const NPT_HttpRequestContext& context,
			NPT_HttpResponse& response)  {/* NOT Supported */};
};

#endif /* IHTTPBRIDGEHANDLER_H_ */
