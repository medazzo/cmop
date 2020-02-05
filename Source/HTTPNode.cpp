/**
 *  \copyright Copyright (c) 2011, 2012 All Right Reserved
 *  \brief implementation de Http Server Tree
 *  \file  HTTPTree.cpp
 *  \date 11 juin 2013
 *  \author Azzouni Mohamed
*/

#include "Neptune.h"

#include "HTTPNode.h"
#include "HTTPUtility.h"
NPT_SET_LOCAL_LOGGER("cmop.server.node")
namespace cmop
{

/*----------------------------------------------------------------------
|   HTTPNode::~HTTPNode
+---------------------------------------------------------------------*/
HTTPNode::~HTTPNode()
{
	if (m_node)
		delete m_node;
	HTTPNode *t = NULL;
	for (NPT_Cardinal i = 0; i < m_childrens->GetItemCount(); i++)
	{
		m_childrens->Get(i, t);
		if (t)
		{
			delete t;
		}
	}
	m_childrens->Clear();
	delete m_childrens;
}

/*----------------------------------------------------------------------
|   HTTPNode::HTTPNode
+---------------------------------------------------------------------*/
HTTPNode::HTTPNode()
{
	m_node = NULL;
	m_childrens = new NPT_List<HTTPNode *>();
}

/*----------------------------------------------------------------------
|   HTTPNode::HTTPNode
+---------------------------------------------------------------------*/
HTTPNode::HTTPNode(IHTTPHandler *node)
{
	m_node = node;
	m_childrens = new NPT_List<HTTPNode *>();
}

/*----------------------------------------------------------------------
|   HTTPNode::operator==
+---------------------------------------------------------------------*/
bool HTTPNode::operator==(const HTTPNode &other)
{
	return (m_node == other.m_node) ? true : false;
}

/*----------------------------------------------------------------------
|   HTTPNode::operator==
+---------------------------------------------------------------------*/
bool HTTPNode::operator==(const HTTPNode *other)
{
	if (other== NULL || other->m_node == NULL)
		return false ;
	IHTTPHandler* hd = other->m_node;
	const char* oth = (const char*)hd->getSegment();
	return (NPT_String(m_node->getSegment()).Compare(oth, false) == 0 ) ? true : false;
}

/*----------------------------------------------------------------------
|   HTTPNode::operator==
+---------------------------------------------------------------------*/
bool HTTPNode::operator==(const NPT_String &other)
{
	return (NPT_String(m_node->getSegment()).Compare(other.GetChars()) == 0 ) ? true : false;
}

/*----------------------------------------------------------------------
|   HTTPNode::AddChildNode
+---------------------------------------------------------------------*/
HTTPNode * HTTPNode::AddChildNode(IHTTPHandler *child)
{
	HTTPNode *ch = new HTTPNode(child);
	if (child != NULL)
	{
		if (child->GetMyHandlerType() == HANDLER_ASTERISK)
			m_childrens->Insert(m_childrens->GetLastItem(), ch);
		else
			m_childrens->Insert(m_childrens->GetFirstItem(), ch);
	}
	else
	{
		NPT_LOG_FATAL(" NULL Handle : will not be Added in Tree !");
	}
	return ch;
}

/*----------------------------------------------------------------------
|   HTTPNode::getChildNode
+---------------------------------------------------------------------*/
HTTPNode * HTTPNode::getChildNode(int nIndex)
{
	HTTPNode *t = NULL;
	m_childrens->Get(nIndex, t);
	return t;
}
NPT_Cardinal HTTPNode::getChildCount()
{
	return m_childrens->GetItemCount();
}

/*----------------------------------------------------------------------
|   HTTPNode::SetupResponse
+---------------------------------------------------------------------*/
NPT_Result HTTPNode::SetupResponse(::NPT_HttpRequest&              request,
                         const ::NPT_HttpRequestContext& context,
                         ::NPT_HttpResponse&             response)
{
	if ((request.GetMethod().Compare(NPT_HTTP_METHOD_GET) == 0) && HTTPUtility::check_support(this->m_node->getMethods(), SUPPORT_GET))
	{
		this->m_node->OnRead(request, context, response);
		return NPT_SUCCESS;
	}
	else if ((request.GetMethod().Compare(NPT_HTTP_METHOD_POST) == 0) && HTTPUtility::check_support(this->m_node->getMethods(), SUPPORT_POST))
	{
		this->m_node->OnUpdate(request, context, response);
		return NPT_SUCCESS;
	}
	else if ((request.GetMethod().Compare(NPT_HTTP_METHOD_PUT) == 0) && HTTPUtility::check_support(this->m_node->getMethods(), SUPPORT_PUT))
	{
		this->m_node->OnCreate(request, context, response);
		return NPT_SUCCESS;
	}
	else if ((request.GetMethod().Compare(NPT_HTTP_METHOD_DELETE) == 0) && HTTPUtility::check_support(this->m_node->getMethods(), SUPPORT_DELETE))
	{
		this->m_node->OnDelete(request, context, response);
		return NPT_SUCCESS;
	}
	else
	{
		NPT_LOG_FATAL_1( "## Error : NOT Supported Method Call : %s !",
				  request.GetMethod().GetChars());
		return NPT_ERROR_NOT_SUPPORTED;
	}
}

/*----------------------------------------------------------------------
|   HTTPNode::FindSegmentChildNode
+---------------------------------------------------------------------*/
bool HTTPNode::FindSegmentChildNode(NPT_String segment,
											  HTTPNode *&found)
{
	HTTPNode *t = NULL;
	for (NPT_Cardinal i = 0; i < m_childrens->GetItemCount(); i++)
	{
		m_childrens->Get(i, t);
		if (t->operator==(segment))
		{
			found = t;
			return true;
		}
	}
	return false;
}

/*----------------------------------------------------------------------
|   IHTTPHandler::getNode
+---------------------------------------------------------------------*/
IHTTPHandler * HTTPNode::getNodeHandler()
{
	return m_node;
}
}
