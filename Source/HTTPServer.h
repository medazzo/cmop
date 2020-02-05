/**
 *  \copyright Copyright (c) 2011, 2012 All Right Reserved
 *  \brief implementation de Http Server
 *  \file  HTTPServer.h
 *  \date 11 juin 2013
 *  \author Azzouni Mohamed
*/

#ifndef HTTPRESTSERVER_H_
#define HTTPRESTSERVER_H_

#include "Neptune.h"

#include "HTTPTree.h"
#include "HTTPServerTaskData.h"
#include "HTTPServerTask.h"

namespace cmop
{

class IHTTP_Handler;

/**
 * \class HTTPServer
 * \brief The server Class .
*/
class HTTPServer: public NPT_HttpServer, public NPT_Thread, public ICServer {
public:
	/**
	 * \brief   instantiate a server without any handler at port and using max threads workers.
	 * \param   listen_address  ip_adress to listen on for clients
	 * \param   listen_port  port to listen on for clients , default HTTP_SERVER_PORT_NUMBER
	 * \param   max_threads_workers : maximum numbers of thread workers : HTTP_DEFAULT_MAX_THREADS_WORKERS
	*/
	HTTPServer(	::NPT_IpAddress listen_address,
				::NPT_UInt16 listen_port,
				::NPT_UInt16 max_threads_workers);

	/**
	 * \brief destructor of Server
	*/
	~HTTPServer();

	/**
	 * \brief  Start call: used to Start Server
	 * \return  NPT_SUCCES else Neptune code error .
	*/
	Result StartServer();
	/**
	 * \brief  Stop call: used to Stop Server from listen to new clients.
	 * \return  NPT_SUCCES else Neptune code error .
	*/
	Result Stop();

	/**
	 * \brief  Called by to post waiting data to a task and ask him to process them
	 * \param  task the Http Task worker
	*/
	void 	ProcessClientData(HTTPServerTask * task );

	/**
	 * \brief  used to transfrom URL to a list of segment .
	 *      UrlPath has the format /root/a/b/c/d/g/
	*      will be transformed to a list {'root','a','b','c','d','g'}.
	 * \return  NPT_SUCCES else Neptune code error .
	*/
	NPT_Result CalculateQueryPath(::NPT_String UrlPath,
			::NPT_List<NPT_String>& path);

	/**
	 * \brief  Get Rest Tree Handler of the Server [could be NULL].
	 * \return pointer on HTTPTree class
	*/
	HTTPTree* getTreeHandler();

	/**
	 * \brief  Get the Server Shared Eventing Variable used to inform workers threads of eventing.
	 * \return pointer on NPT_SharedVariable class
	*/
	::NPT_SharedVariable* getSharedEventing();

	/**
	 * \brief Set the tree Handler to be used by the server.
	 * \param  treeHandler the  Tree pointer to be settled.
	*/
	Result setRoot(IHTTPHandler* treeHandler);

private:
	/**
	 * \brief  Stop all tasks associated with this task manager.
	 * \return  NPT_SUCCES else Neptune code error .
	*/
	Result StopAllTasks();

	/**
	 * \brief  Block  until new client connection is established
	 * 		and send its data to task manager to be executed.
	 * \return NPT_SUCCES else Neptune code error .
	*/
	Result GetNewClient();

	/**
	 * \brief   check if there is a idle task worker from the list
	 * \return true if there is an Idle worker else false .
	*/
	bool IsAWorkerIdle(HTTPServerTask* &task);

	/**
	 * \brief  Server Threaded function implementation
	*/
	void Run();

	/**
	 * \brief  used to know if the server loop is running (Listen to new clients).
	 * \return true if server is running else false .
	*/
	bool isLoop();

	/** \brief  Tree Handler pointer of the Server .*/
	HTTPTree * treeHandler;
	/** \brief  boolean variable used to check if sever is running (looping)*/
	bool m_loop;
	/** \brief  Mutex to lock/unlock access to m_loop variable.*/
	::NPT_Mutex m_LoopLock;
	/** \brief  List of Task manager Data waiting to be executed.*/
	::NPT_List<HTTPServerTaskData*> *m_DataTasksWaiting;
	 /** \brief  List of Task worker ready to execute or already executing some data .*/
	::NPT_List<HTTPServerTask*> *m_DataTasksWorkers;
	 /** \brief  Task manager List lock  .*/
	NPT_Mutex m_TasksLock;
	 /** \brief  contains Max allowed task number*/
	NPT_Cardinal m_MaxTasks;
};
}
#endif /* HTTPRESTSERVER_H_ */
