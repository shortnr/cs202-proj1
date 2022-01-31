//
// File: game.h
// Author: Nick Short
// Class: CS202, Project 1
// Date: 01.12.2022
// Use: Contains class declarations for classes that facilitate the
//      playing of the game.
//

#ifndef GAME_H_
#define GAME_H_

#include "player.h"

class Game {
   private:
      Player player_one;
      Player player_two;
      WINDOW * title_win;
      WINDOW * middle_win;
      WINDOW * player_one_wins[13];
      WINDOW * player_two_wins[13];
      int turns;
      int winner;
      bool is_game_over;

      // Enters the draw phase of the current player's turn.
      void draw_card();

      // Enters the choose card phase of the game. Players choose cards
      // to play this round.
      void choose_card();

      // Enters the apply effects stage of the game. Effects are applied
      // to both characters during this phase.
      void apply_effects();

      // Enters the play a card phase of the current player's turn.
      // Player's chosen cards are played.
      void play_card();

      // Checks to see if the game is over.
      int check_game_over();

      // Prints the winning player number to the screen and ends the game.
      void game_over();

   public:
      Game();

      // Initializes the ncurses environment.
      void init();

      // Starts the game. Players choose their decks then a random
      // player starts.
      void start();
};

#endif
