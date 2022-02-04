//
// File: main.cpp
// Author: Nick Short
// Class: CS202, Project 1
// Date: 01/20/2022
// Description: Entrance file for the project. Instantiates a Game object, then
//              inits it and starts the game.

#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include "game.h"

int main(int argc, char** argv) {
   // Create Game object, then initialize it and start the game.
   Game game;
   game.init();
   game.start();

   // Ends the curses environment and restores the terminal to
   // normal operation.
   endwin();

   return 0;
}

