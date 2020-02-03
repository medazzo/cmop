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

const cmop::IpAddress cmop::IpAddress::Any = cmop::IpAddress();
const cmop::IpAddress cmop::IpAddress::Loopback = cmop::IpAddress(127, 0, 0, 1);

/*----------------------------------------------------------------------
|   cmop::IpAddress::IpAddress
+---------------------------------------------------------------------*/
cmop::IpAddress::IpAddress() : m_Address(0) {}

/*----------------------------------------------------------------------
|   cmop::IpAddress::IpAddress
+---------------------------------------------------------------------*/
cmop::IpAddress::IpAddress(unsigned long address) : m_Address(address) {}

/*----------------------------------------------------------------------
|   cmop::IpAddress::IpAddress
+---------------------------------------------------------------------*/
cmop::IpAddress::IpAddress(unsigned char a,
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
|   cmop::Server::Server
+---------------------------------------------------------------------*/
cmop::Server::Server(cmop::IpAddress listen_address,
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
|   cmop::Server::~Server
+---------------------------------------------------------------------*/
cmop::Server::~Server()
{
	m_server->Stop();
	delete m_server;
	NPT_LOG_INFO("Server Stopped and deleted ");
}

/*----------------------------------------------------------------------
|   cmop::Server::Stop
+---------------------------------------------------------------------*/
cmop::Result cmop::Server::Stop()
{
	return m_server->Stop();
}

/*----------------------------------------------------------------------
|   cmop::Server::setRoot
+---------------------------------------------------------------------*/
void cmop::Server::setRoot( cmop::IHTTPHandler * treeHandler)
{
	m_server->setTreeHandler( new HTTPTree(treeHandler));
}
