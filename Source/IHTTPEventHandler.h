/**
 *  \copyright Copyright (c) 2011, 2012 All Right Reserved
 *  \brief implementation of the Eventing Http Handler
 *  \file  IHTTPEventHandler.h
 *  \date 11 juin 2013
 *  \author Azzouni Mohamed
*/

#ifndef IHTTPEVENTHANDLER_H_
#define IHTTPEVENTHANDLER_H_

#include "IHTTPHandler.h"

namespace cmop
{
/**
 * \class IHTTPEventHandler
 * \brief The interface of Eventing HTTP Handler .
*/
class IHTTPEventHandler: public cmop::IHTTPHandler {
public:
	/**
	 * \brief   instantiate a static HTTP Handler.
	 * \param   segment  The String segment responsible for this handler .
	 * \param   methodsSupportMask The mask containing supported Http Method
	*/
	IHTTPEventHandler(const char* segment, METHODS methodsSupportMask = SUPPORT_NONE);
	/**
	 *  \brief destructor
	 */
	~IHTTPEventHandler();
private:
	/** \brief  the shared variable used to block the wait function until value changes (event occur).*/
	NPT_List<HTTPServerTaskData*> *m_WaitingClient;
	/** \brief  Used to check if an Event is Armed.*/
	bool m_sendnAnEvent;
public:

	/**
	 * \brief   Get the Handler Type.
	 * \return  the Handler Type :HANDLER_EVENT.
	*/
	virtual HANDLERSTYPES GetMyHandlerType(){return HANDLER_EVENT;};
	/**
	 * \brief Send Event to all  blocked client .
	*/
	void NotifyWaitingClients();

	/**
	 * \brief Arm again the eventing mechanism to block new coming clients.
	*/
	void ArmEventToBlockNewClients();

	/**
	 * \brief  this function Will push a client into the Queue
	 * \param   m_input the  client input
	 * \param   m_output the client output
	 * \param   m_context the request http context.
	 * \param   request  the request of the client
	 * \return  NPT_SUCCES  means client is pushing in Queue waiting for event to get response ,
	 * 			NPT_FAILEd: means client is not pushed (because event is already triggred ) and then have to send response immediately
	 */
	NPT_Result PushWaitingclient(NPT_InputStreamReference m_input,
			NPT_OutputStreamReference m_output,NPT_HttpRequestContext* m_context,NPT_HttpRequest* request);

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
};
}
#endif /* IHTTPEVENTHANDLER_H_ */
