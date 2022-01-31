//
// File: game.cpp
// Author: Nick Short
// Date: 01.12.2022
// Use: Contains class definitions for classes that facilitate the
//      playing of the game.
//

#include <locale>
#include "game.h"

Game::Game() : player_one(1, player_one_wins), player_two(2, player_two_wins)
{
   turns = 0;
   winner = 0;
   is_game_over = false;
}

void Game::init()
{
   initscr();
   setlocale(LC_CTYPE, "");
   start_color();
   refresh();
   cbreak();
   keypad(stdscr, TRUE);
   noecho();
   curs_set(0);

   int height, width;
   getmaxyx(stdscr, height, width);

   title_win = newwin(TITLE_WIN_H, TITLE_WIN_W,
                      height / 2 - TITLE_WIN_H,
                      width / 2 - TITLE_WIN_W / 2);

   mvwaddwstr(title_win, 0, 0, Statics::Graphics::title_graphics);
   wgetch(title_win);
   werase(title_win);
   wrefresh(title_win);

   Statics::Init();

   player_one.init();
   player_one.choose_name();
   player_one.build_deck();
   player_one.set_opponent(&player_two);
   player_one.display();
   getch();

   player_two.init();
   player_two.choose_name();
   player_two.build_deck();
   player_two.set_opponent(&player_one);
   player_one.display();
}

void Game::start()
{
   while(!is_game_over)
   {
      draw_card();
      choose_card();
      apply_effects();
      play_card();
      winner = check_game_over();
      
      if(winner)
         game_over();
      else
         turns++;
   }
}

void Game::draw_card()
{
   if(turns % 2 == 0)
   {
      player_one.draw_card();
      player_two.draw_card();
   }
   else
   {
      player_two.draw_card();
      player_one.draw_card();
   }
}

void Game::choose_card()
{
   if(turns % 2 == 0)
   {
      player_one.choose_card();
      player_two.choose_card();
   }
   else
   {
      player_two.choose_card();
      player_one.choose_card();
   }
}

void Game::apply_effects()
{
   if(turns % 2 == 0)
   {
      player_one.apply_effects();
      player_two.apply_effects();
   }
   else
   {
      player_two.apply_effects();
      player_one.apply_effects();
   }
}

void Game::play_card()
{
}

int Game::check_game_over()
{
   int dead_players = 0;

   if(player_one.is_player_dead())
      dead_players += 1;

   if(player_two.is_player_dead())
      dead_players += 2;

   return dead_players;
}

void Game::game_over()
{
   switch(winner)
   {
      case 1:
         // Player two wins
         break;
      case 2:
         // Player one wins
         break;
      case 3:
         // It's a draw! Everyone is dead!
         break;
      default:
         break;
   }

   is_game_over = true;

   player_one.set_opponent(nullptr);
   player_two.set_opponent(nullptr);
}
