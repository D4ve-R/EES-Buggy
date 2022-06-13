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

void driveCircle(Buggy* bug)
{
    bug->moveForward(100, 3000);
    bug->rotate();
    bug->moveForward(100, 3000);
    bug->rotate();
    bug->moveForward(100, 3000);
    bug->rotate();
    bug->moveForward(100, 3000);
    bug->rotate();
    bug->moveForward(100, 3000);
}

int main(int argc, char* argv[])
{
    // set up interrupt handler
    signal(SIGINT, sigHandler);

    buggy = new Buggy(true);
   // driveCircle(buggy);
    buggy->drive();

    delete buggy;
    return 0;
}
