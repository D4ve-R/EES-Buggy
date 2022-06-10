#include "inputcontroller.h"

#include <iostream>
#include <ncurses.h>

InputController::InputController(Buggy& _buggy):
{
    buggy = &_buggy;
  initsrc();
  noecho();
}

void InputController::play()
{
  int c = 0;
  std::cout << "Press q to quit" << std::endl;

  do
  {
    c = getch();
    switch(c)
    {
        case KEY_UP:
      case 'W':
      case 'w':
        buggy->moveForward();
        break;

      case KEY_DOWN:
      case 'S':
      case 's':
        buggy->moveBackwards();
        break;
      
      case KEY_LEFT:
      case 'A':
      case 'a':
        buggy->turnLeft();
        break;

      case KEY_RIGHT:
      case 'D':
      case 'd':
        buggy->turnRight();
        break;
      
      default:
        break;
    }

    c = 0;

  } while(c != 'Q' && c != 'q');

  endwin();
}

