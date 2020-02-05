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


#include "Neptune.h" // just for logging
#include "HTTPServerTestHandlers.h"

NPT_SET_LOCAL_LOGGER("test.server.main")
//static EventingHandler *	eventing = NULL;
/*----------------------------------------------------------------------
|   signal CBX
+---------------------------------------------------------------------*/
static int STATE = 0;
static void sig_term(int sig) {
	Test_UNUSED(sig);
	STATE = 0;
	NPT_LOG_INFO_1("we have received a SIG TERM :%d!!",STATE);
	NPT_LOG_INFO("put a choice:\n 1 :To Send Event.\n 2: To re-arm for events.\n 3:: To exit.\n " );
	scanf ("%d",&STATE);
}
/*----------------------------------------------------------------------
|   main
+---------------------------------------------------------------------*/

void SetAsterisk(cmop::ICServer * server){
	OnAllHandler * root = new OnAllHandler("/root");
	OnReadHandler *	security = new OnReadHandler("/security");
	AstrixHandler * astrix = new AstrixHandler();
	ErrorHandler *	error = new ErrorHandler("/error");
	/*
	 *                ---- error
	 *                ---- security
	 *                ---- root
	 *                ---- *
	 */
	server->AddHandler(astrix),
	server->AddHandler(error);
	server->AddHandler(security);
	server->AddHandler(root);
}
void SetTT(cmop::ICServer * server){
	OnAllHandler * root = new OnAllHandler("/root");
	OnReadHandler *	voila = new OnReadHandler("/api/voila/security");
	ErrorHandler *	error = new ErrorHandler("/error");
	/*
	 *                ---- voila
	 *                ---- error
	 *                ---- root
	 */
	server->AddHandler(voila);
	server->AddHandler(error);
	server->AddHandler(root);
}
int
main(int  argc, char**  argv )
{
	Test_UNUSED(argc);
	Test_UNUSED(argv);
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
	signal(SIGHUP, sig_term);
	signal(SIGINT, sig_term);
	signal(SIGQUIT,sig_term);
	signal(SIGTERM,sig_term);


	cmop::ICServer * server = cmop::CSFactory::Instance().getCServer(cmop::CMOP_INTERFACE_ANY, 1234 , 3);
	cmop::ICServer * server2 = cmop::CSFactory::Instance().getCServer(cmop::CMOP_INTERFACE_ANY, 7777 , 3);

	SetAsterisk(server);
	SetTT(server2);

	server->StartServer();
	NPT_LOG_INFO("Server Started !");
	server2->StartServer();
	NPT_LOG_INFO("Server 2 Started !");

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
        	NPT_LOG_INFO("W'll send an Event . " );
        	//eventing->NotifyWaitingClients();
        }
        else if (STATE == 2){
        	NPT_LOG_INFO("Arming for new  Event . " );
    		//eventing->ArmEventToBlockNewClients();

        }
        else if(STATE == 3)
        {
        	NPT_LOG_INFO("W'll exit . " );
			server->Stop();
			server2->Stop();
			return 0;

        }
        else{}
        STATE = 0;
	}
	delete server;
	delete server2;
}

/** @} */
