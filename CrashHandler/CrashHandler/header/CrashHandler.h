/** \file CrashHandler.h
 This file defines CrashHandler class for handling signals
 @author: Vinay Gupta
 */

#ifndef _CRASHHANDLER_H
#define _CRASHHANDLER_H

#define DEFAULT_STACKTRACE_FILE "stacktrace.txt";
/*! Declare the type of the callback proc supplied by the client */

typedef void (*CrashHandlerProc)(const char *file);

/*!
 * \brief A class for handling signals such as SIGSEGV, SIGFPE, etc.
 */

class CrashHandler
{
public:

	/*! \brief Returns the CrashHandler instance (creates it if needed) */

	static CrashHandler* GetInstance();


	/*! \brief Sets the client callback proc. and installs the signal 
	 *  handlers
	 */
 
	void SetCrashHandlerCB(CrashHandlerProc proc);


	/*! \brief Returns the client callback procc. */
 
	CrashHandlerProc GetCrashHandlerCB() {
		return crashHandlerProc;
	}

	/*! \brief Sets the file for storing stack trace. */

	void SetStackTraceFile(const char* file);

	/*! \brief Returns the file for storing stack trace. */

	const char *GetStackTraceFile() {
		return stackTraceFile;
	}
	/*! \brief: default destrustor */

	virtual	~CrashHandler();
        
private:

	/*!
	 *  \brief Holds the address of the instantiated CrashHandler object
	 *
	 *  It will be used to get the client callback proc and the file for
	 *  storing stack trace by the signal hander registered via this class.
	 */

	static CrashHandler *theCrashHandler;

	/*! \brief Default construstor
	 *  This constructor will not register a client callback and the file
	 *  for stack trace. So when a crash happens only the stack trace will be
	 *  generated in a default file called "stacktrace.txt" in the current folder.
	 */

    CrashHandler();

 	/*! \brief Install signal hander proc for signal like SIGSEGV, SIGFPE, etc. */

	void InstallSignalHandlers();

	/*! \brief holds a file for storing stack trace. */

	char *stackTraceFile;

	/*! \brief holds client callback proc. If it is NULL, no callback will be done. */

    CrashHandlerProc crashHandlerProc;

};

#endif  // _CRASHHANDLER_H
