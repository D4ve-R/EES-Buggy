#include <iostream>
#include <signal.h>

#include "buggy.h"

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
    while(1)
        buggy->moveForward(MAX_SPEED, 10);

    return 0;
}
