//
//  main.cpp
//  CrashHandler
//
//  Created by Vinay Gupta on 24/05/19.
//  Copyright © 2019 Vinay Gupta. All rights reserved.
//

#include <iostream>
#include "CrashHandler.h"
#include <signal.h>

#define STACKTRACE_FILE "stacktrace.txt"

void CrashHandlerCB(const char *file)
{
    std::cout<<"Crashed with CrashHandler";
    exit(-1);
}

int main(int argc, const char * argv[]) {
    std::cout << "Hello, World!\n";
    
    CrashHandler::GetInstance()->SetCrashHandlerCB( CrashHandlerCB );
    raise(SIGABRT);
    
    return 0;
}
