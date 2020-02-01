/**
 *  \copyright Copyright (c) 2011, 2012 All Right Reserved
 *  \brief implementation de Http Server Tree
 *  \file  HTTPTree.h
 *  \date 11 juin 2013
 *  \author Azzouni Mohamed
*/

#ifndef HTTPTREE_H_
#define HTTPTREE_H_

#include "HTTPUtility.h"
#include "IHTTPHandler.h"

/**
 * \class HTTPTree
 * \brief The Http server Tree of IHTTP handler used for REST implementation.
*/
class  HTTPTree{
public:
	/**
	 * \class HTTPNode
	 * \brief The composant of the tree .
	*/
	class  HTTPNode{
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
		bool operator==(const NPT_String& other);


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
		bool FindSegmentChildNode(NPT_String segment,HTTPNode * &found)	;

		/**
		 * \brief   get the node content pointer type of IHTTP_Handler
		 * \return  pointer to the content of this node
		*/
		IHTTPHandler* getNode();

	public:
		/** \brief   pointer to the content of the node type of IHTTP_Handler*/
		IHTTPHandler *m_node;
		/** \brief   lits of pointer of children of this node. */
		NPT_List<HTTPNode *> *m_childrens;
	};

public:
	/**
	 * \brief   default constructor of the tree
	*/
	HTTPTree();

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
	~HTTPTree();

	/**
	 * \brief   search segments list on tree
	 * \param   Segments : segments List
	 * \param   found pointer on node , Update it with pointer if found
	 * \return  True if segment is found and found id updated else False.
	*/
	bool FindChildNodeOnTree(NPT_List<NPT_String> Segments,IHTTPHandler* &found);

	/**
	 * \brief   Set the root of the tree
	 * \param   root : the content of the root node of tree
	*/
	void setRoot(IHTTPHandler* root);

private:
	 /** \brief   the node root of the tree */
	HTTPNode* m_root;
};

#endif /* HTTPTREE_H_ */
