#pragma once

#include "buggy.h"

#include <ncurses.h>

/**
 * handles keyboard input  
 * and controls the buggy
 */
class InputController
{
  Buggy* buggy;

  public:
    InputController(Buggy& _buggy);
    void play();

};
