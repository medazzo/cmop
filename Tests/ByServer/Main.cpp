/*----------------------------------------------------------------------
|       includes
+---------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/file.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/file.h>
#include <sys/poll.h>
#include "HTTPUtility.h"
#include "HTTPTree.h"
#include "IHTTPHandler.h"
#include "HTTPServerTestHandlers.h"

#include "HTTPServer.h"


static EventingHandler *	eventing = NULL;
/*----------------------------------------------------------------------
|   signal CBX
+---------------------------------------------------------------------*/
static int STATE = 0;
static void sig_term(int sig) {
	STATE = 0;
	log_debug("we have received a SIG TERM :%d!!",STATE);
	log_info("put a choice:\n 1 :To Send Event.\n 2: To re-arm for events.\n 3:: To exit.\n " );
	scanf ("%d",&STATE);
}
/*----------------------------------------------------------------------
|   main
+---------------------------------------------------------------------*/

void SetAsterisk(HTTPServer * server){
	RootHandler * root = new RootHandler("root");
	OnReadHandler *	security = new OnReadHandler("security");
	AstrixHandler * astrix = new AstrixHandler();
	ErrorHandler *	error = new ErrorHandler("error");
	HTTPTree * tree = new HTTPTree(root);

	/*
	 *        Testing tree :
	 *                     root
	 *                    /   |
	 *           security      *
	 *                          \
	 *                         error
	 */
	HTTPTree::HTTPNode  * ast = tree->getRoot()->AddChildNode(astrix);
	tree->getRoot()->AddChildNode(security);
	ast->AddChildNode(error);
	/* Setting Server Tree and starting  */
	server->setTreeHandler(tree);
}
void SetTT(HTTPServer * server){
	RootHandler * root = new RootHandler("root");
	OnReadHandler *	security = new OnReadHandler("security");
	ErrorHandler *	error = new ErrorHandler("error");
	HTTPTree * tree = new HTTPTree(root);
	HTTPTree::HTTPNode  * sec = tree->getRoot()->AddChildNode(security);
	HTTPTree::HTTPNode  * err = sec->AddChildNode(error);
	err->AddChildNode(security);
	/* Setting Server Tree and starting  */
	server->setTreeHandler(tree);
}
int
main(int  argc, char**  argv )
{
	sigset_t sig_blocked;
	struct timeval sel_timeout;
	fd_set read_set;
	fd_set write_set;
	sigemptyset(&sig_blocked);
	sigaddset(&sig_blocked, SIGINT);
	sigaddset(&sig_blocked, SIGQUIT);
	sigaddset(&sig_blocked, SIGTERM);
	sigaddset(&sig_blocked, SIGHUP);
	signal(SIGCONT, SIG_IGN);
	signal(SIGUSR1, SIG_IGN);
	signal(SIGUSR2, SIG_IGN);
	signal(SIGPIPE, SIG_IGN);
	signal(SIGALRM, SIG_IGN);
	signal(SIGCHLD, SIG_IGN);
	signal (SIGHUP, sig_term);
	signal (SIGINT, sig_term);
	signal (SIGQUIT,sig_term);
	signal (SIGTERM,sig_term);


	HTTPServer * server = new HTTPServer(NPT_IpAddress::Any,1234,1);
//	SetHandlers(server);
	SetAsterisk(server);
	SetTT(server);
	server->Start();
	log_info("Server Started !");

	for(;;)
	{
        FD_ZERO (&read_set);
        FD_ZERO (&write_set);
        sel_timeout.tv_sec = 1;
        sel_timeout.tv_usec = 0;
        sigprocmask(SIG_UNBLOCK, &sig_blocked, NULL);
        select(2, &read_set, &write_set, NULL, &sel_timeout);
        sigprocmask(SIG_BLOCK, &sig_blocked, NULL);

        if(STATE == 1)
        {
        	log_info("W'll send an Event . " );
        	eventing->NotifyWaitingClients();
        }
        else if (STATE == 2){
    		log_info("Arming for new  Event . " );
    		eventing->ArmEventToBlockNewClients();

        }
        else if(STATE == 3)
        {
        	log_info("W'll exit . " );
			server->Stop();
			delete server;
			return 0;

        }
        else{}
        STATE = 0;
	}
}

/** @} */

