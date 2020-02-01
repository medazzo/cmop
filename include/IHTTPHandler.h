/**
 *  \copyright Copyright (c) 2011, 2012 All Right Reserved
 *  \brief implementation de Http Server Handlers interface
 *  \file  IHTTPHandler.h
 *  \date 11 juin 2013
 *  \author Azzouni Mohamed
*/
#ifndef IHTTPHANDLER_H_
#define IHTTPHANDLER_H_

#include "HTTPUtility.h"
#include "HTTPServerTaskData.h"

/**
 * \class IHTTPHandler
 * \brief The interface of static HTTP Handler .
*/
class IHTTPHandler: public NPT_HttpRequestHandler {

public:
	/**
	 * \brief   instantiate a static HTTP Handler.
	 * \param   segment : The String segment responsible for this handler .
	 * \param   methodsSupportMask The mask containing supported Http Method
	*/
	IHTTPHandler(const char* segment, METHODS methodsSupportMask = SUPPORT_NONE);

	/**
	 * \brief destructor of IHTTP Handler
	*/
	virtual ~IHTTPHandler();

	/**
	 * \brief  used to set the Response : see  NPT_HttpRequestHandler::SetupResponse
	 * \param   request : the client query
	 * \param   context : the request http context.
	 * \param   response : the response
	 * \return  NPT_SUCCES else Neptune code error .
	*/
	NPT_Result SetupResponse(NPT_HttpRequest& request,
			const NPT_HttpRequestContext& context, NPT_HttpResponse& response);

	/**
	 * \brief  used to send the body response : see  NPT_HttpRequestHandler::SendResponseBody
	 * \param   output : the response output stream
	 * \param   context : the request http context.
	 * \param   response : the response
	 * \return  NPT_SUCCES else Neptune code error .
	*/
	NPT_Result SendResponseBody(const NPT_HttpRequestContext& context,
			NPT_HttpResponse& response, NPT_OutputStream& output);

	/**
	 * \brief  operator used to  compare handler segment with string
	 * \param   other : string to be compared with Handler segment.
	 * \return  NPT_SUCCES else Neptune code error .
	*/
	virtual bool operator==(const NPT_String& other);

	/**
	 * \brief get the Handler segment.
	 * \return  NPT_String segment of the handler.
	*/
	virtual NPT_String& getSegment();

	/**
	 * \brief  used to serve file to the client
	 * \param   request : the client query
	 * \param   context : the request http context.
	 * \param   response : the response
	 * \param   file_path: the file path to be served
	 * \param   mime_type: the file mime_type
	 * \return  NPT_SUCCES else Neptune code error .
	*/
	NPT_Result ServeFile(const NPT_HttpRequest&        request,
	                          const NPT_HttpRequestContext& context,
	                          NPT_HttpResponse&             response,
	                          NPT_String                    file_path,
	                          const char*  						 mime_type);

	/**
	 * \brief  used to get the header If-Modified-Since
	 * \param   message : the message to check
	 * \param   date : the date to be filled with value
	 * \return  NPT_SUCCES else Neptune code error .
	*/
	static NPT_Result GetIfModifiedSince(const NPT_HttpMessage& message,
	                                     NPT_DateTime&          date);

	/**
	 * \brief   Get the Handler Type.
	 * \return  the Handler Type :HADNLER_STATIC.
	*/
	virtual HANDLERSTYPES GetMyHandlerType(){return HADNLER_STATIC;};
protected:
	/** \brief the Handler segment. */
	NPT_String m_segment;
	/** \brief  True when HTTP Method POSt is enabled .. */
	METHODS m_methodsSupportMask;

protected:

	/**
	 * \brief  this function is called when a PUT request is arrived and when PUT is enabled .
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
	 * \param   request : the client query
	 * \param   context : the request http context.
	 * \param   response : the response
	 * \return  NPT_SUCCES else Neptune code error .
	*/
	virtual void OnDelete(NPT_HttpRequest& request,
			const NPT_HttpRequestContext& context,
			NPT_HttpResponse& response) = 0;
};


#endif /* IHTTPHANDLER_H_ */
