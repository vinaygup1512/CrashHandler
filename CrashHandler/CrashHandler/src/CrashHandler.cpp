/*!
 * \brief CrashHandler.cpp: implementation of the CrashHandler class.
 */
/** \example CrashHandler.cpp */
 //@author: Vinay Gupta

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <iostream>
#include <fstream>

#include "CrashHandler.h"

CrashHandler* CrashHandler::theCrashHandler = NULL;

static const char *SignalNames[] = {
	"UNKNOWN",  /* 0 */
	"SIGHUP",   /* 1 */
	"SIGINT",   /* 2 */
	"SIGQUIT",  /* 3 */
	"SIGILL",   /* 4 */
	"SIGTRAP",  /* 5 */
	"SIGABRT",  /* 6 */
	"SIGBUS",   /* 7 */
	"SIGFPE",   /* 8 */
	"SIGKILL",  /* 9 */
	"SIGBUS",   /* 10 */
	"SIGSEGV",  /* 11 */
	"SIGUSER2", /* 12 */
	"SIGPIPE",  /* 13 */
	"SIGALRM",  /* 14 */
	"SIGTERM"   /* 15 */
};

/*!
 * \brief The signal handling proc.
 */

void static SignalHandler(int sig)
{
	// Get the stack trace into a file, which could be
	// a file given by the client or a default file

	std::cout << "\n--------------------------------------------\n";
	std::cout << "\n   Crashed with signal [" << SignalNames[sig] << "]\n";
	std::cout << "\n--------------------------------------------\n";
	
	// execute client callback proc if not NULL
	
	CrashHandlerProc callbackProc = CrashHandler::GetInstance()->GetCrashHandlerCB();
	if (callbackProc) {
	//	(*callbackProc)(file);  // callback will issue exit() call
	}
	else {
		exit(-1);
	}
}

/*!
 * \brief Register the signal hander proc.
 * Since signal registration differ from platform to platform, the implementation
 * is enclosed with proper macros such as LINUX, SOLARIS, AIX, HPUX.
 */

#if defined(LINUX) || defined(SOLARIS)

void CrashHandler::InstallSignalHandlers()
{

	// register the signal handler for all signals

	struct sigaction action;

	action.sa_handler = SignalHandler;

	if (sigemptyset(&action.sa_mask))
		perror("sigemptyset() failure");
	if (sigaction(SIGSEGV, &action, NULL))
		perror("Can't set up a signal handler for SIGSEGV");
	if (sigaction(SIGBUS, &action, NULL))
		perror("Can't set up a signal handler for SIGBUS");
	if (sigaction(SIGABRT, &action, NULL))
		perror("Can't set up a signal handler for SIGABRT");
	if (sigaction(SIGFPE, &action, NULL))
		perror("Can't set up a signal handler for SIGFPE");
	if (sigaction(SIGTERM, &action, NULL))
		perror("Can't set up a signal handler for SIGTERM");
	if (sigaction(SIGINT, &action, NULL))
		perror("Can't set up a signal handler for SIGINT");
	if (sigaction(SIGILL, &action, NULL))
		perror("Can't set up a signal handler for SIGILL");

}
#elif defined(WIN_ENV) || defined(AIX)

void CrashHandler::InstallSignalHandlers()
{
	signal(SIGSEGV, SignalHandler);
	signal(SIGINT, SignalHandler);
	signal(SIGABRT, SignalHandler);
	signal(SIGFPE, SignalHandler);
	signal(SIGTERM, SignalHandler);
	signal(SIGILL, SignalHandler);
}

#elif defined(MAC_ENV) || defined(HPUX)

void CrashHandler::InstallSignalHandlers()
{
	signal(SIGSEGV, SignalHandler);
	signal(SIGBUS, SignalHandler);
	signal(SIGINT, SignalHandler);
	signal(SIGABRT, SignalHandler);
	signal(SIGFPE, SignalHandler);
	signal(SIGTERM, SignalHandler);
	signal(SIGILL, SignalHandler);
}

#endif // CrashHandler::InstallSignalHandlers()

/*!
 * \brief Default constructor
 * Set up the default stack trace file and initialize the callback pointer to be NULL.
 * Register signal handler proc for signals and record the address of this CrashHandler object.
 */ 

CrashHandler::CrashHandler()
{
	stackTraceFile = DEFAULT_STACKTRACE_FILE;
	crashHandlerProc = NULL;
}


/*!
 * \brief Default constructor
 * Frees the memory allocated to theCrashHandler if not NULL.
 */
CrashHandler::~CrashHandler()
{
	if( theCrashHandler )
		delete theCrashHandler;
}

/*! \brief Returns the CrashHandler instance (creates it if needed) 
*/

CrashHandler* CrashHandler::GetInstance()
{
	if( theCrashHandler == NULL ){
		theCrashHandler = new CrashHandler();
	}
	return theCrashHandler;
}

/*! \brief Sets the client callback proc. */

void CrashHandler::SetCrashHandlerCB(CrashHandlerProc proc) 
{
	crashHandlerProc = proc;

	InstallSignalHandlers();
}

/*! \brief Sets the file for storing stack trace. */

void CrashHandler::SetStackTraceFile(const char* file)
{
	// If the given file is good, use it; otherwise use default

	if (file != NULL && file[0] != '\0')
		stackTraceFile = (char *) file;
	
	else
		stackTraceFile = DEFAULT_STACKTRACE_FILE;
}

