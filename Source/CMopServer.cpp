/**
 *  \copyright Copyright (c) 2011, 2012 All Right Reserved
 *  \brief implementation de Http Server
 *  \file  HTTPServer.cpp
 *  \date 11 juin 2013
 *  \author Azzouni Mohamed
*/
#include "CMopServer.h"
#include "HTTPServer.h"
#include "HTTPServerTaskData.h"
#include "HTTPServerTask.h"
#include "HTTPTree.h"
#include "Neptune.h"

NPT_SET_LOCAL_LOGGER("cmop.apiserver")

namespace cmop
{


const IpAddress IpAddress::Any = IpAddress();
const IpAddress IpAddress::Loopback = IpAddress(127, 0, 0, 1);

/*----------------------------------------------------------------------
|   IpAddress::IpAddress
+---------------------------------------------------------------------*/
IpAddress::IpAddress() :
		m_Address(0)
{}

/*----------------------------------------------------------------------
|   IpAddress::IpAddress
+---------------------------------------------------------------------*/
IpAddress::IpAddress(unsigned long address) :
		m_Address(address)
{}

/*----------------------------------------------------------------------
|   IpAddress::IpAddress
+---------------------------------------------------------------------*/
IpAddress::IpAddress(unsigned char a,
						   unsigned char b,
						   unsigned char c,
						   unsigned char d)
{
	m_Address = (((unsigned long)a) << 24) |
				(((unsigned long)b) << 16) |
				(((unsigned long)c) << 8) |
				(((unsigned long)d));
}

/*----------------------------------------------------------------------
|   IHTTPHandler::~IHTTPHandler
+---------------------------------------------------------------------*/
IHTTPHandler::~IHTTPHandler()
{
	NPT_LOG_INFO_1( "## destructing  %s  ", m_segment.GetChars());
}

/*----------------------------------------------------------------------
|   IHTTPHandler::IHTTPHandler
+---------------------------------------------------------------------*/
IHTTPHandler::IHTTPHandler( char *  segment, METHODS   methodsSupportMask) :
    m_methodsSupportMask(methodsSupportMask)
{
	memset(m_segment, 0, sizeof(128));
	NPT_CopyMemory(m_segment, segment, strlen(segment));
	NPT_LOG_INFO_1( "## constructing  %s  ", m_segment);
}
/*----------------------------------------------------------------------
|   IHTTPHandler::getSegment
+---------------------------------------------------------------------*/
char *  IHTTPHandler::getSegment()
{
	return m_segment;
}

/*----------------------------------------------------------------------
|   IHTTPHandler::IHTTPHandler
+---------------------------------------------------------------------*/	
Result IHTTPHandlerImpl::ServeFile(const NPT_HttpRequest &request,
						const NPT_HttpRequestContext &context, NPT_HttpResponse &response,
						NPT_String file_path, const char *mime_type)
{
	UNUSED(request);
	UNUSED(context);
	NPT_InputStreamReference stream;
	NPT_File file(file_path);
	NPT_FileInfo file_info;

	// prevent hackers from accessing files outside of our root
	if ((file_path.Find("/..") >= 0) || (file_path.Find("\\..") >= 0) || NPT_FAILED(NPT_File::GetInfo(file_path.GetChars(), &file_info)))
	{
		return NPT_ERROR_NO_SUCH_ITEM;
	}

	/* check for range requests
	const NPT_String *range_spec = request.GetHeaders().GetHeaderValue(
		NPT_HTTP_HEADER_RANGE);

	 // handle potential 304 only if range header not set
	 NPT_DateTime  date;
	 NPT_TimeStamp timestamp;
	 if (NPT_SUCCEEDED(GetIfModifiedSince((NPT_HttpMessage&)request, date)) &&
	 !range_spec) {
	 date.ToTimeStamp(timestamp);

	 NPT_LOG_INFO_3( "File %s timestamps: request=%d (%s) vs file=%d (%s)",
	 (const char*)request.GetUrl().GetPath(),
	 (NPT_UInt32)timestamp.ToSeconds(),
	 (const char*)date.ToString(),
	 (NPT_UInt32)file_info.m_ModificationTime,
	 (const char*)NPT_DateTime(file_info.m_ModificationTime).ToString());

	 if (timestamp >= file_info.m_ModificationTime) {
	 // it's a match
	 NPT_LOG_INFO_1( "Returning 304 for %s", request.GetUrl().GetPath().GetChars());
	 response.SetStatus(304, "Not Modified", NPT_HTTP_PROTOCOL_1_1);
	 //return NPT_SUCCESS;
	 }
	 }*/

	// open file
	if (NPT_FAILED(file.Open(NPT_FILE_OPEN_MODE_READ)) || NPT_FAILED(file.GetInputStream(stream)) || stream.IsNull())
	{
		return NPT_ERROR_NO_SUCH_ITEM;
	}

	// set Last-Modified and Cache-Control headers
	if (file_info.m_ModificationTime)
	{
		NPT_DateTime last_modified = NPT_DateTime(file_info.m_ModificationTime);
		response.GetHeaders().SetHeader("Last-Modified",
										last_modified.ToString(NPT_DateTime::FORMAT_RFC_1123).GetChars(),
										true);
		response.GetHeaders().SetHeader("Cache-Control",
										"max-age=0,must-revalidate", true);
		//response.GetHeaders().SetHeader("Cache-Control", "max-age=1800", true);
	}

	if (stream.IsNull())
		return NPT_FAILURE;

	// set date
	NPT_TimeStamp now;
	NPT_System::GetCurrentTimeStamp(now);
	response.GetHeaders().SetHeader("Date",
									NPT_DateTime(now).ToString(NPT_DateTime::FORMAT_RFC_1123).GetChars(),
									true);

	// get entity
	NPT_HttpEntity *entity = response.GetEntity();
	if (entity == NULL)
	{
		NPT_LOG_FATAL( "Critic : Entity is Null !!");
		return NPT_FAILURE;
	}

	// set the content type
	entity->SetContentType(mime_type);

	// setup entity body
	// NPT_CHECK(NPT_HttpFileRequestHandler::SetupResponseBody(response, stream, ange_spec));

	// set some default headers
	if (response.GetEntity()->GetTransferEncoding() != NPT_HTTP_TRANSFER_ENCODING_CHUNKED)
	{
		// set but don't replace Accept-Range header only if body is seekable
		NPT_Position offset;
		if (NPT_SUCCEEDED(stream->Tell(offset)) && NPT_SUCCEEDED(stream->Seek(offset)))
		{
			response.GetHeaders().SetHeader(NPT_HTTP_HEADER_ACCEPT_RANGES,
											"bytes", false);
		}
	}

	return NPT_SUCCESS;
}
/*----------------------------------------------------------------------
|   IHTTPHandler::IHTTPHandler
+---------------------------------------------------------------------*/	
IHTTPHandler::OnCreate(NPT_HttpRequest &request,
						  const NPT_HttpRequestContext &context,
						  NPT_HttpResponse &response)
{
	NPT_LOG_INFO_1( "## OnCreate on  %s : Not Implemented ! ", m_segment.GetChars());
}

/*----------------------------------------------------------------------
|   IHTTPHandler::IHTTPHandler
+---------------------------------------------------------------------*/	
IHTTPHandler::OnRead(NPT_HttpRequest &request,
						const NPT_HttpRequestContext &context,
						NPT_HttpResponse &response) 
{
	NPT_LOG_INFO_1( "## OnRead on  %s : Not Implemented ! ", m_segment.GetChars());
}

/*----------------------------------------------------------------------
|   IHTTPHandler::IHTTPHandler
+---------------------------------------------------------------------*/	
IHTTPHandler::OnUpdate(NPT_HttpRequest &request,
						  const NPT_HttpRequestContext &context,
						  NPT_HttpResponse &response) 
{
	NPT_LOG_INFO_1( "## OnUpdate on  %s : Not Implemented ! ", m_segment.GetChars());
}


/*----------------------------------------------------------------------
|   IHTTPHandler::IHTTPHandler
+---------------------------------------------------------------------*/	
IHTTPHandler::OnDelete(NPT_HttpRequest &request,
						  const NPT_HttpRequestContext &context,
						  NPT_HttpResponse &response) 
{
	NPT_LOG_INFO_1( "## OnDelete on  %s : Not Implemented ! ", m_segment.GetChars());
}

/*----------------------------------------------------------------------
|   Server::Server
+---------------------------------------------------------------------*/
Server::Server(IpAddress listen_address,
					 unsigned short listen_port, 
					 unsigned short max_threads_workers) : m_listen_address(listen_address),
														   m_listen_port(listen_port),
														   m_max_threads_workers(max_threads_workers)
{

	m_server = new HTTPServer(NPT_IpAddress(m_listen_address.m_Address),
							  (NPT_UInt16) m_listen_port,
							  (NPT_UInt16) m_max_threads_workers);
	NPT_LOG_INFO("Server Created ");
}

/*----------------------------------------------------------------------
|   Server::~Server
+---------------------------------------------------------------------*/
Server::~Server()
{
	m_server->Stop();
	delete m_server;
	NPT_LOG_INFO("Server Stopped and deleted ");
}

/*----------------------------------------------------------------------
|   Server::Stop
+---------------------------------------------------------------------*/
Result Server::Stop()
{
	return m_server->Stop();
}

/*----------------------------------------------------------------------
|   Server::setRoot
+---------------------------------------------------------------------*/
void Server::setRoot( IHTTPHandler * treeHandler)
{
	m_server->setTreeHandler( new HTTPTree(treeHandler));
}
}
