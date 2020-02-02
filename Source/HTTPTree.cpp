/**
 *  \copyright Copyright (c) 2011, 2012 All Right Reserved
 *  \brief implementation de Http Server Tree
 *  \file  HTTPTree.cpp
 *  \date 11 juin 2013
 *  \author Azzouni Mohamed
*/

#include "HTTPUtility.h"
#include "HTTPTree.h"

HTTPTree::HTTPTree()
{
	m_root = NULL;
}
HTTPTree::HTTPTree(IHTTPHandler *content)
{
	m_root = new HTTPNode(content);
}
HTTPTree::HTTPNode *
HTTPTree::getRoot()
{
	return m_root;
}
HTTPTree::~HTTPTree()
{
	delete m_root;
}
void HTTPTree::setRoot(IHTTPHandler *root)
{
	m_root = new HTTPNode(root);
}
bool HTTPTree::FindChildNodeOnTree(NPT_List<NPT_String> Segments,
								   IHTTPHandler *&found)
{
	log_debug("Fetching on Tree !");
	HTTPNode *t = m_root;
	NPT_List<NPT_String>::Iterator it = Segments.GetFirstItem();
	NPT_String segment = *it;
	bool fd = false;
	int count = 0;

	if (t->operator==(segment))
	{
		log_debug("Fetching on Tree : segment[%d]: '%s'  found >>  on root !", count, segment.GetChars());
		fd = true;
		it++;
	}
	else
	{
		log_error("Fetching on Tree : segment[%d]: '%s' NOT found >>  on root !", count, segment.GetChars());
		return false;
	}

	while (it)
	{
		count++;
		segment = *it;
		log_debug("Fetching on Tree for segment[%d]: '%s' !", count, segment.GetChars());
		{
			log_debug("Fetching on Tree : segment[%d]: '%s'  searching on children of '%s' !",
					  count, segment.GetChars(), t->getNode()->getSegment().GetChars());
			fd = t->FindSegmentChildNode(segment, t);
			if (fd)
			{
				log_debug("Fetching on Tree : segment[%d]: '%s'  found >>  on a children !", count, segment.GetChars());
				found = t->m_node;
				fd = true;
				it++;
			}
			else
			{
				fd = false;
				it++;
				return fd;
			}
		}
	}
	return fd;
}

HTTPTree::HTTPNode::~HTTPNode()
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
HTTPTree::HTTPNode::HTTPNode()
{
	m_node = NULL;
	m_childrens = new NPT_List<HTTPNode *>();
}
HTTPTree::HTTPNode::HTTPNode(IHTTPHandler *node)
{
	m_node = node;
	m_childrens = new NPT_List<HTTPNode *>();
}
bool HTTPTree::HTTPNode::operator==(const HTTPNode &other)
{
	return (m_node == other.m_node) ? true : false;
}
bool HTTPTree::HTTPNode::operator==(const HTTPNode *other)
{
	return (m_node == other->m_node) ? true : false;
}
bool HTTPTree::HTTPNode::operator==(const NPT_String &other)
{
	return (*(m_node) == other) ? true : false;
}
HTTPTree::HTTPNode *
HTTPTree::HTTPNode::AddChildNode(IHTTPHandler *child)
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
		log_error(" NULL Handle : will not be Added in Tree !");
	}
	return ch;
}
HTTPTree::HTTPNode *
HTTPTree::HTTPNode::getChildNode(int nIndex)
{
	HTTPNode *t = NULL;
	m_childrens->Get(nIndex, t);
	return t;
}
NPT_Cardinal HTTPTree::HTTPNode::getChildCount()
{
	return m_childrens->GetItemCount();
}
bool HTTPTree::HTTPNode::FindSegmentChildNode(NPT_String segment,
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
IHTTPHandler *
HTTPTree::HTTPNode::getNode()
{
	return m_node;
}
