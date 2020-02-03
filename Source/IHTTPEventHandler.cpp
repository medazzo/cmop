/**
 *  \copyright Copyright (c) 2011, 2012 All Right Reserved
 *  \brief implementation of the Eventing Http Handler
 *  \file  IHTTPEventHandler.cpp
 *  \date 11 juin 2013
 *  \author Azzouni Mohamed
*/

#include "IHTTPEventHandler.h"
#include "Neptune.h"

NPT_SET_LOCAL_LOGGER("cmop.server.handler.event")

IHTTPEventHandler::~IHTTPEventHandler() {
	m_WaitingClient->Clear();
	delete m_WaitingClient;
}

IHTTPEventHandler::IHTTPEventHandler(const char* segment, METHODS methodsSupportMask) :
		IHTTPHandler(segment,m_methodsSupportMask) {
	UNUSED(methodsSupportMask);
	UNUSED(segment);
	m_WaitingClient = new NPT_List<HTTPServerTaskData*>();
	m_sendnAnEvent = false ;
}

NPT_Result
IHTTPEventHandler::PushWaitingclient(NPT_InputStreamReference m_input,
							NPT_OutputStreamReference m_output,NPT_HttpRequestContext* m_context,NPT_HttpRequest* request) {
	if ( m_sendnAnEvent == false ){
		m_WaitingClient->Add( new HTTPServerTaskData(m_input,m_output,m_context,request));
		return NPT_SUCCESS;
	}
	else
	{
		// have to respond client immediately
		return NPT_FAILURE;
	}
}



void
IHTTPEventHandler::NotifyWaitingClients() {
	if ( m_sendnAnEvent == false )
	{
		//send client responses ;
		int count = 0;
		this->m_WaitingClient->GetItemCount();
		HTTPServerTaskData * taskata  = NULL ;
		while(this->m_WaitingClient->PopHead(taskata)  == NPT_SUCCESS )
		{
			if(taskata != NULL)
			{
				count++;
				NPT_LOG_FINE_1( "Sending response to client %d: ",count);

				NPT_HttpResponse* response = NULL;
				NPT_Result result = NPT_ERROR_NO_SUCH_ITEM;
				bool terminate_server = false;
				IHTTPHandler* found = NULL;
				NPT_LOG_INFO_1( ">> Event Are Armed , user will wait @@ %p ..",(void*)taskata->m_input.AsPointer());
				NPT_HttpResponder responder(taskata->m_input, taskata->m_output);

				// prepare the response body , create a response object, and set it empty
				response = new NPT_HttpResponse(200, "OK", NPT_HTTP_PROTOCOL_1_0);
				NPT_HttpEntity* body = new NPT_HttpEntity();
				response->SetEntity(body);
				body->SetInputStream("");
				body->SetContentType("text/html");

				//Process .
				this->SetupResponse(*taskata->m_request, *taskata->m_context, *response);

				// augment the headers with server information
				response->GetHeaders().SetHeader(NPT_HTTP_HEADER_SERVER,
						"Mon Serveur De TEST", false);

				// send the response headers
				result = responder.SendResponseHeaders(*response);
				if (NPT_FAILED(result)) {
					NPT_LOG_INFO_2( "SendResponseHeaders failed (%d:%s)", result,
							NPT_ResultText(result));
					goto end;
				}

				// send the body
				if (taskata->m_request->GetMethod() != NPT_HTTP_METHOD_HEAD) {
					if (found) {
						result = found->SendResponseBody(*taskata->m_context, *response, *taskata->m_output);
					} else {
						// send body manually in case there was an error with the handler or no handler was found
						NPT_InputStreamReference body_stream;
						body->GetInputStream(body_stream);
						if (!body_stream.IsNull()) {
							result = NPT_StreamToStreamCopy(*body_stream, *taskata->m_output, 0,
									body->GetContentLength());
							if (NPT_FAILED(result)) {
								NPT_LOG_INFO_2( "NPT_StreamToStreamCopy returned %d (%s)", result, NPT_ResultText(result));
								goto end;
							}
						}
					}
				}

				// flush
				taskata->m_output->Flush();

				// if we need to die, we return an error code
				if (NPT_SUCCEEDED(result) && terminate_server)
					result = NPT_ERROR_TERMINATED;

				end:
				// cleanup
				delete response;
				//delete taskata->m_request;
				delete taskata;
			}
		}

		//update variable
		m_sendnAnEvent = true;
	}
	else
	{
		// already done
	}
}

void
IHTTPEventHandler::ArmEventToBlockNewClients() {
	m_sendnAnEvent = false ;
}

