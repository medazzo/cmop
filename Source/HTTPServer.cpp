/**
 *  \copyright Copyright (c) 2011, 2012 All Right Reserved
 *  \brief implementation de Http Server
 *  \file  HTTPServer.cpp
 *  \date 11 juin 2013
 *  \author Azzouni Mohamed
*/
#include "CMopServer.h"
#include "HTTPServer.h"
#include "HTTPUtility.h"
#include "HTTPServerTaskData.h"
#include "HTTPServerTask.h"


#include "Neptune.h"
NPT_SET_LOCAL_LOGGER("cmop.server")

namespace cmop
{

/*----------------------------------------------------------------------
|   HTTPServer::HTTPServer
+---------------------------------------------------------------------*/
HTTPServer::HTTPServer(NPT_IpAddress listen_address,NPT_UInt16 listen_port, NPT_UInt16 max_threads_workers) :
NPT_HttpServer(listen_address,listen_port) {

    m_MaxTasks = max_threads_workers;
	m_DataTasksWaiting = new NPT_List<HTTPServerTaskData*>();
	m_DataTasksWorkers = new NPT_List<HTTPServerTask*>();
	m_childrens = new NPT_List<HTTPNode *>();
	{
		NPT_AutoLock lock(m_LoopLock);
		m_loop = true;
	}
	NPT_LOG_INFO(">>>>>>>>>>>>>>>>>>>>");
}

/*----------------------------------------------------------------------
|   HTTPServer::~HTTPServer
+---------------------------------------------------------------------*/
HTTPServer::~HTTPServer() {
	NPT_LOG_INFO("## destroyer  ...  ");
	if (isLoop())
		this->Stop();


    for(NPT_Cardinal i = 0;i<m_DataTasksWorkers->GetItemCount();i++)
	{
		HTTPServerTask* t= NULL;
		m_DataTasksWorkers->Get(i, t);
		if(t)
		{
			NPT_LOG_WARNING_1( "Task %llu is deleted !! ",t->GetCurrentThreadId());
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

/*----------------------------------------------------------------------
|   HTTPServer::isLoop
+---------------------------------------------------------------------*/
bool HTTPServer::isLoop() {
	{
		NPT_AutoLock lock(m_LoopLock);
		return m_loop;
	}
}


/*----------------------------------------------------------------------
|   HTTPServer::FindChildNodeOnTree
+---------------------------------------------------------------------*/
bool HTTPServer::FindChildNode(NPT_List<NPT_String> Segments,HTTPNode* &found )
{
	NPT_LOG_INFO("Fetching on Serving handlers ");
	::NPT_List<NPT_String>::Iterator it = Segments.GetFirstItem();
	::NPT_List<HTTPNode*>::Iterator t = m_childrens->GetFirstItem();
	::NPT_String segment = *it;

	while (t)
	{
		HTTPNode* nt = *t;
		// todo deal with start with and loop into  Segments
		NPT_LOG_INFO_3("Fetching on Tree : segment[%d]: '%s'  searching on children of '%s' !",
				Segments.GetItemCount(), segment.GetChars(), nt->getNodeHandler()->getSegment() );
		if (nt->operator==(segment))
		{
			NPT_LOG_FINE_1( "Fetching on Tree : segment  '%s'  found >>  on root !",  segment.GetChars());
			found = *t;
			return true;
		}
		// next Node
		t++;
	}

	NPT_LOG_SEVERE_2("Fetching on Tree : segment[%d]: '%s' NOT found  !", Segments.GetItemCount(), segment.GetChars());
	return false;

}
/*----------------------------------------------------------------------
|   HTTPServer::setTreeHandler
+---------------------------------------------------------------------*/
Result HTTPServer::AddHandler(IHTTPHandler* handler){
	if (handler != NULL)
	{
		HTTPNode *ch = new HTTPNode(handler);
		if (handler->GetMyHandlerType() == HANDLER_ASTERISK)
			m_childrens->Insert(m_childrens->GetLastItem(), ch);
		else
			m_childrens->Insert(m_childrens->GetFirstItem(), ch);
	}
	else
	{
		NPT_LOG_FATAL(" NULL Handle : will not be Added in Tree !");
	}
	return CMOP_SUCCESS;
}

/*----------------------------------------------------------------------
|   HTTPServer::CalculateQueryPath
+---------------------------------------------------------------------*/
NPT_Result HTTPServer::CalculateQueryPath(NPT_String UrlPath,
		NPT_List<NPT_String>& path) {
	NPT_LOG_INFO_1( "CalculateQueryPath returned (%s)", UrlPath.GetChars());
	// now managing only static path
	path.Add(UrlPath);
	//todo manage  Variable Url Path like '/root/a/b/c/{id}/g/' where '{id}' is a special to refer id value
	//     			 will be transformed to a list {' /root/a/b/c/','{id}','g'}.
	NPT_LOG_INFO_2( " %s >> %d ", UrlPath.GetChars(), path.GetItemCount());
	return NPT_SUCCESS;
}

/*----------------------------------------------------------------------
|   HTTPServer::Run
+---------------------------------------------------------------------*/
void HTTPServer::Run() {
	NPT_Result result = NPT_FAILURE;
	do {
		NPT_LOG_INFO("Test HTTP server waiting for connection ...");
		result = GetNewClient();
		NPT_LOG_INFO_2( "GetNewClient returned %d (%s)", result,
				NPT_ResultText(result));
	} while (isLoop());
}

/*----------------------------------------------------------------------
|   HTTPServer::Start
+---------------------------------------------------------------------*/
Result HTTPServer::StartServer() {
	this->Start();
	return CMOP_SUCCESS;
}
/*----------------------------------------------------------------------
|   HTTPServer::Stop
+---------------------------------------------------------------------*/
Result HTTPServer::Stop() {
	{
		NPT_AutoLock lock(m_LoopLock);
		m_loop = false;
		NPT_LOG_WARNING("Test HTTP server Stopped ...");
	}
	Abort();
	this->StopAllTasks();
	return CMOP_SUCCESS;
}

/*----------------------------------------------------------------------
|   HTTPServer::ProcessClientData
+---------------------------------------------------------------------*/
void HTTPServer::ProcessClientData(HTTPServerTask * task )
{
	NPT_LOG_INFO("will extract a new TaskData !");
	HTTPServerTaskData * taskData  = NULL ;
	{
		NPT_AutoLock lock(m_TasksLock);
		this->m_DataTasksWaiting->PopHead(taskData);
	}
	if(taskData != NULL)
	{
		NPT_LOG_INFO("Got the new popped TaskData !");
		if(task != NULL)
		{
			NPT_LOG_INFO_2("Will use an old idle task Worker  [%d/%d]!",this->m_DataTasksWorkers->GetItemCount(),m_MaxTasks);
			task->setData(taskData->m_input,taskData->m_output,taskData->m_context);
			task->backToWork();
		}
		else if (this->m_DataTasksWorkers->GetItemCount() < m_MaxTasks)
		{
			NPT_LOG_INFO("Will create a new task Worker  !");
			HTTPServerTask * tt = new HTTPServerTask(taskData->m_input,taskData->m_output,taskData->m_context,this);
			this->m_DataTasksWorkers->Add(tt);
			tt->Start();
		}
		else
		{
			NPT_LOG_FATAL( "Very Bizarre Error lost TaskData  : will not be executed ");
		}
		//cleaning
		delete taskData ;
	}
	else
	{
		NPT_LOG_INFO( "No Waiting TaskData  to be executed ");
	}
}

/*----------------------------------------------------------------------
|   HTTPServer::GetNewClient
+---------------------------------------------------------------------*/
Result HTTPServer::GetNewClient() {
	::NPT_InputStreamReference input;
	::NPT_OutputStreamReference output;
	::NPT_HttpRequestContext context;

	// Wait to get a new Client
	::NPT_Result result = WaitForNewClient(input, output, &context);
	NPT_LOG_FATAL_2( "HAVE A NEW CLIENT !! :WaitForNewClient returned %d (%s)", result,
			NPT_ResultText(result));
	if (NPT_FAILED(result))
		return HTTPUtility::MapNPTResult(result);

	{
		NPT_AutoLock lock(m_TasksLock);
		result = this->m_DataTasksWaiting->Add(new HTTPServerTaskData(input,output,&context));
		NPT_LOG_WARNING_2( "PushData  returned %d (%s)", result, NPT_ResultText(result));
	}

	NPT_LOG_WARNING_1("A new Data has arrived to be executed  [%s ]",NPT_ResultText(result));
	// check if we can pop data , else it will be lost
	HTTPServerTask * task = NULL;
	bool idle = IsAWorkerIdle(task);
	if((idle == true) || (this->m_DataTasksWorkers->GetItemCount() < m_MaxTasks) )
	{
		ProcessClientData(task );
	}
	else
	{
		NPT_LOG_WARNING("we cannot Launch a new Task , have to wait");
	}

	return HTTPUtility::MapNPTResult(result);
}

/*----------------------------------------------------------------------
|   HTTPServer::IsAWorkerIdle
+---------------------------------------------------------------------*/
bool HTTPServer::IsAWorkerIdle(HTTPServerTask* &task) {
	NPT_LOG_INFO_1( "Cheking if there is an Idle task from %d ones ",m_DataTasksWorkers->GetItemCount());
	int count = 0;
	for(NPT_Cardinal i = 0;i<m_DataTasksWorkers->GetItemCount();i++)
	{
		count++;
		HTTPServerTask* t= NULL;
		m_DataTasksWorkers->Get(i, t);
		if(t)
		{
			NPT_LOG_INFO_4( "on  %d/%d :%llu: IsIdle>> %d  ",count, m_DataTasksWorkers->GetItemCount(), t->GetCurrentThreadId(),t->IsIdle());
			if( t->IsIdle() == true )
			{
				task = t;
				return true;
			}
		}
	}
	return false;
}

/*----------------------------------------------------------------------
|   HTTPServer::StopAllTasks
+---------------------------------------------------------------------*/
Result HTTPServer::StopAllTasks()
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
			NPT_LOG_WARNING_1( "Task %llu is Stopped !! ",t->GetCurrentThreadId());
			t->Stop();
		}
	}
    NPT_LOG_WARNING(">> All task are Stopped !! ");
    return  CMOP_SUCCESS;
}
}
