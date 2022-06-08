#pragma once

#include "buggy.h"

class InputController
{
  Buggy* buggy;

  public:
    InputController(Buggy& _buggy);
    void play();

};
