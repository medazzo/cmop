/**
 *  \copyright Copyright (c) 2011, 2012 All Right Reserved
 *  \brief implementation of Http Handlers interface.
 *  \file IHTTPHandler.cpp
 *  \date 11 juin 2013
 *  \author Azzouni Mohamed
 */

#include "HTTPUtility.h"
#include "IHTTPHandler.h"

#include "Neptune.h"
NPT_SET_LOCAL_LOGGER("cmop.server.handler")

IHTTPHandler::~IHTTPHandler()
{
	NPT_LOG_INFO_1( "## destructing  %s  ", m_segment.GetChars());
}

IHTTPHandler::IHTTPHandler(const char *segment, METHODS methodsSupportMask) : m_segment(segment), m_methodsSupportMask(methodsSupportMask)
{
	NPT_LOG_INFO_1( "## constructing  %s  ", m_segment.GetChars());
}

NPT_Result
IHTTPHandler::GetIfModifiedSince(const NPT_HttpMessage &message,
								 NPT_DateTime &date)
{

	const NPT_String *value = message.GetHeaders().GetHeaderValue(
		"If-Modified-Since");
	if (!value)
		return NPT_FAILURE;

	// Try RFC 1123, RFC 1036, then ANSI
	if (NPT_SUCCEEDED(date.FromString((*value).GetChars(), NPT_DateTime::FORMAT_RFC_1123)))
		return NPT_SUCCESS;
	if (NPT_SUCCEEDED(date.FromString((*value).GetChars(), NPT_DateTime::FORMAT_RFC_1036)))
		return NPT_SUCCESS;
	return date.FromString((*value).GetChars(), NPT_DateTime::FORMAT_ANSI);
}
NPT_Result
IHTTPHandler::ServeFile(const NPT_HttpRequest &request,
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

NPT_Result
IHTTPHandler::SetupResponse(NPT_HttpRequest &request,
							const NPT_HttpRequestContext &context, NPT_HttpResponse &response)
{
	if ((request.GetMethod().Compare(NPT_HTTP_METHOD_GET) == 0) && check_support(m_methodsSupportMask, SUPPORT_GET))
	{
		OnRead(request, context, response);
		return NPT_SUCCESS;
	}
	else if ((request.GetMethod().Compare(NPT_HTTP_METHOD_POST) == 0) && check_support(m_methodsSupportMask, SUPPORT_POST))
	{
		OnUpdate(request, context, response);
		return NPT_SUCCESS;
	}
	else if ((request.GetMethod().Compare(NPT_HTTP_METHOD_PUT) == 0) && check_support(m_methodsSupportMask, SUPPORT_PUT))
	{
		OnCreate(request, context, response);
		return NPT_SUCCESS;
	}
	else if ((request.GetMethod().Compare(NPT_HTTP_METHOD_DELETE) == 0) && check_support(m_methodsSupportMask, SUPPORT_DELETE))
	{
		OnDelete(request, context, response);
		return NPT_SUCCESS;
	}
	else
	{
		NPT_LOG_FATAL_1( "## Error : NOT Supported Method Call : %s !",
				  request.GetMethod().GetChars());
		return NPT_ERROR_NOT_SUPPORTED;
	}
}
NPT_Result
IHTTPHandler::SendResponseBody(
	const NPT_HttpRequestContext &context, NPT_HttpResponse &response,
	NPT_OutputStream &output)
{
	UNUSED(context);
	NPT_HttpEntity *entity = response.GetEntity();
	if (entity == NULL)
		return NPT_SUCCESS;

	NPT_InputStreamReference body_stream;
	entity->GetInputStream(body_stream);
	if (body_stream.IsNull())
		return NPT_SUCCESS;

	// check for chunked transfer encoding
	NPT_OutputStream *dest = &output;
	if (entity->GetTransferEncoding() == NPT_HTTP_TRANSFER_ENCODING_CHUNKED)
	{
		dest = new NPT_HttpChunkedOutputStream(output);
	}

	// send the body
	NPT_LOG_INFO_1( "sending body stream, %lld bytes", entity->GetContentLength());
	NPT_LargeSize bytes_written = 0;
	NPT_Result result = NPT_StreamToStreamCopy(*body_stream, *dest, 0,
											   entity->GetContentLength(), &bytes_written);
	if (NPT_FAILED(result))
	{
		NPT_LOG_INFO_3( "body stream only partially sent, %lld bytes (%d:%s)",
				  bytes_written, result, NPT_ResultText(result));
	}

	// flush to write out any buffered data left in chunked output if used
	dest->Flush();

	// cleanup (this will send zero size chunk followed by CRLF)
	if (dest != &output)
		delete dest;

	return result;
}
bool IHTTPHandler::operator==(const NPT_String &other)
{
	NPT_LOG_INFO_2( "##@ %s  ?? %s ##", m_segment.GetChars(), other.GetChars());
	return (m_segment == other) ? true : false;
}
NPT_String &
IHTTPHandler::getSegment()
{
	return m_segment;
}
