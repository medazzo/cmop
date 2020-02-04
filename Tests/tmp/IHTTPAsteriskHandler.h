/**
 *  \copyright Copyright (c) 2011, 2012 All Right Reserved
 *  \brief implementation of the Asterisk Http Handler
 *  \file  IHTTPAsteriskHandler.h
 *  \date 11 juin 2013
 *  \author Azzouni Mohamed
*/

#ifndef IHTTPASTERISKHANDLER_H_
#define IHTTPASTERISKHANDLER_H_

#include "IHTTPHandler.h"

namespace cmop
{
/**
 * \class IHTTPAsteriskHandler
 * \brief The interface of Asterisk HTTP Handler .
*/
class IHTTPAsteriskHandler  : public IHTTPHandler {
	public:
	/**
	 * \brief   instantiate a static IHTTPAsteriskHandler Handler.
	 * \param   methodsSupportMask The mask containing supported Http Method
	*/
	IHTTPAsteriskHandler(METHODS methodsSupportMask = SUPPORT_NONE);
	/**
	 *  \brief destructor
	 */
	~IHTTPAsteriskHandler();
	/**
	 * \brief   Get the Handler Type.
	 * \return  the Handler Type :HANDLER_ASTERISK.
	*/
	virtual HANDLERSTYPES GetMyHandlerType(){return HANDLER_ASTERISK;};

	/**
	 * \brief  operator used to  compare handler segment with string
	 * \param   other : string to be compared with Handler segment.
	 * \return  NPT_SUCCES else Neptune code error .
	*/
	virtual bool operator==(const NPT_String& other);

	/**
	 * \brief  this function is called when a PUT request is arrived and when PUT is enabled
	 * :: for Eventing type Handler this function is called whe nthe event is sent .
	 * \param   request : the client query
	 * \param   context : the request http context.
	 * \param   response : the response
	 * \return  NPT_SUCCES else Neptune code error .
	*/
	virtual void OnCreate(NPT_HttpRequest& request,
			const NPT_HttpRequestContext& context,
			NPT_HttpResponse& response) = 0;

	/**
	 * \brief  this function is called when a GET request is arrived and when GET is enabled .
	 * :: for Eventing type Handler this function is called whe nthe event is sent .
	 * \param   request : the client query
	 * \param   context : the request http context.
	 * \param   response : the response
	 * \return  NPT_SUCCES else Neptune code error .
	*/
	virtual void OnRead(NPT_HttpRequest& request,
			const NPT_HttpRequestContext& context,
			NPT_HttpResponse& response) = 0;

	/**
	 * \brief  this function is called when a POST request is arrived and when POST is enabled .
	 * :: for Eventing type Handler this function is called whe nthe event is sent .
	 * \param   request : the client query
	 * \param   context : the request http context.
	 * \param   response : the response
	 * \return  NPT_SUCCES else Neptune code error .
	*/
	virtual void OnUpdate(NPT_HttpRequest& request,
			const NPT_HttpRequestContext& context,
			NPT_HttpResponse& response) = 0;

	/**
	 * \brief  this function is called when a DELETE request is arrived and when DELETE is enabled .
	 * :: for Eventing type Handler this function is called whe nthe event is sent .
	 * \param   request : the client query
	 * \param   context : the request http context.
	 * \param   response : the response
	 * \return  NPT_SUCCES else Neptune code error .
	*/
	virtual void OnDelete(NPT_HttpRequest& request,
			const NPT_HttpRequestContext& context,
			NPT_HttpResponse& response) = 0;

	/**
	 * \brief  this function Return the string in the URL representing the '*'  .
	 * \return  NPT_String : the Asterisk Delta found in the URL
	*/
	NPT_String &getDelta();

protected:
	/** \brief the Asterisk Delta found in the URL. */
	NPT_String m_delta;
	/** \brief the Handler segment. */
	NPT_String m_segment;
	/** \brief  True when HTTP Method POSt is enabled .. */
	METHODS m_methodsSupportMask;
};
}
#endif /* IHTTPASTERISKHANDLER_H_ */
