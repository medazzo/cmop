/**
 *  \copyright Copyright (c) 2011, 2012 All Right Reserved
 *  \brief implementation de Http Server Task
 *  \file  HTTPServerTask.cpp
 *  \date 11 juin 2013
 *  \author Azzouni Mohamed
*/

#include "HTTPServerTask.h"
#include "HTTPUtility.h"
#include "IHTTPHandler.h"
#include "IHTTPEventHandler.h"

HTTPServerTask::HTTPServerTask(NPT_InputStreamReference& input,
		NPT_OutputStreamReference& output, NPT_HttpRequestContext* context,
		HTTPServer *server) :
		NPT_Thread() {
	m_server = server;
	m_input = input;
	m_output = output;
	m_context = context;
	m_running = true;
	m_status = new NPT_SharedVariable(1);
}
HTTPServerTask::~HTTPServerTask() {
	log_debug("## destructing  ");
	this->Stop();
	delete m_status;
}
void HTTPServerTask::setData(NPT_InputStreamReference& input,
		NPT_OutputStreamReference& output, NPT_HttpRequestContext* context) {
	if (IsIdle()) {
		this->m_context = context;
		this->m_input = input;
		this->m_output = output;
		log_debug(" Task : data are updated !");
	} else {
		log_error("Busy Task , data will not be updated !");
	}
}
void HTTPServerTask::backToWork() {
	if (IsIdle()) {
		m_status->SetValue(1);
		log_debug(" Task : data are updated !");
	} else {
		log_error("Busy Task , data will not be updated !");
	}
}
bool HTTPServerTask::IsIdle() {
	if (m_status->GetValue() == 0) //idle
		return true;
	else
		return false;
}
void HTTPServerTask::Stop() {
	if(m_running)
	{
		this->Interrupt();
		m_running = false ;
		m_status->SetValue(1);
		log_warning("Stopping Task ! ");
	}
	else
	{
		log_warning("Task is Already Stopped ! ");
	}
}
void HTTPServerTask::Run() {
	log_debug("HTTPServerTask is launched !! %d", m_status->GetValue());
	do {
		m_status->WaitWhileEquals(0); //has nothing to do
		if (m_running) {
			log_info("[task : %ld] : task Begin of the Run .",
					this->GetCurrentThreadId());
			this->RespondToClient();
			log_info("[task : %ld] : task End   of the Run .",
					this->GetCurrentThreadId());
			log_info("[task : %ld] : Backing TO idle %d .",
							this->GetCurrentThreadId(),m_status->GetValue());

			log_info("[task : %ld] : task End   of the Run .",
								this->GetCurrentThreadId());
			//back to idle status
			m_status->SetValue(0);

			log_info("[task : %ld] : Now we are idle , will seek new data to process from Server  .",
											this->GetCurrentThreadId());
			m_server->ProcessClientData(this);


		}
	} while (m_running);
	log_warning(" Task [%ld] is Stopped !! ",this->GetCurrentThreadId());
}
NPT_Result HTTPServerTask::RespondToClient() {
	NPT_HttpRequest* request;
	NPT_HttpResponse* response = NULL;
	NPT_Result result = NPT_ERROR_NO_SUCH_ITEM;
	bool resultsearch = false;
	bool terminate_server = false;
	IHTTPHandler* found = NULL;

	NPT_HttpResponder responder(m_input, m_output);
	result = responder.ParseRequest(request, &m_context->GetLocalAddress());
	if (result != NPT_SUCCESS ){
		log_warning("Failed responder.ParseRequest >> %d:%s!!",result,NPT_ResultText(result));
		if(request == NULL)
		{
			log_error("Failed responder. Getting a NULL Request !!");
			return result;
		}
	}
	log_info("request, path=%s",
			request->GetUrl().ToRequestString(true).GetChars());

	// prepare the response body , create a response object, and set it empty
	response = new NPT_HttpResponse(200, "OK", NPT_HTTP_PROTOCOL_1_0);
	NPT_HttpEntity* body = new NPT_HttpEntity();
	response->SetEntity(body);
	body->SetInputStream("");
	body->SetContentType("text/html");

   // augment the headers with server information
	response->GetHeaders().SetHeader(NPT_HTTP_HEADER_SERVER, HTTP_SERVER_NAME_VERSION_NUMBER, false);

	//Calculate demanded PATH
	NPT_List<NPT_String> path;
	result = this->m_server->CalculateQueryPath(request->GetUrl().GetPath(),
			path);
	log_info("CalculateQueryPath returned %d (%s)", result,
			NPT_ResultText(result));
	if (!NPT_FAILED(result) && (this->m_server->getTreeHandler() != NULL)) {
		//Search for handler correspondant to the PATH
		resultsearch = this->m_server->getTreeHandler()->FindChildNodeOnTree(
				path, found);
		log_info("FindChildNodeOnTree returned %d (%s)", result,
				NPT_ResultText(result));
	}

	if (found) {
		log_info("We Found the handle , will trait-it !");
		if(found->GetMyHandlerType() == HANDLER_EVENT /*TODO add check for method support*/)
		{
			log_info(">> THE HANDLER IS AN EVENTING ONE .");
			IHTTPEventHandler * event= (IHTTPEventHandler*) found;
			if (event->PushWaitingclient(m_input,m_output,m_context,request) == NPT_SUCCESS)
			{
				log_info(">> Event Are Armed , user will wait @@ %p ..",m_input.AsPointer());
				goto endEventing;
			}
			else
			{
				log_info(">> No Event Are Armed , user will get Response Immediately");
			}
		}

		// ask the handler to setup the response
		result = found->SetupResponse(*request, *m_context, *response);
		log_info("We Found the handle ,  it return %s  !",
				NPT_ResultText(result));

	}

	if ((result == NPT_ERROR_NO_SUCH_ITEM)|| (found == NULL)|| (resultsearch == false)){
		body->SetInputStream(NPT_HTTP_DEFAULT_404_HTML);
		body->SetContentType("text/html");
		response->SetStatus(404, "Not Found");
		//response->SetEntity(body);
		log_error("NPT_ERROR_NO_SUCH_ITEM ");
	}
	else if (result == NPT_ERROR_PERMISSION_DENIED)
	{

		body->SetInputStream(NPT_HTTP_DEFAULT_403_HTML);
		body->SetContentType("text/html");
		response->SetStatus(402, "Not supported");
		log_error("NPT_ERROR_PERMISSION_DENIED ");
	}
	else if (result == NPT_ERROR_TERMINATED)
	{
		// mark that we want to exit
		terminate_server = true;
	}
	else if (NPT_FAILED(result))
	{
		body->SetInputStream(NPT_HTTP_DEFAULT_500_HTML);
		body->SetContentType("text/html");
		response->SetStatus(500, "Internal Error");
		log_error("NPT_ERROR_TERMINATED ");
	}

	// send the response headers
	result = responder.SendResponseHeaders(*response);
	if (NPT_FAILED(result)) {
		log_info("SendResponseHeaders failed (%d:%s)", result,
				NPT_ResultText(result));
		goto end;
	}

	// send the body
	if (request->GetMethod() != NPT_HTTP_METHOD_HEAD) {
		if (found) {
			result = found->SendResponseBody(*m_context, *response, *m_output);
		} else {
			// send body manually in case there was an error with the handler or no handler was found
			NPT_InputStreamReference body_stream;
			log_error("Sending The Body !!!!!!!!!!!!!!!!");
			body->GetInputStream(body_stream);
			if (!body_stream.IsNull()) {
				result = NPT_StreamToStreamCopy(*body_stream, *m_output, 0,
						body->GetContentLength());
				if (NPT_FAILED(result)) {
					log_info("NPT_StreamToStreamCopy returned %d (%s)", result,
							NPT_ResultText(result));
					goto end;
				}
			}
		}
	}

	// flush
	m_output->Flush();

	// if we need to die, we return an error code
	if (NPT_SUCCEEDED(result) && terminate_server)
		result = NPT_ERROR_TERMINATED;

end:
	// cleanup
	delete request;
endEventing:
	delete response;
	return result;
}