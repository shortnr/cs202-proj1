//
// File: player.cpp
// Author: Nick Short
// Date: 01/13/2022
// Description: Contains the implementation of the Player, Hand, and Deck classes.
//

#include <iostream>
#include <fstream>
#include <string>
#include <climits>
#include "player.h"
#include "statics.h"

using namespace std;

Player::Player(int p_num, WINDOW * wins[13]) : name(""), hp(420), selected_card(nullptr), player_number(p_num)
{
}

void Player::init()
{
   int height, width;
   getmaxyx(stdscr, height, width);

   field_win = newwin(FIELD_WIN_H, width, height - FIELD_WIN_H, 0);
   health_win = newwin(HEALTH_WIN_H, HEALTH_WIN_W, height - HEALTH_WIN_H - 2, 3);
   status_win = newwin(height - 7, 25, 6, 12);
   name_win = newwin(7, 40, height / 2 - 2, width / 2 - 20);
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
   wrefresh(name_win);
   wgetch(name_win);
   wattroff(name_win, A_STANDOUT);
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

Card * Player::choose_card()
{
   Card * return_val = nullptr;
   return return_val;
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

}

void Player::dis_card()
{

}

void Player::set_opponent(Player * new_opponent)
{
   opponent = new_opponent;
}

void Player::heal(int amount)
{
   int old_hp = hp;
   hp += amount;

   // Update hp bar here.
}

void Player::harm(int amount)
{
   int old_hp = hp;
   hp -= amount;

   // Update hp bar here.
}

// Returns false if player's hp > 0, true if hp == 0.
bool Player::is_player_dead()
{
   return !!!hp;
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
   std::string name_string = "Player " + to_string(player_number) + ": " + name + " "; 
   Statics::draw_border(field_win, Statics::Borders::box_white);
   wattron(field_win, A_STANDOUT);
   mvwaddstr(field_win, 1, 1, name_string.c_str());
   wattroff(field_win, A_STANDOUT);
   waddwstr(field_win, L"\u259B");
   wmove(field_win, 2, 1);
   for(int i = 0; i < name_string.length(); i++)
      waddwstr(field_win, L"\u2580");
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
   
}

void Player::display_decks()
{
   draw_pile.display(draw_wins);
   junk_pile.display(junk_wins);
}

void Player::display_hand()
{
   
}














