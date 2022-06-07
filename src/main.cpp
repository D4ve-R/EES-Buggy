#include <signal.h>

#include "buggy.h"

void sigHandler(int sigNum)
{
  // TODO
  // cut all sensors
  exit(sigNum);
}

int main()
{
  signal(SIGINT, sigHandler);

  Buggy bug;

  while(1);


  return 0;
}
