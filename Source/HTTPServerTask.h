/**
 *  \copyright Copyright (c) 2011, 2012 All Right Reserved
 *  \brief implementation de Http Server
 *  \file  HTTPServerTask.h
 *  \date 11 juin 2013
 *  \author Azzouni Mohamed
*/

#ifndef HTTPPFSTASK_H_
#define HTTPPFSTASK_H_

#include "HTTPUtility.h"
#include "HTTPServerTaskData.h"
#include "HTTPServer.h"

namespace cmop
{
/**
 * \class HTTPServerTask
 * \brief The server worker thread task .
*/
class HTTPServerTask: public NPT_Thread {
public:
	/**
	 * \brief   instantiate a server task thread worker.
	 * \param   input : Client request input stream
	 * \param   output : Client request output stream
	 * \param   context : Client request context
	 * \param   server : The HTTP server
	*/
	HTTPServerTask(NPT_InputStreamReference& input,
			NPT_OutputStreamReference& output, NPT_HttpRequestContext* context,
			HTTPServer *server);
	/**
	 * \brief default destructor
	*/
	~HTTPServerTask();

	/**
	 * \brief  Update task process data , nothing will be done if is already busy.
	 * \param   input : Client request input stream
	 * \param   output : Client request output stream
	 * \param   context : Client request context
	*/
	void setData(NPT_InputStreamReference& input,
			NPT_OutputStreamReference& output, NPT_HttpRequestContext* context);

	/**
	 * \brief  Ask the Task to back to work and process data , nothing will be done if is already busy.
	*/
	void backToWork();

	/**
	 * \brief  check if task has a job or not
	 * \return true if task is idle , has no new data to process .
	*/
	bool IsIdle();

	/**
	 * \brief  called to stop the task thread
	*/
	void Stop();

private:
	/**
	 * \brief  used to respond to client
	 * \return  NPT_SUCCES else Neptune code error .
	*/
	NPT_Result RespondToClient();

	/**
	 * \brief  Task Threaded function implementation
	*/
	virtual void Run();

	/** \brief  pointer on Http Server .*/
	cmop::HTTPServer * m_server;
	/** \brief  Client request input stream.*/
	NPT_InputStreamReference m_input;
	/** \brief  Client request ouput stream.*/
	NPT_OutputStreamReference m_output;
	/** \brief  Client request context */
	NPT_HttpRequestContext* m_context;
	/** \brief  shared Variable used to indicate busy status .*/
	NPT_SharedVariable * m_status;
	/** \brief  boolean variable to check if thread is running.*/
	bool m_running;
};
}
#endif /* HTTPPFSTASK_H_ */
