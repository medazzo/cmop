/**
 *  \copyright Copyright (c) 2011, 2012 All Right Reserved
 *  \brief implementation de Http Server Tree
 *  \file  HTTPTree.h
 *  \date 11 juin 2013
 *  \author Azzouni Mohamed
*/

#ifndef HTTPTREE_H_
#define HTTPTREE_H_

#include "Neptune.h"

#include "HTTPNode.h"
#include "CMopServer.h"

namespace cmop
{

/**
 * \class HTTPTree
 * \brief The Http server Tree of IHTTP handler used for REST implementation.
*/
class  HTTPTree{
public:
	/**
	 * \brief   constructor of the tree
	 * \param   content : the content of the root node
	*/
	HTTPTree(IHTTPHandler* content);

	/**
	 * \brief   gettter for the root node of the tree
	 * \return  pointer on the root Http Node
	*/
	HTTPNode *getRoot();

	/**
	 * \brief default destructor of the tree
	*/
	virtual ~HTTPTree();

	/**
	 * \brief   search segments list on tree
	 * \param   Segments : segments List
	 * \param   found pointer on node , Update it with pointer if found
	 * \return  True if segment is found and found id updated else False.
	*/
	bool FindChildNodeOnTree(NPT_List<NPT_String> Segments, HTTPNode* &found);

private:
	 /** \brief   the node root of the tree */
	HTTPNode* m_root;
};
} /* namespace cmop */
#endif /* HTTPTREE_H_ */
