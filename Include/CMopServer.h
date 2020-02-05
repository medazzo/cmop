/**
 *  \copyright Copyright (c) 2011, 2012 All Right Reserved
 *  \brief implementation de Http Server
 *  \file  HTTPServer.h
 *  \date 11 juin 2013
 *  \author Azzouni Mohamed
*/

#ifndef CMOPRESTSERVER_H_
#define CMOPRESTSERVER_H_

class NPT_HttpRequest;
class NPT_HttpRequestContext;
class NPT_HttpResponse;

namespace cmop
{
/*!< define default HTTP MACx Segment Length     */
const int HTTP_MAX_SEGMENT_LENGTH = 256;
/**
 * \enum METHODS
 * \brief Contains the Value of the Http Method that can be supported by different types of handlers
*/
typedef enum {
SUPPORT_NONE 	= 0x0,		/*!< Do not support Any Methods. */
SUPPORT_PUT   	= 0x1000,	/*!< support for PUT Method. */
SUPPORT_GET   	= 0x0100,	/*!< support for GET Method. */
SUPPORT_POST   	= 0x0010,	/*!< support for POST Method. */
SUPPORT_DELETE  = 0x0001,	/*!< support for DELETE Method. */
SUPPORT_ALL     = SUPPORT_PUT|SUPPORT_GET|SUPPORT_POST|SUPPORT_DELETE	/*!< support for ALL Method. */
}METHODS;

/**
 * \enum HANDLERSTYPES
 * \brief Contains the Value of theHandlers Types : Static , Eventing or Asterisk .
*/
typedef enum  {
HADNLER_STATIC 		= 0x0,  	/*!< define Static Handler */
HANDLER_EVENT  		= 0x1000,   /*!< define the Events Handler. */
HANDLER_ASTERISK   	= 0x0100 	/*!< define the Asterisk Handler. */
}HANDLERSTYPES;


/**
  * \brief enumeration that define results
*/
typedef enum
{
   CMOP_SUCCESS   = 0,
   CMOP_ERROR     ,
   CMOP_UNKNOWN_RESULT,
   CMOP_NO_SUCH_ITEM_ERROR,
   CMOP_NOTDEFINED_RESULT
}Result;
/**
 * \class CMOP IpAddress
 * \brief The Ip Address Class .
*/
class IpAddress
{
public:
	// class members
	static IpAddress Any;
	static IpAddress Loopback;

	// constructors
	IpAddress():m_Address(0) {};
	IpAddress(unsigned long address):m_Address(address) {};
	IpAddress(unsigned char a, unsigned char b, unsigned char c, unsigned char d){
		m_Address = (((unsigned long)a) << 24) |
					(((unsigned long)b) << 16) |
					(((unsigned long)c) << 8) |
					(((unsigned long)d));
	};

    // members
    unsigned long m_Address;
};


/**
 * \class IHTTPHandler
 * \brief The interface of static HTTP Handler .
*/
class IHTTPHandler
{
public:

	/**
	 * \brief   instantiate a static HTTP Handler.
	 * \param   segment : The String segment responsible for this handler .
	 * \param   methodsSupportMask The mask containing supported Http Method
	*/
	IHTTPHandler(    char * segment,
                     METHODS methodsSupportMask = SUPPORT_NONE);
	/**
	 * \brief   Get the Handler Type.
	 * \return  the Handler Type :HANDLER_ASTERISK.
	*/
	virtual HANDLERSTYPES GetMyHandlerType() = 0;
	/**
	 * \brief destructor of IHTTP Handler
	*/
	virtual ~IHTTPHandler();

	/**
	 * \brief  this function is called when a PUT request is arrived and when PUT is enabled .
	 * \param   request : the client query
	 * \param   context : the request http context.
	 * \param   response : the response
	 * \return  NPT_SUCCES else Neptune code error .
	*/
	virtual void OnCreate(::NPT_HttpRequest &request,
						  const ::NPT_HttpRequestContext &context,
						  ::NPT_HttpResponse &response) ;

	/**
	 * \brief  this function is called when a GET request is arrived and when GET is enabled .
	 * \param   request : the client query
	 * \param   context : the request http context.
	 * \param   response : the response
	 * \return  NPT_SUCCES else Neptune code error .
	*/
	virtual void OnRead(::NPT_HttpRequest &request,
						const ::NPT_HttpRequestContext &context,
						::NPT_HttpResponse &response) ;

	/**
	 * \brief  this function is called when a POST request is arrived and when POST is enabled .
	 * \param   request : the client query
	 * \param   context : the request http context.
	 * \param   response : the response
	 * \return  NPT_SUCCES else Neptune code error .
	*/
	virtual void OnUpdate(::NPT_HttpRequest &request,
						  const ::NPT_HttpRequestContext &context,
						  ::NPT_HttpResponse &response) ;

	/**
	 * \brief  this function is called when a DELETE request is arrived and when DELETE is enabled .
	 * \param   request : the client query
	 * \param   context : the request http context.
	 * \param   response : the response
	 * \return  NPT_SUCCES else Neptune code error .
	*/
	virtual void OnDelete(::NPT_HttpRequest &request,
						  const ::NPT_HttpRequestContext &context,
						  ::NPT_HttpResponse &response) ;

	/**
	 * \brief get the Handler segment.
	 * \return  char * segment of the handler.
	*/
	virtual char * getSegment();
	/**
	 * \brief get the HTTP supported Methods of the handler.
	 * \return  METHODS enum type.
	*/
	METHODS getMethods();


	/**
	 * \brief  used to serve file to the client
	 * \param   request : the client query
	 * \param   context : the request http context.
	 * \param   response : the response
	 * \param   file_path: the file path to be served
	 * \param   mime_type: the file mime_type
	 * \return  NPT_SUCCES else Neptune code error .
	*/
	Result ServeFile( const ::NPT_HttpRequest&        request,
                            const ::NPT_HttpRequestContext& context,
							::NPT_HttpResponse&             response,
                            const char*                   file_path,
                            const char*  			 	 mime_type);

protected:
	/** \brief the Handler segment. */
	char m_segment[HTTP_MAX_SEGMENT_LENGTH];
	/** \brief  True when HTTP Method POSt is enabled .. */
	METHODS m_methodsSupportMask;
};


/**
 * \class ICServer
 * \brief The interface of Cmop Server .
*/
class ICServer
{
	/**
	 * \brief   Start a server
	*/
	virtual Result StartServer() = 0 ;


	/**
	 * \brief  Stop call: used to Stop Server from listen to new clients,
	 * and all working threads .
	 * \return  CMOP_Result else CMOP code error .
	*/
	virtual Result Stop() = 0 ;

	/**
	 * \brief Set the root Handler to be used by the server.
	 * \param  treeHandler the  Tree pointer to be settled.
	*/
	virtual Result setRoot( IHTTPHandler *treeHandler) = 0 ;
};
/**
 * \class CMOP Server Factory
 * \brief The Factory Class .
*/
class CSFactory
{
public:
	/**
	 * \brief   Start a server at address:port and using max threads workers.
	 * \param   listen_address  ip_adress to listen on for clients
	 * \param   listen_port  port to listen on for clients , default HTTP_SERVER_PORT_NUMBER
	 * \param   max_threads_workers : maximum numbers of thread workers : HTTP_DEFAULT_MAX_THREADS_WORKERS
	*/
	ICServer * getCServer(IpAddress listen_address,
			   unsigned short listen_port,
			   unsigned short max_threads_workers);

public:
	/**
	 * \brief Return the factory singleton.
	 */
    static CSFactory& Instance();

private:
	CSFactory();
	~CSFactory();
    static CSFactory m_instance;

};

} // namespace cmop
#endif /* CMOPRESTSERVER_H_ */
