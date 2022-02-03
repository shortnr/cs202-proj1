//
// File: game.cpp
// Author: Nick Short
// Date: 01.31.2022
// Use: Contains class definitions for the Game and Player classes.
//

#include <locale>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "game.h"

using namespace std;

Player::Player(int p_num) : name(""), health(420), selected_card(nullptr),
                            player_number(p_num), spell_absorb(0),
                            physical_mit(0), avoid_chance(0), is_avoiding(false)
{
}

void Player::init_wins()
{
   int height, width;
   getmaxyx(stdscr, height, width);

   // newwin(width, height, y, x)
   if(player_number == 1)
   {
      name_win = newwin(7, 40, height / 2 - 2, width / 2 - 20);
      field_win = newwin(FIELD_WIN_H, FIELD_WIN_W, height - FIELD_WIN_H, 0);
      health_win = newwin(HEALTH_WIN_H, HEALTH_WIN_W, height - HEALTH_WIN_H - 2, 3);
      status_win = newwin(STATUS_WIN_H, STATUS_WIN_W, height - HEALTH_WIN_H - STATUS_WIN_H - 3, 3);
      draw_main_win = newwin(DECK_M_WIN_H, DECK_M_WIN_W, height - DECK_M_WIN_H - 2, HEALTH_WIN_W + 5);

      int draw_x, draw_y;
      getbegyx(draw_main_win, draw_y, draw_x);

      for(int i = 0; i < 5; i++)
         draw_sub_wins[i] = newwin(DECK_C_WIN_H, DECK_C_WIN_W, draw_y + 2 + i, draw_x + 3 + 2 * i);

      junk_main_win = newwin(DECK_M_WIN_H, DECK_M_WIN_W, height - DECK_M_WIN_H - 2, draw_x + DECK_M_WIN_W + 2);

      for(int i = 0; i < 5; i++)
         junk_sub_wins[i] = newwin(DECK_C_WIN_H, DECK_C_WIN_W, draw_y + 2 + i, draw_x + 3 + 2 * i);

      int spacing = (int)((width - 5 * CARD_WIN_W - 2 * CARD_WIN_BUFF) / 4);

      for(int i = 0; i < 5; i++)
      {
         int x_coord = CARD_WIN_BUFF + i * (spacing + CARD_WIN_W);
         hand_wins[i] = newwin(CARD_WIN_H, CARD_WIN_W, height / 2 - CARD_WIN_H / 2, x_coord); 
      }
   }
   else
   {
      name_win = newwin(7, 40, height / 2 - 2, width / 2 - 20);
      field_win = newwin(FIELD_WIN_H, FIELD_WIN_W, 0, width - FIELD_WIN_W);
      health_win = newwin(HEALTH_WIN_H, HEALTH_WIN_W, 2, width - HEALTH_WIN_W - 3);
      status_win = newwin(STATUS_WIN_H, STATUS_WIN_W, 3 + HEALTH_WIN_H, width - STATUS_WIN_W - 3);
      draw_main_win = newwin(DECK_M_WIN_H, DECK_M_WIN_W, 2, width - HEALTH_WIN_W - DECK_M_WIN_W - 5);

      int draw_x, draw_y;
      getbegyx(draw_main_win, draw_y, draw_x);

      for(int i = 0; i < 5; i++)
         draw_sub_wins[i] = newwin(DECK_C_WIN_H, DECK_C_WIN_W, draw_y + 2 + i, draw_x + 3 + 2 * i);

      junk_main_win = newwin(DECK_M_WIN_H, DECK_M_WIN_W, 2, width - HEALTH_WIN_W - 2 * DECK_M_WIN_W - 7);

      for(int i = 0; i < 5; i++)
         junk_sub_wins[i] = newwin(DECK_C_WIN_H, DECK_C_WIN_W, draw_y + 2 + i, draw_x + 3 + 2 * i);

      int spacing = (int)((width - 5 * CARD_WIN_W - 2 * CARD_WIN_BUFF) / 4);

      for(int i = 0; i < 5; i++)
      {
         int x_coord = CARD_WIN_BUFF + i * (spacing + CARD_WIN_W);
         hand_wins[i] = newwin(CARD_WIN_H, CARD_WIN_W, height / 2 - CARD_WIN_H / 2, x_coord); 
      }
   }
}

void Player::choose_name()
{
   int width, height;
   char temp_char[20] = "blah";
   getmaxyx(stdscr, height, width);
   Statics::draw_border(name_win, Statics::Borders::double_line_white);
   std::string title = "Player " + std::to_string(player_number);
   mvwaddstr(name_win, 0, 2, title.c_str());
   mvwaddstr(name_win, 2, 3, "What is your name? ");
   wrefresh(name_win);
   echo();
   curs_set(1);
   wgetstr(name_win, temp_char);
   curs_set(0);
   noecho();
   std::string temp_str(temp_char);
   name = temp_str;
   wattron(name_win, A_STANDOUT);
   mvwaddstr(name_win, 4, 11, "Press Any Key To Continue");
   wattroff(name_win, A_STANDOUT);
   wrefresh(name_win);
   wgetch(name_win);
   werase(name_win);
   wrefresh(name_win);
}

void Player::build_deck()
{
   std::string line = "blah";
   std::ifstream file("deck_a.dat");
   Card * card = nullptr;
   char c;

   if(file.is_open())
   {
      while(getline(file, line))
      {
         try
         {
            c = line.at(0);
         }
         catch(...)
         {
            c = '\n';
         }

         switch(c)
         {
            case 'a':
               card = build_action(file);
               break;
            case 's':
               card = build_spell(file);
               break;
            case 'd':
               card = build_defense(file);
               break;
            case '\n':
            default:
               card = nullptr;
               break;
         }

         if(card)
            draw_pile.insert(card);
      }
      file.close();
   }
   for(int i = 0; i < 5; i++)
      draw_card();
}

Card * Player::build_action(std::ifstream & stream)
{
   std::string name, desc, value, type, target;
   Action * action;

   getline(stream, name);
   getline(stream, desc);
   getline(stream, value);
   getline(stream, type);
   getline(stream, target);

   action = new Action();

   action->change_name(name.c_str());
   action->change_description(desc);
   action->change_value(std::stoi(value));

   return action;
}

Card * Player::build_spell(std::ifstream & stream)
{
   std::string name, desc, value, type, target, effect_text, duration;
   char char_name[30] = {'\0'};
   
   Spell * spell;

   getline(stream, name);
   getline(stream, desc);
   getline(stream, value);
   getline(stream, type);
   getline(stream, target);
   getline(stream, effect_text);
   getline(stream, duration);

   spell = new Spell();

   strncpy(char_name, name.c_str(), name.length());
   spell->change_name(char_name);
   spell->change_description(desc);
   spell->change_value(std::stoi(value));

   return spell;
}

Card * Player::build_defense(std::ifstream & stream)
{
   std::string name, desc, value, type;

   Defense * defense;

   getline(stream, name);
   getline(stream, desc);
   getline(stream, value);
   getline(stream, type);

   defense = new Defense();

   defense->change_name(name.c_str());
   defense->change_description(desc);
   defense->change_value(std::stoi(value));

   return defense;
}

void Player::draw_card()
{
   Card * card = nullptr;
   while(!card)
   {
      draw_pile.swap_decks(junk_pile);
      card = draw_pile.draw_card();
   }
   player_hand.insert(card);
}

void Player::choose_card()
{
   int ch = 0;
   selected_card = player_hand.begin();
   Card * last_card = nullptr;
   
   selected_card_int = 0;
   player_hand.display(hand_wins);
   
   while(ch != 10)
   {
      selected_card->display(hand_wins[selected_card_int], Statics::Borders::double_line_white);
      switch(ch)
      {
         case 'a':
         case 'A':
            if(selected_card_int != 0)
            {
               --selected_card_int;
               selected_card = player_hand.prev();
            }
            break;
         case 'd':
         case 'D':
            if(selected_card_int != 4)
            {
               ++selected_card_int;
               selected_card = player_hand.next();
            }
            break;
         default:
            break;
      }
      selected_card->display(hand_wins[selected_card_int], Statics::Borders::double_line_red);
      ch = getch();
   }
}

void Player::add_effect()
{
   //Status * new_status = new Status;
   //new_status->time_left = selected_card->get_duration();
}

// Iterates through the status effects applied to the player, adding
// or removing health as necessary. If the time left on an effect is
// zero, the effect is removed from the vector.
void Player::apply_effects()
{
   /*
   vector<Status>::iterator effect;

   for(status = status_effects.begin(); status != status_effects.end(); ++status)
   {
      if(status->damaging_effect)
         harm(status->value);
      else
         heal(status->value);

      time_left--;
      if(!time_left)
         status = status_effects.erase(status)
   }
   */
}

void Player::play_card()
{
   selected_card->play(this, opponent);
}

void Player::set_opponent(Player * new_opponent)
{
   opponent = new_opponent;
}

void Player::apply_avoidance(int chance)
{
   avoid_chance = chance;
   is_avoiding = true;
}

void Player::take_physical_damage(int damage)
{
   if(is_avoiding)
      if(avoid_chance > rand() % 100)
         return;

   if(damage > physical_mit)
   {
      harm(damage - physical_mit);
      physical_mit = 0;
   }
   else
      physical_mit -= damage;
}

void Player::take_spell_damage(int damage)
{
   if(damage > spell_absorb)
   {
      harm(damage - spell_absorb);
      spell_absorb = 0;
   }
   else
      spell_absorb -= damage;
}

void Player::heal(int amount)
{
   int old_health = health;
   health += amount;

   // Update hp bar here.
}

void Player::harm(int amount)
{
   int red_hearts;
   int green_hearts;

   health -= amount;

   if(health % 20 == 0)
   {
      red_hearts = 20;
      green_hearts = (health - 20) / 20;
   }
   else
   {
      red_hearts = health % 20;
      green_hearts = (health - health % 20) / 20;
   }

   wmove(health_win, 3, 5);
   wattron(health_win, COLOR_PAIR(Statics::Colors::green_on_black));
   for(int i = 0; i < green_hearts; i++)
   {
      waddwstr(health_win, L"\u2665 ");
      wrefresh(health_win);
   }
   wattroff(health_win, COLOR_PAIR(Statics::Colors::green_on_black));

   wattron(health_win, COLOR_PAIR(Statics::Colors::white_on_black));
   for(int i = green_hearts; i < 20; i++)
   {
      waddwstr(health_win, L"\u2665 ");
      wrefresh(health_win);
   }
   wattroff(health_win, COLOR_PAIR(Statics::Colors::white_on_black));

   wmove(health_win, 4, 5);
   wattron(health_win, COLOR_PAIR(Statics::Colors::red_on_black));
   for(int i = 0; i < red_hearts; i++)
   {
      waddwstr(health_win, L"\u2665 ");
      wrefresh(health_win);
   }
   wattroff(health_win, COLOR_PAIR(Statics::Colors::red_on_black));

   wattron(health_win, COLOR_PAIR(Statics::Colors::white_on_black));
   for(int i = red_hearts; i < 20; i++)
   {
      waddwstr(health_win, L"\u2665 ");
      wrefresh(health_win);
   }
   wattroff(health_win, COLOR_PAIR(Statics::Colors::white_on_black));

   wrefresh(health_win);
}

// Returns false if player's hp > 0, true if hp == 0.
bool Player::is_player_dead()
{
   return !!!health;
}

void Player::draw_border(WINDOW * win, cchar_t border[]) {
    wborder_set(win, &border[0], &border[1], &border[2], &border[3],
               &border[4], &border[5], &border[6], &border[7]);
}

void Player::display() 
{
   display_field();
   display_health();
   display_status();
   display_decks();
}

void Player::display_field()
{
   Statics::draw_border(field_win, Statics::Borders::box_white);
   string name_string;
   int name_string_len;

   if(player_number == 1)
   {
      name_string = " Player " + to_string(player_number) + ": " + name + " ";
      name_string_len = strlen(name_string.c_str());
      wattron(field_win, A_STANDOUT);
      mvwaddstr(field_win, 1, 1, name_string.c_str());
      wattroff(field_win, A_STANDOUT);
      waddwstr(field_win, L"\u259B");
      wmove(field_win, 2, 1);
      for(int i = 0; i < name_string_len; i++)
         waddwstr(field_win, L"\u2580");
   }
   else
   {
      name_string = " Player " + to_string(player_number) + ": " + name + " ";
      name_string_len = strlen(name_string.c_str());
      mvwaddwstr(field_win, FIELD_WIN_H - 2, FIELD_WIN_W - name_string_len - 2, L"\u259F");
      wattron(field_win, A_STANDOUT);
      waddstr(field_win, name_string.c_str());
      wattroff(field_win, A_STANDOUT);
      wmove(field_win, FIELD_WIN_H - 3, FIELD_WIN_W - name_string_len - 1);
      for(int i = 0; i < name_string_len; i++)
         waddwstr(field_win, L"\u2584");
   }

   wrefresh(field_win);
}

void Player::display_health()
{
   draw_border(health_win, Statics::Borders::double_line_white);
   mvwaddwstr(health_win, 0, 2, L"\u2563Health\u2560");
   wattron(health_win, COLOR_PAIR(Statics::Colors::green_on_black));
   for(int i = 0; i < 2; i++)
   {
      wmove(health_win, 3 + i, 5);
      for(int j = 0; j < 20; j++)
      {
         waddwstr(health_win, L"\u2665 ");
      }
      wattron(health_win, COLOR_PAIR(Statics::Colors::red_on_black));
   }
   wrefresh(health_win);
   wattroff(health_win, COLOR_PAIR(Statics::Colors::red_on_black));
}

void Player::display_status()
{
   Statics::draw_border(status_win, Statics::Borders::double_line_white);
   mvwaddwstr(status_win, 0, 2, L"\u2563Status Effects\u2560");
   wrefresh(status_win);
}

void Player::display_decks()
{
   Statics::draw_border(draw_main_win, Statics::Borders::double_line_white);
   mvwaddwstr(draw_main_win, 0, 2, L"\u2563Draw\u2560");
   wrefresh(draw_main_win);

   Statics::draw_border(junk_main_win, Statics::Borders::double_line_white);
   mvwaddwstr(junk_main_win, 0, 2, L"\u2563Junk\u2560");
   wrefresh(junk_main_win);

   draw_pile.display(draw_sub_wins);
}

void Player::display_hand()
{
   player_hand.display(hand_wins);
}



//
// Game class implementation
//

Game::Game() : player_one(1), player_two(2)
{
   turns = 0;
   winner = 0;
   is_game_over = false;
}

void Game::init()
{
   setlocale(LC_CTYPE, "");
   srand(time(0));
   initscr();
   clear();
   start_color();
   cbreak();
   intrflush(stdscr, FALSE);
   keypad(stdscr, TRUE);
   noecho();
   refresh();
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

   player_one.init_wins();
   player_one.choose_name();
   player_one.build_deck();
   player_one.set_opponent(&player_two);
   player_one.display();

   player_two.init_wins();
   player_two.choose_name();
   player_two.build_deck();
   player_two.set_opponent(&player_one);
   player_two.display();
}

void Game::start()
{
   while(!is_game_over)
   {
      choose_card();
      //apply_effects();
      play_card();
      //draw_card();
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
   if(turns % 2 == 0)
   {
      player_one.play_card();
      player_two.play_card();
   }
   else
   {
      player_two.play_card();
      player_one.play_card();
   }
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
