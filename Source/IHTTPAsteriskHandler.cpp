/**
 *  \copyright Copyright (c) 2011, 2012 All Right Reserved
 *  \brief implementation of the Asterisk Http Handler
 *  \file  IHTTPAsteriskHandler.h
 *  \date 11 juin 2013
 *  \author Azzouni Mohamed
*/

#include "IHTTPAsteriskHandler.h"

#include "Neptune.h"
NPT_SET_LOCAL_LOGGER("cmop.server.handler.asterisk")

IHTTPAsteriskHandler::IHTTPAsteriskHandler(METHODS methodsSupportMask)
:IHTTPHandler("*",methodsSupportMask) {
	m_methodsSupportMask = methodsSupportMask;
	m_segment = NPT_String("*");
	m_delta = NPT_String("");
}

IHTTPAsteriskHandler::~IHTTPAsteriskHandler() {
	// TODO Auto-generated destructor stub
}


bool IHTTPAsteriskHandler::operator ==(const NPT_String& other) {
	NPT_LOG_FINE_1( "Fetching on Tree : segment <<<%s::*>>  on Asterisk Handler !",other.GetChars());
	this->m_delta.Assign(other.GetChars(),other.GetLength());
	return true ; // when comparison come to this handler , its always true
}

NPT_String& IHTTPAsteriskHandler::getDelta() {
	return m_delta;
}
