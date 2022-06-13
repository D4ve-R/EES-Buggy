#include "inputcontroller.h"

#include <iostream>

InputController::InputController(Buggy& _buggy)
{
  buggy = &_buggy;
  initscr();
  cbreak();
  noecho();

  keypad(stdscr, true);
}

/**
 * game loop
 * 
 */
void InputController::play()
{
  int c = 0;
    printw("Press Q to quit");
    refresh();

  do
  {
    flushinp();
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
        buggy->moveBackward();
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

      case 'P':
      case 'p':
        buggy->stop();
        break;
      
      default:
        break;
    }
  } while(c != 'Q' && c != 'q');

  buggy->stop();

  endwin();
}

