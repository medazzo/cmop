/**
 *  \copyright Copyright (c) 2011, 2012 All Right Reserved
 *  \brief implementation de Http Server
 *  \file  HTTPServer.cpp
 *  \date 11 juin 2013
 *  \author Azzouni Mohamed
*/
#include "HTTPServer.h"
#include "HTTPUtility.h"
#include "HTTPServerTaskData.h"
#include "HTTPServerTask.h"
#include "HTTPTree.h"

HTTPServer::HTTPServer(NPT_IpAddress listen_address,NPT_UInt16 listen_port, NPT_UInt16 max_threads_workers) :
NPT_HttpServer(listen_address,listen_port) {

	treeHandler = NULL;
    m_MaxTasks = max_threads_workers;
	m_DataTasksWaiting = new NPT_List<HTTPServerTaskData*>();
	m_DataTasksWorkers = new NPT_List<HTTPServerTask*>();

	{
		NPT_AutoLock lock(m_LoopLock);
		m_loop = true;
	}
	log_info(">>>>>>>>>>>>>>>>>>>>");
}
HTTPServer::~HTTPServer() {
	log_debug("## destructing  ...  ");
	if (isLoop())
		this->Stop();

	if(treeHandler)
		delete treeHandler;

    for(NPT_Cardinal i = 0;i<m_DataTasksWorkers->GetItemCount();i++)
	{
		HTTPServerTask* t= NULL;
		m_DataTasksWorkers->Get(i, t);
		if(t)
		{
			log_warning("Task %ld is deleted !! ",t->GetCurrentThreadId());
			delete t;
		}
	}

    {
		NPT_AutoLock lock(m_TasksLock);
		m_DataTasksWaiting->Clear();
		delete m_DataTasksWaiting;
    }
    m_DataTasksWorkers->Clear();
    delete m_DataTasksWorkers;
}
bool
HTTPServer::isLoop() {
	{
		NPT_AutoLock lock(m_LoopLock);
		return m_loop;
	}
}
HTTPTree*
HTTPServer::getTreeHandler() {
	return treeHandler;
}
void
HTTPServer::setTreeHandler(HTTPTree*& treeHandler) {
	this->treeHandler = treeHandler;
}
NPT_Result
HTTPServer::CalculateQueryPath(NPT_String UrlPath,
		NPT_List<NPT_String>& path) {
	if (UrlPath.StartsWith("/")) {
		path = UrlPath.Right(UrlPath.GetLength() - 1).Split("/");
	} else {
		path = UrlPath.Split("/");
		log_info("%s >> %d ", UrlPath.GetChars(), path.GetItemCount());
	}
	log_info(" %s >> %d ", UrlPath.GetChars(), path.GetItemCount());
	return NPT_SUCCESS;
}

void
HTTPServer::Run() {
	NPT_Result result = NPT_FAILURE;
	do {
		log_info("Test HTTP server waiting for connection ...");
		result = GetNewClient();
		log_info("GetNewClient returned %d (%s)", result,
				NPT_ResultText(result));
	} while (isLoop());
}
NPT_Result
HTTPServer::Stop() {
	{
		NPT_AutoLock lock(m_LoopLock);
		m_loop = false;
		log_warning("Test HTTP server Stopped ...");
	}
	Abort();
	this->StopAllTasks();
	return NPT_SUCCESS;
}
void
HTTPServer::ProcessClientData(HTTPServerTask * task )
{
	log_debug("will extract a new TaskData !");
	HTTPServerTaskData * taskData  = NULL ;
	{
		NPT_AutoLock lock(m_TasksLock);
		this->m_DataTasksWaiting->PopHead(taskData);
	}
	if(taskData != NULL)
	{
		log_debug("Got the new popped TaskData !");
		if(task != NULL)
		{
			log_debug("Will use an old idle task Worker  [%d/%d]!",this->m_DataTasksWorkers->GetItemCount(),m_MaxTasks);
			task->setData(taskData->m_input,taskData->m_output,taskData->m_context);
			task->backToWork();
		}
		else if (this->m_DataTasksWorkers->GetItemCount() < m_MaxTasks)
		{
			log_debug("Will create a new task Worker  !");
			HTTPServerTask * tt = new HTTPServerTask(taskData->m_input,taskData->m_output,taskData->m_context,this);
			this->m_DataTasksWorkers->Add(tt);
			tt->Start();
		}
		else
		{
			log_error("Very Bizarre Error lost TaskData  : will not be executed ");
		}
		//cleaning
		delete taskData ;
	}
	else
	{
		log_debug("No Waiting TaskData  to be executed ");
	}
}
NPT_Result
HTTPServer::GetNewClient() {
	NPT_InputStreamReference input;
	NPT_OutputStreamReference output;
	NPT_HttpRequestContext context;

	// Wait to get a new Client
	NPT_Result result = WaitForNewClient(input, output, &context);
	log_error("HAVE A NEW CLIENT !! :WaitForNewClient returned %d (%s)", result,
			NPT_ResultText(result));
	if (NPT_FAILED(result))
		return result;

	{
		NPT_AutoLock lock(m_TasksLock);
		result = this->m_DataTasksWaiting->Add(new HTTPServerTaskData(input,output,&context));
		log_info("PushData  returned %d (%s)", result, NPT_ResultText(result));
	}

	log_debug("A new Data has arrived to be executed  [%s ]",NPT_ResultText(result));
	// check if we can pop data , else it will be lost
	HTTPServerTask * task = NULL;
	bool idle = IsAWorkerIdle(task);
	if((idle == true) || (this->m_DataTasksWorkers->GetItemCount() < m_MaxTasks) )
	{
		ProcessClientData(task );
	}
	else
	{
		log_warning("we cannot Launch a new Task , have to wait");
	}

	return result;
}
bool
HTTPServer::IsAWorkerIdle(HTTPServerTask* &task) {
	log_info("Cheking if there is an Idle task from %d ones ",m_DataTasksWorkers->GetItemCount());
	int count = 0;
	for(NPT_Cardinal i = 0;i<m_DataTasksWorkers->GetItemCount();i++)
	{
		count++;
		HTTPServerTask* t= NULL;
		m_DataTasksWorkers->Get(i, t);
		if(t)
		{
			log_info("on  %d/%d :%ld: IsIdle>> %d  ",count,m_DataTasksWorkers->GetItemCount(),t->GetCurrentThreadId(),t->IsIdle());
			if( t->IsIdle() == true )
			{
				task = t;
				return true;
			}
		}
	}
	return false;
}
NPT_Result
HTTPServer::StopAllTasks()
{
	{
		NPT_AutoLock lock(m_TasksLock);
		m_DataTasksWaiting->Clear();
    }

    for(NPT_Cardinal i = 0;i<m_DataTasksWorkers->GetItemCount();i++)
	{
		HTTPServerTask* t= NULL;
		m_DataTasksWorkers->Get(i, t);
		if(t)
		{
			log_warning("Task %ld is Stopped !! ",t->GetCurrentThreadId());
			t->Stop();
		}
	}
    log_warning(">> All task are Stopped !! ");
    return NPT_SUCCESS;
}
