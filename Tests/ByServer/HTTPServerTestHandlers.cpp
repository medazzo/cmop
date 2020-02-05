/*
 * HTTPServerTestHandlers.h
 *
 *  Created on: 5 juin 2013
 *      Author: azzouni
 */

#include "Neptune.h" // just for logging
#include "HTTPServerTestHandlers.h"

using namespace cmop;

NPT_SET_LOCAL_LOGGER("test.server.handlers")

void AstrixHandler::OnRead(NPT_HttpRequest &request,
                           const NPT_HttpRequestContext &context,
                           NPT_HttpResponse &response) {
  Test_UNUSED(request);
  Test_UNUSED(context);
  Test_UNUSED(response);
  NPT_TimeStamp ts;
  NPT_System::GetCurrentTimeStamp(ts);
  NPT_DateTime l_creationDate;
  l_creationDate.FromTimeStamp(ts);
  NPT_String msg = NPT_String::Format(
      "<HTML> <B> !!!  [%s] GOOD AT OnReadHandler::%s !!! </B> </HTML>",
      l_creationDate.ToString(NPT_DateTime::FORMAT_W3C), __FUNCTION__);
  NPT_HttpEntity *entity = response.GetEntity();
  entity->SetContentType("text/html");
  entity->SetInputStream(msg);
  NPT_LOG_INFO_2( "On A client %s, @@  '%s' !",this->getSegment(),l_creationDate.ToString(NPT_DateTime::FORMAT_W3C).GetChars());

}

void OnCreateHandler::OnCreate(NPT_HttpRequest &request,
                               const NPT_HttpRequestContext &context,
                               NPT_HttpResponse &response) {
  Test_UNUSED(request);
  Test_UNUSED(context);
  Test_UNUSED(response);
  NPT_TimeStamp ts;
  NPT_System::GetCurrentTimeStamp(ts);
  NPT_DateTime l_creationDate;
  l_creationDate.FromTimeStamp(ts);
  NPT_String msg = NPT_String::Format(
      "<HTML> <B> !!!  [%s] GOOD AT OnCreateHandler::%s !!! </B> </HTML>",
      l_creationDate.ToString(NPT_DateTime::FORMAT_W3C), __FUNCTION__);
  NPT_HttpEntity *entity = response.GetEntity();
  entity->SetContentType("text/html");
  entity->SetInputStream(msg);
  NPT_LOG_INFO_2( "On A client %s, @@  '%s' !",this->getSegment(),l_creationDate.ToString(NPT_DateTime::FORMAT_W3C).GetChars());
}

void ErrorHandler::OnRead(NPT_HttpRequest &request,
                          const NPT_HttpRequestContext &context,
                          NPT_HttpResponse &response) {
  Test_UNUSED(request);
  Test_UNUSED(context);
  Test_UNUSED(response);
  NPT_TimeStamp ts;
  NPT_System::GetCurrentTimeStamp(ts);
  NPT_DateTime l_creationDate;
  l_creationDate.FromTimeStamp(ts);
  NPT_String msg = NPT_String::Format(
      "<HTML> <B> !!!  [%s] GOOD AT OnReadHandler::%s !!! </B> </HTML>",
      l_creationDate.ToString(NPT_DateTime::FORMAT_W3C), __FUNCTION__);
  NPT_HttpEntity *entity = response.GetEntity();
  entity->SetContentType("text/html");
  entity->SetInputStream(msg);
  NPT_LOG_INFO_2("On A client %s, @@  '%s' !",this->getSegment(),l_creationDate.ToString(NPT_DateTime::FORMAT_W3C).GetChars());
}

void OnReadHandler::OnRead(NPT_HttpRequest &request,
                           const NPT_HttpRequestContext &context,
                           NPT_HttpResponse &response) {
  Test_UNUSED(request);
  Test_UNUSED(context);
  Test_UNUSED(response);
  NPT_TimeStamp ts;
  NPT_System::GetCurrentTimeStamp(ts);
  NPT_DateTime l_creationDate;
  l_creationDate.FromTimeStamp(ts);
  NPT_String msg = NPT_String::Format(
      "<HTML> <B> !!!  [%s] GOOD AT OnReadHandler::%s !!! </B> </HTML>",
      l_creationDate.ToString(NPT_DateTime::FORMAT_W3C), __FUNCTION__);
  NPT_HttpEntity *entity = response.GetEntity();
  entity->SetContentType("text/html");
  entity->SetInputStream(msg);
  NPT_LOG_INFO_2("On A client %s, @@  '%s' !",this->getSegment(),l_creationDate.ToString(NPT_DateTime::FORMAT_W3C).GetChars());
}

void OnUpdateHandler::OnUpdate(NPT_HttpRequest &request,
                               const NPT_HttpRequestContext &context,
                               NPT_HttpResponse &response) {
  Test_UNUSED(request);
  Test_UNUSED(context);
  Test_UNUSED(response);
  NPT_TimeStamp ts;
  NPT_System::GetCurrentTimeStamp(ts);
  NPT_DateTime l_creationDate;
  l_creationDate.FromTimeStamp(ts);
  NPT_String msg = NPT_String::Format(
      "<HTML> <B> !!!  [%s] GOOD AT OnUpdateHandler::%s !!! </B> </HTML>",
      l_creationDate.ToString(NPT_DateTime::FORMAT_W3C), __FUNCTION__);
  NPT_HttpEntity *entity = response.GetEntity();
  entity->SetContentType("text/html");
  entity->SetInputStream(msg);
  NPT_LOG_INFO_2("On A client %s, @@  '%s' !",this->getSegment(),l_creationDate.ToString(NPT_DateTime::FORMAT_W3C).GetChars());
}

void OnDeleteHandler::OnDelete(NPT_HttpRequest &request,
                               const NPT_HttpRequestContext &context,
                               NPT_HttpResponse &response) {
  Test_UNUSED(request);
  Test_UNUSED(context);
  Test_UNUSED(response);
  NPT_TimeStamp ts;
  NPT_System::GetCurrentTimeStamp(ts);
  NPT_DateTime l_creationDate;
  l_creationDate.FromTimeStamp(ts);
  NPT_String msg = NPT_String::Format(
      "<HTML> <B> !!!  [%s] GOOD AT OnDeleteHandler::%s !!! </B> </HTML>",
      l_creationDate.ToString(NPT_DateTime::FORMAT_W3C), __FUNCTION__);
  NPT_HttpEntity *entity = response.GetEntity();
  entity->SetContentType("text/html");
  entity->SetInputStream(msg);
  NPT_LOG_INFO_2("On A client %s, @@  '%s' !",this->getSegment(),l_creationDate.ToString(NPT_DateTime::FORMAT_W3C).GetChars());
}

void OnAllHandler::OnCreate(NPT_HttpRequest &request,
                            const NPT_HttpRequestContext &context,
                            NPT_HttpResponse &response) {
  Test_UNUSED(request);
  Test_UNUSED(context);
  Test_UNUSED(response);
  NPT_TimeStamp ts;
  NPT_System::GetCurrentTimeStamp(ts);
  NPT_DateTime l_creationDate;
  l_creationDate.FromTimeStamp(ts);
  NPT_String msg = NPT_String::Format(
      "<HTML> <B> !!!  [%s] GOOD AT OnAllHandler::%s !!! </B> </HTML>",
      l_creationDate.ToString(NPT_DateTime::FORMAT_W3C), __FUNCTION__);
  NPT_HttpEntity *entity = response.GetEntity();
  entity->SetContentType("text/html");
  entity->SetInputStream(msg);
  NPT_LOG_INFO_2("On A client %s, @@  '%s' !",this->getSegment(),l_creationDate.ToString(NPT_DateTime::FORMAT_W3C).GetChars());
}
void OnAllHandler::OnRead(NPT_HttpRequest &request,
                          const NPT_HttpRequestContext &context,
                          NPT_HttpResponse &response) {
  Test_UNUSED(request);
  Test_UNUSED(context);
  Test_UNUSED(response);
  NPT_TimeStamp ts;
  NPT_System::GetCurrentTimeStamp(ts);
  NPT_DateTime l_creationDate;
  l_creationDate.FromTimeStamp(ts);
  NPT_String msg = NPT_String::Format(
      "<HTML> <B> !!!  [%s] GOOD AT OnAllHandler::%s !!! </B> </HTML>",
      l_creationDate.ToString(NPT_DateTime::FORMAT_W3C), __FUNCTION__);
  NPT_HttpEntity *entity = response.GetEntity();
  entity->SetContentType("text/html");
  entity->SetInputStream(msg);
  NPT_LOG_INFO_2("On A client %s, @@  '%s' !",this->getSegment(),l_creationDate.ToString(NPT_DateTime::FORMAT_W3C).GetChars());
}
void OnAllHandler::OnUpdate(NPT_HttpRequest &request,
                            const NPT_HttpRequestContext &context,
                            NPT_HttpResponse &response) {
  Test_UNUSED(request);
  Test_UNUSED(context);
  Test_UNUSED(response);
  NPT_TimeStamp ts;
  NPT_System::GetCurrentTimeStamp(ts);
  NPT_DateTime l_creationDate;
  l_creationDate.FromTimeStamp(ts);
  NPT_String msg = NPT_String::Format(
      "<HTML> <B> !!!  [%s] GOOD AT OnAllHandler::%s !!! </B> </HTML>",
      l_creationDate.ToString(NPT_DateTime::FORMAT_W3C), __FUNCTION__);
  NPT_HttpEntity *entity = response.GetEntity();
  entity->SetContentType("text/html");
  entity->SetInputStream(msg);
  NPT_LOG_INFO_2("On A client %s, @@  '%s' !",this->getSegment(),l_creationDate.ToString(NPT_DateTime::FORMAT_W3C).GetChars());
}
void OnAllHandler::OnDelete(NPT_HttpRequest &request,
                            const NPT_HttpRequestContext &context,
                            NPT_HttpResponse &response) {
  Test_UNUSED(request);
  Test_UNUSED(context);
  Test_UNUSED(response);
  NPT_TimeStamp ts;
  NPT_System::GetCurrentTimeStamp(ts);
  NPT_DateTime l_creationDate;
  l_creationDate.FromTimeStamp(ts);
  NPT_String msg = NPT_String::Format(
      "<HTML> <B> !!!  [%s] GOOD AT OnAllHandler::%s !!! </B> </HTML>",
      l_creationDate.ToString(NPT_DateTime::FORMAT_W3C), __FUNCTION__);
  NPT_HttpEntity *entity = response.GetEntity();
  entity->SetContentType("text/html");
  entity->SetInputStream(msg);
  NPT_LOG_INFO_2("On A client %s, @@  '%s' !",this->getSegment(),l_creationDate.ToString(NPT_DateTime::FORMAT_W3C).GetChars());
}

