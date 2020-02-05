/*
 * HTTPServerTestHandlers.h
 *
 *  Created on: 5 juin 2013
 *      Author: azzouni
 */

#ifndef HTTPSERVERTESTHANDLERS_H_
#define HTTPSERVERTESTHANDLERS_H_

#include "Neptune.h" // just for logging
#include "CMopServer.h"


#define Test_UNUSED(x)	 (void)(x)

using namespace cmop;
NPT_SET_LOCAL_LOGGER("test.server.handlers")
/*
+---------------------------------------------------------------------
|       TestHttp
+---------------------------------------------------------------------
*/
class RootHandler : public IHTTPHandler{
public:
	RootHandler(char * segment):IHTTPHandler(segment,SUPPORT_NONE){};
	HANDLERSTYPES GetMyHandlerType() { return HANDLERSTYPES::HANDLER_STATIC; }
};

class AstrixHandler : public IHTTPHandler{
public:
	AstrixHandler():IHTTPHandler(SUPPORT_GET){};
	HANDLERSTYPES GetMyHandlerType() { return HANDLERSTYPES::HANDLER_ASTERISK; }

	virtual void OnRead  (NPT_HttpRequest& request, const NPT_HttpRequestContext& context,NPT_HttpResponse& response){
		Test_UNUSED(request);
		Test_UNUSED(context);
		Test_UNUSED(response);
		NPT_TimeStamp ts;
			NPT_System::GetCurrentTimeStamp(ts);
			NPT_DateTime  l_creationDate;
			l_creationDate.FromTimeStamp(ts);
			NPT_String msg =NPT_String::Format("<HTML> <B> !!!  [%s] GOOD AT OnReadHandler::%s !!! </B> </HTML>",l_creationDate.ToString(NPT_DateTime::FORMAT_W3C),__FUNCTION__);
			NPT_HttpEntity* entity = response.GetEntity();
			entity->SetContentType("text/html");
			entity->SetInputStream(msg);
			NPT_LOG_INFO_2( "On A client %s, @@  '%s' !",this->getSegment(),l_creationDate.ToString(NPT_DateTime::FORMAT_W3C).GetChars());

	}
};
/*

class FileServing : public IHTTPHandler{
public:
	FileServing(NPT_String segment):IHTTPHandler(segment,SUPPORT_GET){
		idx = 0;
		folder = NPT_String("/home/medazzo/Images");
		NPT_File::ListDir(folder,entrys);
	};
private:
	int idx ;
	NPT_String folder;
	NPT_List<NPT_String> entrys  ;
	virtual void OnCreate(NPT_HttpRequest& request, const NPT_HttpRequestContext& context,NPT_HttpResponse& response){
		Test_UNUSED(request);
		Test_UNUSED(context);
		Test_UNUSED(response);
	}
	virtual void OnRead  (NPT_HttpRequest& request, const NPT_HttpRequestContext& context,NPT_HttpResponse& response){
		Test_UNUSED(request);
		Test_UNUSED(context);
		Test_UNUSED(response);
		NPT_String file;
		NPT_String mime = "image/jpeg ";
		int ii = idx % entrys.GetItemCount();
		entrys.Get(ii,file);
		file = NPT_String::Format("%s/%s",folder,file );
		NPT_Result res =  this->ServeFile(request,context,response,file,mime);
		NPT_LOG_INFO_6( "On A client %s,serving the file::%d/%d:: '%s' with mime '%s' have error '%s' !",
				this->getSegment(),ii,entrys.GetItemCount(),file,mime,NPT_ResultText(res));
		idx++;
	}
};*/
class OnCreateHandler : public IHTTPHandler{
public:
	OnCreateHandler(char * segment):IHTTPHandler(segment,SUPPORT_PUT){};
	HANDLERSTYPES GetMyHandlerType() { return HANDLERSTYPES::HANDLER_STATIC; }
private:
	virtual void OnCreate(NPT_HttpRequest& request, const NPT_HttpRequestContext& context,NPT_HttpResponse& response) {
		Test_UNUSED(request);
		Test_UNUSED(context);
		Test_UNUSED(response);
		NPT_TimeStamp ts;
		NPT_System::GetCurrentTimeStamp(ts);
		NPT_DateTime  l_creationDate;
		l_creationDate.FromTimeStamp(ts);
		NPT_String msg =NPT_String::Format("<HTML> <B> !!!  [%s] GOOD AT OnCreateHandler::%s !!! </B> </HTML>",l_creationDate.ToString(NPT_DateTime::FORMAT_W3C),__FUNCTION__);
		NPT_HttpEntity* entity = response.GetEntity();
		entity->SetContentType("text/html");
		entity->SetInputStream(msg);
		NPT_LOG_INFO_2( "On A client %s, @@  '%s' !",this->getSegment(),l_creationDate.ToString(NPT_DateTime::FORMAT_W3C).GetChars());
	}
};
class ErrorHandler : public IHTTPHandler{
public:
	ErrorHandler(char * segment):IHTTPHandler(segment,SUPPORT_PUT){};
	HANDLERSTYPES GetMyHandlerType() { return HANDLERSTYPES::HANDLER_STATIC; }
private:
	virtual void OnRead  (NPT_HttpRequest& request, const NPT_HttpRequestContext& context,NPT_HttpResponse& response){
		Test_UNUSED(request);
		Test_UNUSED(context);
		Test_UNUSED(response);
		NPT_TimeStamp ts;
		NPT_System::GetCurrentTimeStamp(ts);
		NPT_DateTime  l_creationDate;
		l_creationDate.FromTimeStamp(ts);
		NPT_String msg =NPT_String::Format("<HTML> <B> !!!  [%s] GOOD AT OnReadHandler::%s !!! </B> </HTML>",l_creationDate.ToString(NPT_DateTime::FORMAT_W3C),__FUNCTION__);
		NPT_HttpEntity* entity = response.GetEntity();
		entity->SetContentType("text/html");
		entity->SetInputStream(msg);
		NPT_LOG_INFO_2("On A client %s, @@  '%s' !",this->getSegment(),l_creationDate.ToString(NPT_DateTime::FORMAT_W3C).GetChars());
	}
};
/*
class EventingHandler : public IHTTPEventHandler{
public:
	EventingHandler(char * segment):IHTTPEventHandler(segment,SUPPORT_GET){};
	HANDLERSTYPES GetMyHandlerType() { return HANDLERSTYPES::HANDLER_ASTERISK; }
private:

	virtual void OnRead  (NPT_HttpRequest& request, const NPT_HttpRequestContext& context,NPT_HttpResponse& response){
		Test_UNUSED(request);
		Test_UNUSED(context);
		Test_UNUSED(response);
		NPT_LOG_INFO_1("On A client %s ; Will wait until  get 0  !",this->getSegment());
		NPT_TimeStamp ts;
		NPT_System::GetCurrentTimeStamp(ts);
		NPT_DateTime  l_creationDate;
		l_creationDate.FromTimeStamp(ts);
		NPT_String msg =NPT_String::Format("<HTML> <B> !!!  [%s] GOOD AT EVENTING HANDLER ::%s !!! </B> </HTML>",l_creationDate.ToString(NPT_DateTime::FORMAT_W3C),__FUNCTION__);
		NPT_HttpEntity* entity = response.GetEntity();
		entity->SetContentType("text/html");
		entity->SetInputStream(msg);
		NPT_LOG_INFO_2("On A client %s, @@  '%s' ;End of  wait, got 0 !!",this->getSegment(),l_creationDate.ToString(NPT_DateTime::FORMAT_W3C).GetChars());
	}
};
*/
class OnReadHandler : public IHTTPHandler{
public:
	OnReadHandler(char * segment):IHTTPHandler(segment,SUPPORT_GET){};
	HANDLERSTYPES GetMyHandlerType() { return HANDLERSTYPES::HANDLER_STATIC; }
private:
	virtual void OnRead  (NPT_HttpRequest& request, const NPT_HttpRequestContext& context,NPT_HttpResponse& response){
		Test_UNUSED(request);
		Test_UNUSED(context);
		Test_UNUSED(response);
		NPT_TimeStamp ts;
		NPT_System::GetCurrentTimeStamp(ts);
		NPT_DateTime  l_creationDate;
		l_creationDate.FromTimeStamp(ts);
		NPT_String msg =NPT_String::Format("<HTML> <B> !!!  [%s] GOOD AT OnReadHandler::%s !!! </B> </HTML>",l_creationDate.ToString(NPT_DateTime::FORMAT_W3C),__FUNCTION__);
		NPT_HttpEntity* entity = response.GetEntity();
		entity->SetContentType("text/html");
		entity->SetInputStream(msg);
		NPT_LOG_INFO_2("On A client %s, @@  '%s' !",this->getSegment(),l_creationDate.ToString(NPT_DateTime::FORMAT_W3C).GetChars());
	}
};
class OnUpdateHandler : public IHTTPHandler{
public:
	OnUpdateHandler(char* segment):IHTTPHandler(segment,SUPPORT_POST){};
	HANDLERSTYPES GetMyHandlerType() { return HANDLERSTYPES::HANDLER_STATIC; }
private:
	virtual void OnUpdate(NPT_HttpRequest& request, const NPT_HttpRequestContext& context,NPT_HttpResponse& response){
		Test_UNUSED(request);
		Test_UNUSED(context);
		Test_UNUSED(response);
		NPT_TimeStamp ts;
		NPT_System::GetCurrentTimeStamp(ts);
		NPT_DateTime  l_creationDate;
		l_creationDate.FromTimeStamp(ts);
		NPT_String msg =NPT_String::Format("<HTML> <B> !!!  [%s] GOOD AT OnUpdateHandler::%s !!! </B> </HTML>",l_creationDate.ToString(NPT_DateTime::FORMAT_W3C),__FUNCTION__);
		NPT_HttpEntity* entity = response.GetEntity();
		entity->SetContentType("text/html");
		entity->SetInputStream(msg);
		NPT_LOG_INFO_2("On A client %s, @@  '%s' !",this->getSegment(),l_creationDate.ToString(NPT_DateTime::FORMAT_W3C).GetChars());
	}
};
class OnDeleteHandler : public IHTTPHandler{
public:
	OnDeleteHandler( char *   segment):IHTTPHandler(segment,SUPPORT_DELETE){};
	HANDLERSTYPES GetMyHandlerType() { return HANDLERSTYPES::HANDLER_STATIC; }
private:
	virtual void OnDelete(NPT_HttpRequest& request, const NPT_HttpRequestContext& context,NPT_HttpResponse& response){
		Test_UNUSED(request);
		Test_UNUSED(context);
		Test_UNUSED(response);
		NPT_TimeStamp ts;
		NPT_System::GetCurrentTimeStamp(ts);
		NPT_DateTime  l_creationDate;
		l_creationDate.FromTimeStamp(ts);
		NPT_String msg =NPT_String::Format("<HTML> <B> !!!  [%s] GOOD AT OnDeleteHandler::%s !!! </B> </HTML>",l_creationDate.ToString(NPT_DateTime::FORMAT_W3C),__FUNCTION__);
		NPT_HttpEntity* entity = response.GetEntity();
		entity->SetContentType("text/html");
		entity->SetInputStream(msg);
		NPT_LOG_INFO_2("On A client %s, @@  '%s' !",this->getSegment(),l_creationDate.ToString(NPT_DateTime::FORMAT_W3C).GetChars());
	}
};
class OnAllHandler : public IHTTPHandler{
public:
	OnAllHandler( char *   segment):IHTTPHandler(segment ,SUPPORT_ALL){};
	HANDLERSTYPES GetMyHandlerType() { return HANDLERSTYPES::HANDLER_STATIC; }
private:
	virtual void OnCreate(NPT_HttpRequest& request, const NPT_HttpRequestContext& context,NPT_HttpResponse& response){
		Test_UNUSED(request);
		Test_UNUSED(context);
		Test_UNUSED(response);
		NPT_TimeStamp ts;
		NPT_System::GetCurrentTimeStamp(ts);
		NPT_DateTime  l_creationDate;
		l_creationDate.FromTimeStamp(ts);
		NPT_String msg =NPT_String::Format("<HTML> <B> !!!  [%s] GOOD AT OnAllHandler::%s !!! </B> </HTML>",l_creationDate.ToString(NPT_DateTime::FORMAT_W3C),__FUNCTION__);
		NPT_HttpEntity* entity = response.GetEntity();
		entity->SetContentType("text/html");
		entity->SetInputStream(msg);
		NPT_LOG_INFO_2("On A client %s, @@  '%s' !",this->getSegment(),l_creationDate.ToString(NPT_DateTime::FORMAT_W3C).GetChars());
	}
 	virtual void OnRead  (NPT_HttpRequest& request, const NPT_HttpRequestContext& context,NPT_HttpResponse& response){
		 Test_UNUSED(request);
		Test_UNUSED(context);
		Test_UNUSED(response);
		NPT_TimeStamp ts;
		NPT_System::GetCurrentTimeStamp(ts);
		NPT_DateTime  l_creationDate;
		l_creationDate.FromTimeStamp(ts);
		NPT_String msg =NPT_String::Format("<HTML> <B> !!!  [%s] GOOD AT OnAllHandler::%s !!! </B> </HTML>",l_creationDate.ToString(NPT_DateTime::FORMAT_W3C),__FUNCTION__);
		NPT_HttpEntity* entity = response.GetEntity();
		entity->SetContentType("text/html");
		entity->SetInputStream(msg);
		NPT_LOG_INFO_2("On A client %s, @@  '%s' !",this->getSegment(),l_creationDate.ToString(NPT_DateTime::FORMAT_W3C).GetChars());
 	}
	virtual void OnUpdate(NPT_HttpRequest& request, const NPT_HttpRequestContext& context,NPT_HttpResponse& response){
		Test_UNUSED(request);
		Test_UNUSED(context);
		Test_UNUSED(response);
		NPT_TimeStamp ts;
		NPT_System::GetCurrentTimeStamp(ts);
		NPT_DateTime  l_creationDate;
		l_creationDate.FromTimeStamp(ts);
		NPT_String msg =NPT_String::Format("<HTML> <B> !!!  [%s] GOOD AT OnAllHandler::%s !!! </B> </HTML>",l_creationDate.ToString(NPT_DateTime::FORMAT_W3C),__FUNCTION__);
		NPT_HttpEntity* entity = response.GetEntity();
		entity->SetContentType("text/html");
		entity->SetInputStream(msg);
		NPT_LOG_INFO_2("On A client %s, @@  '%s' !",this->getSegment(),l_creationDate.ToString(NPT_DateTime::FORMAT_W3C).GetChars());
	}
	virtual void OnDelete(NPT_HttpRequest& request, const NPT_HttpRequestContext& context,NPT_HttpResponse& response){
		Test_UNUSED(request);
		Test_UNUSED(context);
		Test_UNUSED(response);
		NPT_TimeStamp ts;
		NPT_System::GetCurrentTimeStamp(ts);
		NPT_DateTime  l_creationDate;
		l_creationDate.FromTimeStamp(ts);
		NPT_String msg =NPT_String::Format("<HTML> <B> !!!  [%s] GOOD AT OnAllHandler::%s !!! </B> </HTML>",l_creationDate.ToString(NPT_DateTime::FORMAT_W3C),__FUNCTION__);
		NPT_HttpEntity* entity = response.GetEntity();
		entity->SetContentType("text/html");
		entity->SetInputStream(msg);
		NPT_LOG_INFO_2("On A client %s, @@  '%s' !",this->getSegment(),l_creationDate.ToString(NPT_DateTime::FORMAT_W3C).GetChars());
	}
};


#endif /* HTTPSERVERTESTHANDLERS_H_ */
