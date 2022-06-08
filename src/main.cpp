#include <iostream>
#include <signal.h>

#include "buggy.h"


void sigHandler(int sigNum)
{
  exit(sigNum);
}

int main(int argc, char* argv[])
{
  signal(SIGINT, sigHandler);

  Buggy bug;

  buggy.moveForward(MAX_SPEED, 3000);
  std::cout << buggy.getSpeedMax();
  

  return 0;
}
