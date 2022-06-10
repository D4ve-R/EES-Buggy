#pragma once

#include "buggy.h"

#include <ncurses.h>

class InputController
{
  Buggy* buggy;

  public:
    InputController(Buggy& _buggy);
    void play();

};
