/*
 * HTTPServerTestHandlers.h
 *
 *  Created on: 5 juin 2013
 *      Author: azzouni
 */

#ifndef HTTPSERVERTESTHANDLERS_H_
#define HTTPSERVERTESTHANDLERS_H_

#include "HTTPUtility.h"
#include "IHTTPHandler.h"
#include "IHTTPEventHandler.h"
#include "IHTTPAsteriskHandler.h"

/*
+---------------------------------------------------------------------
|       TestHttp
+---------------------------------------------------------------------
*/
class RootHandler : public IHTTPHandler{
public:
	RootHandler(NPT_String segment):IHTTPHandler(segment.GetChars(),SUPPORT_NONE){};
	virtual void OnCreate(NPT_HttpRequest& request, const NPT_HttpRequestContext& context,NPT_HttpResponse& response){}
	virtual void OnRead  (NPT_HttpRequest& request, const NPT_HttpRequestContext& context,NPT_HttpResponse& response){}
	virtual void OnUpdate(NPT_HttpRequest& request, const NPT_HttpRequestContext& context,NPT_HttpResponse& response){}
	virtual void OnDelete(NPT_HttpRequest& request, const NPT_HttpRequestContext& context,NPT_HttpResponse& response){}
};

class AstrixHandler : public IHTTPAsteriskHandler{
public:
	AstrixHandler():IHTTPAsteriskHandler(SUPPORT_GET){};
	virtual void OnCreate(NPT_HttpRequest& request, const NPT_HttpRequestContext& context,NPT_HttpResponse& response){}
	virtual void OnRead  (NPT_HttpRequest& request, const NPT_HttpRequestContext& context,NPT_HttpResponse& response){
		NPT_TimeStamp ts;
			NPT_System::GetCurrentTimeStamp(ts);
			NPT_DateTime  l_creationDate;
			l_creationDate.FromTimeStamp(ts);
			NPT_String msg =NPT_String::Format("<HTML> <B> !!!  [%s] GOOD AT OnReadHandler::%s !!! </B> </HTML>",l_creationDate.ToString(NPT_DateTime::FORMAT_W3C).GetChars(),__FUNCTION__);
			NPT_HttpEntity* entity = response.GetEntity();
			entity->SetContentType("text/html");
			entity->SetInputStream(msg);
			log_debug("On A client %s, @@  '%s' !",this->getSegment().GetChars(),l_creationDate.ToString(NPT_DateTime::FORMAT_W3C).GetChars());

	}
	virtual void OnUpdate(NPT_HttpRequest& request, const NPT_HttpRequestContext& context,NPT_HttpResponse& response){}
	virtual void OnDelete(NPT_HttpRequest& request, const NPT_HttpRequestContext& context,NPT_HttpResponse& response){}
};


class FileServing : public IHTTPHandler{
public:
	FileServing(NPT_String segment):IHTTPHandler(segment.GetChars(),SUPPORT_GET){
		idx = 0;
		folder = NPT_String("/home/azzouni/Images");
		NPT_File::ListDir(folder.GetChars(),entrys);
	};
private:
	int idx ;
	NPT_String folder;
	NPT_List<NPT_String> entrys  ;
	virtual void OnCreate(NPT_HttpRequest& request, const NPT_HttpRequestContext& context,NPT_HttpResponse& response){}
	virtual void OnRead  (NPT_HttpRequest& request, const NPT_HttpRequestContext& context,NPT_HttpResponse& response){
		NPT_String file;
		NPT_String mime = "image/jpeg ";
		int ii = idx % entrys.GetItemCount();
		entrys.Get(ii,file);
		file = NPT_String::Format("%s/%s",folder.GetChars(),file.GetChars() );
		NPT_Result res =  this->ServeFile(request,context,response,file.GetChars(),mime.GetChars());
		log_debug("On A client %s,serving the file::%d/%d:: '%s' with mime '%s' have error '%s' !",
				this->getSegment().GetChars(),ii,entrys.GetItemCount(),file.GetChars(),mime.GetChars(),NPT_ResultText(res));
		idx++;
	}
	virtual void OnUpdate(NPT_HttpRequest& request, const NPT_HttpRequestContext& context,NPT_HttpResponse& response){}
	virtual void OnDelete(NPT_HttpRequest& request, const NPT_HttpRequestContext& context,NPT_HttpResponse& response){}
};
class OnCreateHandler : public IHTTPHandler{
public:
	OnCreateHandler(NPT_String segment):IHTTPHandler(segment.GetChars(),SUPPORT_PUT){};
private:
	virtual void OnCreate(NPT_HttpRequest& request, const NPT_HttpRequestContext& context,NPT_HttpResponse& response) {
		NPT_TimeStamp ts;
		NPT_System::GetCurrentTimeStamp(ts);
		NPT_DateTime  l_creationDate;
		l_creationDate.FromTimeStamp(ts);
		NPT_String msg =NPT_String::Format("<HTML> <B> !!!  [%s] GOOD AT OnCreateHandler::%s !!! </B> </HTML>",l_creationDate.ToString(NPT_DateTime::FORMAT_W3C).GetChars(),__FUNCTION__);
		NPT_HttpEntity* entity = response.GetEntity();
		entity->SetContentType("text/html");
		entity->SetInputStream(msg);
		log_debug("On A client %s, @@  '%s' !",this->getSegment().GetChars(),l_creationDate.ToString(NPT_DateTime::FORMAT_W3C).GetChars());
	}
	virtual void OnRead  (NPT_HttpRequest& request, const NPT_HttpRequestContext& context,NPT_HttpResponse& response){}
	virtual void OnUpdate(NPT_HttpRequest& request, const NPT_HttpRequestContext& context,NPT_HttpResponse& response){}
	virtual void OnDelete(NPT_HttpRequest& request, const NPT_HttpRequestContext& context,NPT_HttpResponse& response){}
};
class ErrorHandler : public IHTTPHandler{
public:
	ErrorHandler(NPT_String segment):IHTTPHandler(segment.GetChars(),SUPPORT_PUT){};
private:
	virtual void OnCreate(NPT_HttpRequest& request, const NPT_HttpRequestContext& context,NPT_HttpResponse& response) {
		log_error("BUGGED :: BUGGED : on vas rien faire !");
	}
	virtual void OnRead  (NPT_HttpRequest& request, const NPT_HttpRequestContext& context,NPT_HttpResponse& response){
		NPT_TimeStamp ts;
		NPT_System::GetCurrentTimeStamp(ts);
		NPT_DateTime  l_creationDate;
		l_creationDate.FromTimeStamp(ts);
		NPT_String msg =NPT_String::Format("<HTML> <B> !!!  [%s] GOOD AT OnReadHandler::%s !!! </B> </HTML>",l_creationDate.ToString(NPT_DateTime::FORMAT_W3C).GetChars(),__FUNCTION__);
		NPT_HttpEntity* entity = response.GetEntity();
		entity->SetContentType("text/html");
		entity->SetInputStream(msg);
		log_debug("On A client %s, @@  '%s' !",this->getSegment().GetChars(),l_creationDate.ToString(NPT_DateTime::FORMAT_W3C).GetChars());
	}
	virtual void OnUpdate(NPT_HttpRequest& request, const NPT_HttpRequestContext& context,NPT_HttpResponse& response){}
	virtual void OnDelete(NPT_HttpRequest& request, const NPT_HttpRequestContext& context,NPT_HttpResponse& response){}
};

class EventingHandler : public IHTTPEventHandler{
public:
	EventingHandler(NPT_String segment):IHTTPEventHandler(segment.GetChars(),SUPPORT_GET){};
private:
	virtual void OnCreate(NPT_HttpRequest& request, const NPT_HttpRequestContext& context,NPT_HttpResponse& response) {}
	virtual void OnRead  (NPT_HttpRequest& request, const NPT_HttpRequestContext& context,NPT_HttpResponse& response){
		log_debug("On A client %s ; Will wait until  get 0  !",this->getSegment().GetChars());
		NPT_TimeStamp ts;
		NPT_System::GetCurrentTimeStamp(ts);
		NPT_DateTime  l_creationDate;
		l_creationDate.FromTimeStamp(ts);
		NPT_String msg =NPT_String::Format("<HTML> <B> !!!  [%s] GOOD AT EVENTING HANDLER ::%s !!! </B> </HTML>",l_creationDate.ToString(NPT_DateTime::FORMAT_W3C).GetChars(),__FUNCTION__);
		NPT_HttpEntity* entity = response.GetEntity();
		entity->SetContentType("text/html");
		entity->SetInputStream(msg);

		log_debug("On A client %s, @@  '%s' ;End of  wait, got 0 !!",this->getSegment().GetChars(),l_creationDate.ToString(NPT_DateTime::FORMAT_W3C).GetChars());
	}
	virtual void OnUpdate(NPT_HttpRequest& request, const NPT_HttpRequestContext& context,NPT_HttpResponse& response){}
	virtual void OnDelete(NPT_HttpRequest& request, const NPT_HttpRequestContext& context,NPT_HttpResponse& response){}
};
class OnReadHandler : public IHTTPHandler{
public:
	OnReadHandler(NPT_String segment):IHTTPHandler(segment.GetChars(),SUPPORT_GET){};
private:
	virtual void OnCreate(NPT_HttpRequest& request, const NPT_HttpRequestContext& context,NPT_HttpResponse& response) {}
	virtual void OnRead  (NPT_HttpRequest& request, const NPT_HttpRequestContext& context,NPT_HttpResponse& response){
		NPT_TimeStamp ts;
		NPT_System::GetCurrentTimeStamp(ts);
		NPT_DateTime  l_creationDate;
		l_creationDate.FromTimeStamp(ts);
		NPT_String msg =NPT_String::Format("<HTML> <B> !!!  [%s] GOOD AT OnReadHandler::%s !!! </B> </HTML>",l_creationDate.ToString(NPT_DateTime::FORMAT_W3C).GetChars(),__FUNCTION__);
		NPT_HttpEntity* entity = response.GetEntity();
		entity->SetContentType("text/html");
		entity->SetInputStream(msg);
		log_debug("On A client %s, @@  '%s' !",this->getSegment().GetChars(),l_creationDate.ToString(NPT_DateTime::FORMAT_W3C).GetChars());
	}
	virtual void OnUpdate(NPT_HttpRequest& request, const NPT_HttpRequestContext& context,NPT_HttpResponse& response){}
	virtual void OnDelete(NPT_HttpRequest& request, const NPT_HttpRequestContext& context,NPT_HttpResponse& response){}
};
class OnUpdateHandler : public IHTTPHandler{
public:
	OnUpdateHandler(NPT_String segment):IHTTPHandler(segment.GetChars(),SUPPORT_POST){};
private:
	virtual void OnCreate(NPT_HttpRequest& request, const NPT_HttpRequestContext& context,NPT_HttpResponse& response) {}
	virtual void OnRead  (NPT_HttpRequest& request, const NPT_HttpRequestContext& context,NPT_HttpResponse& response){}
	virtual void OnUpdate(NPT_HttpRequest& request, const NPT_HttpRequestContext& context,NPT_HttpResponse& response){
		NPT_TimeStamp ts;
		NPT_System::GetCurrentTimeStamp(ts);
		NPT_DateTime  l_creationDate;
		l_creationDate.FromTimeStamp(ts);
		NPT_String msg =NPT_String::Format("<HTML> <B> !!!  [%s] GOOD AT OnUpdateHandler::%s !!! </B> </HTML>",l_creationDate.ToString(NPT_DateTime::FORMAT_W3C).GetChars(),__FUNCTION__);
		NPT_HttpEntity* entity = response.GetEntity();
		entity->SetContentType("text/html");
		entity->SetInputStream(msg);
		log_debug("On A client %s, @@  '%s' !",this->getSegment().GetChars(),l_creationDate.ToString(NPT_DateTime::FORMAT_W3C).GetChars());
	}
	virtual void OnDelete(NPT_HttpRequest& request, const NPT_HttpRequestContext& context,NPT_HttpResponse& response){}
};
class OnDeleteHandler : public IHTTPHandler{
public:
	OnDeleteHandler(NPT_String segment):IHTTPHandler(segment.GetChars(),SUPPORT_DELETE){};
private:
	virtual void OnCreate(NPT_HttpRequest& request, const NPT_HttpRequestContext& context,NPT_HttpResponse& response) {}
	virtual void OnRead  (NPT_HttpRequest& request, const NPT_HttpRequestContext& context,NPT_HttpResponse& response){}
	virtual void OnUpdate(NPT_HttpRequest& request, const NPT_HttpRequestContext& context,NPT_HttpResponse& response){}
	virtual void OnDelete(NPT_HttpRequest& request, const NPT_HttpRequestContext& context,NPT_HttpResponse& response){
		NPT_TimeStamp ts;
		NPT_System::GetCurrentTimeStamp(ts);
		NPT_DateTime  l_creationDate;
		l_creationDate.FromTimeStamp(ts);
		NPT_String msg =NPT_String::Format("<HTML> <B> !!!  [%s] GOOD AT OnDeleteHandler::%s !!! </B> </HTML>",l_creationDate.ToString(NPT_DateTime::FORMAT_W3C).GetChars(),__FUNCTION__);
		NPT_HttpEntity* entity = response.GetEntity();
		entity->SetContentType("text/html");
		entity->SetInputStream(msg);
		log_debug("On A client %s, @@  '%s' !",this->getSegment().GetChars(),l_creationDate.ToString(NPT_DateTime::FORMAT_W3C).GetChars());
	}
};
class OnAllHandler : public IHTTPHandler{
public:
	OnAllHandler(NPT_String segment):IHTTPHandler(segment.GetChars(),SUPPORT_ALL){};
private:
	virtual void OnCreate(NPT_HttpRequest& request, const NPT_HttpRequestContext& context,NPT_HttpResponse& response){
		NPT_TimeStamp ts;
		NPT_System::GetCurrentTimeStamp(ts);
		NPT_DateTime  l_creationDate;
		l_creationDate.FromTimeStamp(ts);
		NPT_String msg =NPT_String::Format("<HTML> <B> !!!  [%s] GOOD AT OnAllHandler::%s !!! </B> </HTML>",l_creationDate.ToString(NPT_DateTime::FORMAT_W3C).GetChars(),__FUNCTION__);
		NPT_HttpEntity* entity = response.GetEntity();
		entity->SetContentType("text/html");
		entity->SetInputStream(msg);
		log_debug("On A client %s, @@  '%s' !",this->getSegment().GetChars(),l_creationDate.ToString(NPT_DateTime::FORMAT_W3C).GetChars());
	}
 	virtual void OnRead  (NPT_HttpRequest& request, const NPT_HttpRequestContext& context,NPT_HttpResponse& response){
		NPT_TimeStamp ts;
		NPT_System::GetCurrentTimeStamp(ts);
		NPT_DateTime  l_creationDate;
		l_creationDate.FromTimeStamp(ts);
		NPT_String msg =NPT_String::Format("<HTML> <B> !!!  [%s] GOOD AT OnAllHandler::%s !!! </B> </HTML>",l_creationDate.ToString(NPT_DateTime::FORMAT_W3C).GetChars(),__FUNCTION__);
		NPT_HttpEntity* entity = response.GetEntity();
		entity->SetContentType("text/html");
		entity->SetInputStream(msg);
		log_debug("On A client %s, @@  '%s' !",this->getSegment().GetChars(),l_creationDate.ToString(NPT_DateTime::FORMAT_W3C).GetChars());
 	}
	virtual void OnUpdate(NPT_HttpRequest& request, const NPT_HttpRequestContext& context,NPT_HttpResponse& response){
		NPT_TimeStamp ts;
		NPT_System::GetCurrentTimeStamp(ts);
		NPT_DateTime  l_creationDate;
		l_creationDate.FromTimeStamp(ts);
		NPT_String msg =NPT_String::Format("<HTML> <B> !!!  [%s] GOOD AT OnAllHandler::%s !!! </B> </HTML>",l_creationDate.ToString(NPT_DateTime::FORMAT_W3C).GetChars(),__FUNCTION__);
		NPT_HttpEntity* entity = response.GetEntity();
		entity->SetContentType("text/html");
		entity->SetInputStream(msg);
		log_debug("On A client %s, @@  '%s' !",this->getSegment().GetChars(),l_creationDate.ToString(NPT_DateTime::FORMAT_W3C).GetChars());
	}
	virtual void OnDelete(NPT_HttpRequest& request, const NPT_HttpRequestContext& context,NPT_HttpResponse& response){
		NPT_TimeStamp ts;
		NPT_System::GetCurrentTimeStamp(ts);
		NPT_DateTime  l_creationDate;
		l_creationDate.FromTimeStamp(ts);
		NPT_String msg =NPT_String::Format("<HTML> <B> !!!  [%s] GOOD AT OnAllHandler::%s !!! </B> </HTML>",l_creationDate.ToString(NPT_DateTime::FORMAT_W3C).GetChars(),__FUNCTION__);
		NPT_HttpEntity* entity = response.GetEntity();
		entity->SetContentType("text/html");
		entity->SetInputStream(msg);
		log_debug("On A client %s, @@  '%s' !",this->getSegment().GetChars(),l_creationDate.ToString(NPT_DateTime::FORMAT_W3C).GetChars());
	}
};


#endif /* HTTPSERVERTESTHANDLERS_H_ */
