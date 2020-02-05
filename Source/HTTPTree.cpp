/**
 *  \copyright Copyright (c) 2011, 2012 All Right Reserved
 *  \brief implementation de Http Server Tree
 *  \file  HTTPTree.cpp
 *  \date 11 juin 2013
 *  \author Azzouni Mohamed
*/

#include "HTTPTree.h"
#include "HTTPNode.h"
#include "Neptune.h"

NPT_SET_LOCAL_LOGGER("cmop.server.tree")

namespace cmop
{
/*----------------------------------------------------------------------
|   HTTPTree::~HTTPTree
+---------------------------------------------------------------------*/
HTTPTree::~HTTPTree()
{
	delete m_root;
}

/*----------------------------------------------------------------------
|   HTTPTree::HTTPTree
+---------------------------------------------------------------------*/
HTTPTree::HTTPTree(IHTTPHandler *content)
{
	m_root = new HTTPNode(content);
}

/*----------------------------------------------------------------------
|   HTTPTree::~getRoot
+---------------------------------------------------------------------*/
HTTPNode * HTTPTree::getRoot()
{
	return m_root;
}

/*----------------------------------------------------------------------
|   HTTPTree::FindChildNodeOnTree
+---------------------------------------------------------------------*/
bool HTTPTree::FindChildNodeOnTree(NPT_List<NPT_String> Segments,HTTPNode* &found )
{
	NPT_LOG_INFO("Fetching on Tree !");
	HTTPNode *t = m_root;
	::NPT_List<NPT_String>::Iterator it = Segments.GetFirstItem();
	::NPT_String segment = *it;
	bool fd = false;
	int count = 0;

	if (t->operator==(segment))
	{
		NPT_LOG_FINE_2( "Fetching on Tree : segment[%d]: '%s'  found >>  on root !", count, segment.GetChars());
		found = t;
		fd = true;
		it++;
	}
	else
	{
		NPT_LOG_SEVERE_2("Fetching on Tree : segment[%d]: '%s' NOT found >>  on root !", count, segment.GetChars());
		return false;
	}

	while (it)
	{
		count++;
		segment = *it;
		NPT_LOG_FINE_2( "Fetching on Tree for segment[%d]: '%s' !", count, segment.GetChars());
		NPT_LOG_INFO_3("Fetching on Tree : segment[%d]: '%s'  searching on children of '%s' !",
				  count, segment.GetChars(), t->getNodeHandler()->getSegment() );
		fd = t->FindSegmentChildNode(segment, t);
		if (fd)
		{
			NPT_LOG_FINE_2( "Fetching on Tree : segment[%d]: '%s'  found >>  on a children !", count, segment.GetChars());
			found = t;
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
	return fd;
}
}
