/* 
 *
 *
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include "datastructure.h"
#include "game.h"

int main(int argc, char** argv) {
   Game game;
   game.init();
   game.start();
   endwin();
   return 0;
}





