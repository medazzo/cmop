/**
 *  \copyright Copyright (c) 2011, 2012 All Right Reserved
 *  \brief implementation de Http Server
 *  \file  HTTPServer.h
 *  \date 11 juin 2013
 *  \author Azzouni Mohamed
*/

#ifndef CMOPRESTSERVER_H_
#define CMOPRESTSERVER_H_

namespace cmop
{

class HTTPServer;
class NPT_HttpRequest;
class NPT_HttpRequestContext;
class NPT_HttpResponse;
/**
  * \brief enumeration that define results
*/
typedef enum
{
	CMOP_SUCCESS = 0,
	IPV6
} Result;

/**
 * \class CMOP IpAddress 
 * \brief The Ip Address Class .
*/
class IpAddress
{
public:
	// class members
	static const IpAddress Any;
	static const IpAddress Loopback;

	// constructors and destructor
	IpAddress();	
	IpAddress(unsigned long address);
	IpAddress(unsigned char a, unsigned char b, unsigned char c, unsigned char d);

    // members    
    unsigned long m_Address;
};


/**
 * \class IHTTPHandler
 * \brief The interface of static HTTP Handler .
*/
class IHTTPHandler
{

	/**
	 * \brief  this function is called when a PUT request is arrived and when PUT is enabled .
	 * \param   request : the client query
	 * \param   context : the request http context.
	 * \param   response : the response
	 * \return  NPT_SUCCES else Neptune code error .
	*/
	virtual void OnCreate(NPT_HttpRequest &request,
						  const NPT_HttpRequestContext &context,
						  NPT_HttpResponse &response) = 0;

	/**
	 * \brief  this function is called when a GET request is arrived and when GET is enabled .
	 * \param   request : the client query
	 * \param   context : the request http context.
	 * \param   response : the response
	 * \return  NPT_SUCCES else Neptune code error .
	*/
	virtual void OnRead(NPT_HttpRequest &request,
						const NPT_HttpRequestContext &context,
						NPT_HttpResponse &response) = 0;

	/**
	 * \brief  this function is called when a POST request is arrived and when POST is enabled .
	 * \param   request : the client query
	 * \param   context : the request http context.
	 * \param   response : the response
	 * \return  NPT_SUCCES else Neptune code error .
	*/
	virtual void OnUpdate(NPT_HttpRequest &request,
						  const NPT_HttpRequestContext &context,
						  NPT_HttpResponse &response) = 0;

	/**
	 * \brief  this function is called when a DELETE request is arrived and when DELETE is enabled .
	 * \param   request : the client query
	 * \param   context : the request http context.
	 * \param   response : the response
	 * \return  NPT_SUCCES else Neptune code error .
	*/
	virtual void OnDelete(NPT_HttpRequest &request,
						  const NPT_HttpRequestContext &context,
						  NPT_HttpResponse &response) = 0;
};

/**
 * \class CMOP Server
 * \brief The server Class .
*/
class Server
{
public:
	/**
	 * \brief   instantiate a server without any handler at port and using max threads workers.
	 * \param   listen_address  ip_adress to listen on for clients
	 * \param   listen_port  port to listen on for clients , default HTTP_SERVER_PORT_NUMBER
	 * \param   max_threads_workers : maximum numbers of thread workers : HTTP_DEFAULT_MAX_THREADS_WORKERS
	*/
	Server(IpAddress listen_address,
		   unsigned short listen_port,
		   unsigned short max_threads_workers);

	/**
	 * \brief destructor of Server
	*/
	~Server();

	/**
	 * \brief  Stop call: used to Stop Server from listen to new clients,
	 * and all working threads .
	 * \return  CMOP_Result else CMOP code error .
	*/
	Result Stop();

	/**
	 * \brief Set the root Handler to be used by the server.
	 * \param  treeHandler the  Tree pointer to be settled.
	*/
	void setRoot( cmop::IHTTPHandler *treeHandler);
 
private:
	/** \brief  Tree Handler pointer of the Server .*/
	HTTPServer * m_server;
	/** \brief  The listen address of the Server .*/
	IpAddress m_listen_address;
	/** \brief  The listen port of the Server .*/
	unsigned short m_listen_port;
	/** \brief  The nmaximumu number of threads workers of the Server .*/
	unsigned short m_max_threads_workers;
};

} // namespace cmop
#endif /* CMOPRESTSERVER_H_ */
