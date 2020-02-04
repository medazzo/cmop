/**
 *  \copyright Copyright (c) 2011, 2012 All Right Reserved
 *  \brief implementation de Http Node Tree
 *  \file  HTTPNode.h
 *  \date 11 juin 2013
 *  \author Azzouni Mohamed
*/

#ifndef HTTPNODE_H_
#define HTTPNODE_H_

#include "Neptune.h"
#include "CMopServer.h"

namespace cmop
{

/**
 * \class HTTPNode
 * \brief The composant of the tree .
*/
class  HTTPNode /*: public NPT_HttpRequestHandler*/
{
public:
	/**
	 * \brief destructor of HTTPNode
	*/
	~HTTPNode();

	/**
	 * \brief   instantiate a Http Node on Http Server Tree
	*/
	HTTPNode();

	/**
	 * \brief   instantiate a Http Node on Http Server Tree
	 * \param   node :the content of the node type of IHTTP_Handler
	*/
	HTTPNode(IHTTPHandler *node);

	/**
	 * \brief   operator == used to compare
	 * \param   other :the other Http Node
	 * \return  true if segment on two node is equal else false.
	*/
	bool operator==(const HTTPNode& other);

	/**
	 * \brief   operator == used to compare
	 * \param   other : the other Http Node
	 * \return  true if segment on two node is equal else false.
	*/
	bool operator==(const HTTPNode* other);

	/**
	 * \brief   operator == used to compare
	 * \param   other :the other Http Node
	 * \return  true if segment on two node is equal else false.
	*/
	bool operator==(const ::NPT_String& other);


	/**
	 * \brief   Add children node on this Node
	 * \param   child :the content of the node type of IHTTP_Handler
	 * \return  pointer on the created node else NULL.
	*/
	HTTPNode * AddChildNode(IHTTPHandler *child);

	/**
	 * \brief   get the children node at index
	 * \param   nIndex : children node  index
	 * \return  pointer on the child node else NULL.
	*/
	HTTPNode* getChildNode(int nIndex);

	/**
	 * \brief   get the children count
	 * \return  the children nodes count
	*/
	NPT_Cardinal getChildCount();

	/**
	 * \brief   search segment on children nodes
	 * \param   segment :searched segment
	 * \param   found pointer on node , Update it with pointer if found
	 * \return  True if segment is found and found id updated else False.
	*/
	bool FindSegmentChildNode(::NPT_String segment, HTTPNode * &found)	;

	/**
	 * \brief   get the node content pointer type of IHTTP_Handler
	 * \return  pointer to the content of this node
	*/
	IHTTPHandler* getNode();

	/**
	 * \brief  used to set the Response : see  NPT_HttpRequestHandler::SetupResponse
	 * \param   request : the client query
	 * \param   context : the request http context.
	 * \param   response : the response
	 * \return  NPT_SUCCES else Neptune code error .
	*/
    NPT_Result SetupResponse(::NPT_HttpRequest&              request,
                             const ::NPT_HttpRequestContext& context,
                             ::NPT_HttpResponse&             response);

public:
	/** \brief   pointer to the content of the node type of IHTTP_Handler*/
	IHTTPHandler *m_node;
	/** \brief   lits of pointer of children of this node. */
	::NPT_List<HTTPNode *> *m_childrens;
};


}
#endif /* HTTPNODE_H_ */
