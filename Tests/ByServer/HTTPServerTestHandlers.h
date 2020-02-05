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

using namespace cmop;

#define Test_UNUSED(x)	 (void)(x)

/*
 +---------------------------------------------------------------------
 |       TestHttp
 +---------------------------------------------------------------------
 */
class RootHandler : public IHTTPHandler {
 public:
  RootHandler(const char *segment)
      :
      IHTTPHandler(segment, SUPPORT_NONE) {
  }
  ;
  HANDLERSTYPES GetMyHandlerType() {
    return HANDLER_STATIC;
  }
};

class AstrixHandler : public IHTTPHandler {
 public:
  AstrixHandler()
      :
      IHTTPHandler(SUPPORT_GET) {
  }
  ;
  HANDLERSTYPES GetMyHandlerType() {
    return HANDLER_ASTERISK;
  }

  virtual void OnRead(NPT_HttpRequest &request,
                      const NPT_HttpRequestContext &context,
                      NPT_HttpResponse &response);
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
class OnCreateHandler : public IHTTPHandler {
 public:
  OnCreateHandler(const char *segment)
      :
      IHTTPHandler(segment, SUPPORT_PUT) {
  }
  ;
  HANDLERSTYPES GetMyHandlerType() {
    return HANDLER_STATIC;
  }
 private:
  virtual void OnCreate(NPT_HttpRequest &request,
                        const NPT_HttpRequestContext &context,
                        NPT_HttpResponse &response);
};
class ErrorHandler : public IHTTPHandler {
 public:
  ErrorHandler(const char *segment)
      :
      IHTTPHandler(segment, SUPPORT_PUT) {
  }
  ;
  HANDLERSTYPES GetMyHandlerType() {
    return HANDLER_STATIC;
  }
 private:
  virtual void OnRead(NPT_HttpRequest &request,
                      const NPT_HttpRequestContext &context,
                      NPT_HttpResponse &response);
};
/*
 class EventingHandler : public IHTTPEventHandler{
 public:
 EventingHandler(char * segment):IHTTPEventHandler(segment,SUPPORT_GET){};
 HANDLERSTYPES GetMyHandlerType() { return HANDLER_ASTERISK; }
 private:

 virtual void OnRead  (NPT_HttpRequest& request, const NPT_HttpRequestContext& context,NPT_HttpResponse& response);
 };
 */
class OnReadHandler : public IHTTPHandler {
 public:
  OnReadHandler(const char *segment)
      :
      IHTTPHandler(segment, SUPPORT_GET) {
  }
  ;
  HANDLERSTYPES GetMyHandlerType() {
    return HANDLER_STATIC;
  }
 private:
  virtual void OnRead(NPT_HttpRequest &request,
                      const NPT_HttpRequestContext &context,
                      NPT_HttpResponse &response);
};
class OnUpdateHandler : public IHTTPHandler {
 public:
  OnUpdateHandler(const char *segment)
      :
      IHTTPHandler(segment, SUPPORT_POST) {
  }
  ;
  HANDLERSTYPES GetMyHandlerType() {
    return HANDLER_STATIC;
  }
 private:
  virtual void OnUpdate(NPT_HttpRequest &request,
                        const NPT_HttpRequestContext &context,
                        NPT_HttpResponse &response);
};
class OnDeleteHandler : public IHTTPHandler {
 public:
  OnDeleteHandler(const char *segment)
      :
      IHTTPHandler(segment, SUPPORT_DELETE) {
  }
  ;
  HANDLERSTYPES GetMyHandlerType() {
    return HANDLER_STATIC;
  }
 private:
  virtual void OnDelete(NPT_HttpRequest &request,
                        const NPT_HttpRequestContext &context,
                        NPT_HttpResponse &response);
};
class OnAllHandler : public IHTTPHandler {
 public:
  OnAllHandler(const char *segment)
      :
      IHTTPHandler(segment, SUPPORT_ALL) {
  }
  ;
  HANDLERSTYPES GetMyHandlerType() {
    return HANDLER_STATIC;
  }
 private:
  virtual void OnCreate(NPT_HttpRequest &request,
                        const NPT_HttpRequestContext &context,
                        NPT_HttpResponse &response);
  virtual void OnRead(NPT_HttpRequest &request,
                      const NPT_HttpRequestContext &context,
                      NPT_HttpResponse &response);
  virtual void OnUpdate(NPT_HttpRequest &request,
                        const NPT_HttpRequestContext &context,
                        NPT_HttpResponse &response);
  virtual void OnDelete(NPT_HttpRequest &request,
                        const NPT_HttpRequestContext &context,
                        NPT_HttpResponse &response);
};

#endif /* HTTPSERVERTESTHANDLERS_H_ */
