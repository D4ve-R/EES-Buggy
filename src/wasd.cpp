#include <iostream>
#include <signal.h>

#include "buggy.h"
#include "inputcontroller.h"

Buggy* buggy = nullptr;

/**
 * Interrupt handler for ctrl-c keyboard interrupt
 */
void sigHandler(int sigNum)
{
    if(buggy != nullptr)
    {
        delete buggy;
    }
    exit(sigNum);
}

int main(int argc, char* argv[])
{
    signal(SIGINT, sigHandler);

    buggy = new Buggy();
    InputController ic = InputController(*buggy);
    
    ic.play();

    return 0;
}
