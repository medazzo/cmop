/**
 *  \copyright Copyright (c) 2011, 2012 All Right Reserved
 *  \brief definition of class HTTPServerTaskData
 *  \file  HTTPServerTaskData.h
 *  \date 11 juin 2013
 *  \author Azzouni Mohamed
*/

#ifndef HTTPTASKDATA_H_
#define HTTPTASKDATA_H_

#include "Neptune.h"
#include "HTTPUtility.h"
namespace cmop
{
/**
 * \class HTTPServerTaskData
 * \brief The server Task Manager Data Class .
*/
class HTTPServerTaskData {
public:
	/**
	 * \brief   instantiate a server Task Manager Data .
	 * \param   input  Client request input stream
	 * \param   output  Client request output stream
	 * \param   context  Client  context
	*/
	HTTPServerTaskData(NPT_InputStreamReference& input,
			NPT_OutputStreamReference& output,
			NPT_HttpRequestContext* context) :
				m_input(input), m_output(output), m_context(context),m_request(NULL) {
	};

	/**
	 * \brief   instantiate a server Task Manager Data .
	 * \param   input  Client request input stream
	 * \param   output  Client request output stream
	 * \param   context  Client  context.
	 * \param   request  Client request
	*/
	HTTPServerTaskData(NPT_InputStreamReference& input,
				NPT_OutputStreamReference& output,
				NPT_HttpRequestContext* context,
				NPT_HttpRequest* request) :
					m_input(input), m_output(output), m_context(context),m_request(request) {
		};

	/** \brief  m_input  Client request input stream */
	NPT_InputStreamReference m_input;
	/** \brief  m_output  Client request output stream */
	NPT_OutputStreamReference m_output;
	/** \brief  m_context  Client  context */
	NPT_HttpRequestContext* m_context;
	/** \brief  m_request  Client request  */
	NPT_HttpRequest* m_request;
};
}
#endif /* HTTPTASKDATA_H_ */
