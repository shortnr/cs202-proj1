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
#include <sstream>
#include <iomanip>
#include <iostream>
#include "game.h"

using namespace std;

// Player class constructor
Player::Player(int p_num) : name(""), health(420), selected_card(nullptr),
                            player_number(p_num), spell_absorb(0),
                            physical_mit(0), avoid_chance(0), is_avoiding(false)
{
   draw_pile = new Deck();
   junk_pile = new Deck(); // Not currently used.
}

Player::~Player()
{
   delete draw_pile;
   delete junk_pile; // Not currently used.
}

// Initializes the windows associated with the players. The windows are located in different
// areas of the screen (depending on the player) so there are two sets of window construction
// commands.
void Player::init_wins()
{
   int height, width;
   getmaxyx(stdscr, height, width);

   // newwin(width, height, y, x)
   if(player_number == 1)
   {
      // Initialize all of the main windows with their dimensions and locations
      name_win = newwin(7, 40, height / 2 - 2, width / 2 - 20);
      arrow_win = newwin(8, 17, height - FIELD_WIN_H / 2 - 4, FIELD_WIN_W + 6);
      field_win = newwin(FIELD_WIN_H, FIELD_WIN_W, height - FIELD_WIN_H, 0);
      health_win = newwin(HEALTH_WIN_H, HEALTH_WIN_W, height - HEALTH_WIN_H - 2, 3);
      status_win = newwin(STATUS_WIN_H, STATUS_WIN_W, height - HEALTH_WIN_H - STATUS_WIN_H - 3, 3);
      draw_main_win = newwin(DECK_M_WIN_H, DECK_M_WIN_W, height - DECK_M_WIN_H - 2, HEALTH_WIN_W + 5);

      // Gets the x and y locations of the draw window. Used to instantiate the individual
      // "card" windows.
      int draw_x, draw_y;
      getbegyx(draw_main_win, draw_y, draw_x);

      // Instantiates the draw pile subwindows.
      for(int i = 0; i < 5; i++)
         draw_sub_wins[i] = newwin(DECK_C_WIN_H, DECK_C_WIN_W, draw_y + 2 + i, draw_x + 3 + 2 * i);

      // Instantiates the junk (discard) pile. This is not used.
      junk_main_win = newwin(DECK_M_WIN_H, DECK_M_WIN_W, height - DECK_M_WIN_H - 2, draw_x + DECK_M_WIN_W + 2);

      // Instantiates the junk subwindows.
      for(int i = 0; i < 5; i++)
         junk_sub_wins[i] = newwin(DECK_C_WIN_H, DECK_C_WIN_W, draw_y + 2 + i, draw_x + 3 + 2 * i);

      int spacing = (int)((width - 5 * CARD_WIN_W - 2 * CARD_WIN_BUFF) / 4);

      // Instantiates the hand windows.
      for(int i = 0; i < 5; i++)
      {
         int x_coord = CARD_WIN_BUFF + i * (spacing + CARD_WIN_W);
         hand_wins[i] = newwin(CARD_WIN_H, CARD_WIN_W, height / 2 - CARD_WIN_H / 2, x_coord); 
      }
   }
   else
   {
      // Initialize all of the main windows with their dimensions and locations
      name_win = newwin(7, 40, height / 2 - 2, width / 2 - 20);
      arrow_win = newwin(8, 18, FIELD_WIN_H / 2 - 4, width - FIELD_WIN_W - 21);
      field_win = newwin(FIELD_WIN_H, FIELD_WIN_W, 0, width - FIELD_WIN_W);
      health_win = newwin(HEALTH_WIN_H, HEALTH_WIN_W, 2, width - HEALTH_WIN_W - 3);
      status_win = newwin(STATUS_WIN_H, STATUS_WIN_W, 3 + HEALTH_WIN_H, width - STATUS_WIN_W - 3);
      draw_main_win = newwin(DECK_M_WIN_H, DECK_M_WIN_W, 2, width - HEALTH_WIN_W - DECK_M_WIN_W - 5);

      // Gets the x and y locations of the draw window. Used to instantiate the individual
      // "card" windows.
      int draw_x, draw_y;
      getbegyx(draw_main_win, draw_y, draw_x);

      // Instantiates the draw pile subwindows.
      for(int i = 0; i < 5; i++)
         draw_sub_wins[i] = newwin(DECK_C_WIN_H, DECK_C_WIN_W, draw_y + 2 + i, draw_x + 3 + 2 * i);

      // Instantiates the junk (discard) pile. This is not used.
      junk_main_win = newwin(DECK_M_WIN_H, DECK_M_WIN_W, 2, width - HEALTH_WIN_W - 2 * DECK_M_WIN_W - 7);

      // Instantiates the junk subwindows.
      for(int i = 0; i < 5; i++)
         junk_sub_wins[i] = newwin(DECK_C_WIN_H, DECK_C_WIN_W, draw_y + 2 + i, draw_x + 3 + 2 * i);

      int spacing = (int)((width - 5 * CARD_WIN_W - 2 * CARD_WIN_BUFF) / 4);

      // Instantiates the hand windows.
      for(int i = 0; i < 5; i++)
      {
         int x_coord = CARD_WIN_BUFF + i * (spacing + CARD_WIN_W);
         hand_wins[i] = newwin(CARD_WIN_H, CARD_WIN_W, height / 2 - CARD_WIN_H / 2, x_coord); 
      }
   }
}

// Shows a popup that allows the player to enter their name.
void Player::choose_name()
{
   // Begin crazy ncurses stuff
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
   // End crazy ncurses stuff
}

// Reimplementation of the deck swap function. This one uses pointers to the deck
// objects and seems to work well.
void Player::swap_decks()
{
   Deck * temp;
   temp = draw_pile;
   draw_pile = junk_pile;
   junk_pile = temp;
}

// Opens the (hardcoded) data file containing the card information. Parses the file
// calling the respective card build functions depending on the data pulled from the
// data file. There is no error checking currently, so the datafile is very sensitive
// to errors.
void Player::build_deck()
{
   // Priming the line variable
   std::string line = "blah";

   // Open the filestream. Probably should be checking for errors here.
   std::ifstream file("deck_a.dat");

   Card * card = nullptr;
   char c;

   if(file.is_open())
   {
      while(getline(file, line))
      {
         // Gets the first character of the line associated with the card type for
         // use in the following switch statement. There were errors when using the
         // at(n) function when the line was empty, so I wrapped it in a try-catch
         // block and just assigned '\n' to c. I suppose I could have assigned it
         // anything and it would have been caught by the default case of the switch
         // statement.
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
            case 'a':   // Action Card
               card = build_action(file);
               break;
            case 's':   // Spell Card
               card = build_spell(file);
               break;
            case 'd':   // Defense Card
               card = build_defense(file);
               break;
            case '\n':
            default:
               card = nullptr;
               break;
         }

         // If the card was created, insert it into the draw pile.
         if(card)
            draw_pile->insert(card);
      }
      file.close();
   }
   // After the player's deck is created, draw 5 cards, randomly. This would probably
   // make more sense somewhere else (maybe inside it's own function?)
   for(int i = 0; i < 5; i++)
      draw_card();
}

// Builds Action cards. Parses the file for all the data associated with an Action Card.
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

// Builds Spell cards. Parses the file for all the data associated with a Spell Card.
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
   spell->change_duration(std::stoi(duration));

   if(type == "heal")
      spell->change_effect(Statics::SpellEffect::HEAL);
   else if(type == "hurt")
      spell->change_effect(Statics::SpellEffect::HURT);
   else
      spell->change_effect(Statics::SpellEffect::ABSORB);

   if(target == "self")
      spell->change_target(Statics::Target::SELF);
   else
      spell->change_target(Statics::Target::OPPONENT);

   return spell;
}

// Builds Defense cards. Parses the file for all the data associated with a Defense Card.
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

   if(type == "mitigation")
      defense->change_type(Statics::DefenseType::MITIGATION);
   else if(type == "avoidance")
      defense->change_type(Statics::DefenseType::AVOIDANCE);

   return defense;
}

// Draws a card from the Deck. Functionality described in datastructure.cpp:104.
void Player::draw_card()
{
   Card * card = nullptr;
  
   // Swaps decks if the draw pile is empty.
   if(draw_pile->is_empty())
      swap_decks();
   
   // Draws a card
   card = draw_pile->draw_card();

   // Removes the drawn card from the draw pile.
   draw_pile->remove(card);

   // Inserts the drawn card into the player's hand.
   player_hand.insert(card);

   // Updates the decks in their windows.
   display_decks();
}

// Allows the player to choose a card from their hand. 'a' moves left and 'd'
// moves right. Enter selects the card. When a player is choosing their card,
// a red, large-print unicode arrow points to that player. The currently selected
// card is bordered in red.
void Player::choose_card()
{
   int ch = 0;

   // Sets the players hand list iterator to the beginning of the list (first
   // card in their hand).
   selected_card = player_hand.begin();
   Card * last_card = nullptr;
   
   selected_card_int = 0;

   // Displays the cards in the players hand.
   player_hand.display(hand_wins);
 
   // Displays the red large-print arrow for the current player.
   wattron(arrow_win, COLOR_PAIR(Statics::Colors::red_on_black));
   if(player_number == 1)
      mvwaddwstr(arrow_win, 0, 0, Statics::Graphics::left_arrow);
   else
      mvwaddwstr(arrow_win, 0, 0, Statics::Graphics::right_arrow);
   wattroff(arrow_win, COLOR_PAIR(Statics::Colors::red_on_black));
   wrefresh(arrow_win);

   // While the player hasn't pressed the enter key, allows the player to move
   // left and right along their hand to select their desired card.
   while(ch != 10)
   {
      // Borders the previously selected card in white.
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
      // Borders the newly selected card in red.
      selected_card->display(hand_wins[selected_card_int], Statics::Borders::double_line_red);
      // Gets the new key press from the user. The getch() function has a timed
      // counterpart that waits a specified time for a keypress before timing out.
      // I think this could be used to slowly flash the border of the currently 
      // selected card, for extra effect.
      ch = getch();
   }

   wclear(arrow_win);
   wrefresh(arrow_win);
}

// Adds effects to the players StatusEffects DS. This is unfortunately not implemented.
void Player::add_effect(Effect * new_effect)
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

// Plays the player's selected card. Passes pointers to the player and the
// player's opponent so that the chosen card can directly call functions in
// the player objects (to deal damage, or heal, etc.). Removes the played
// card from the players hand and reinserts into the draw pile.
void Player::play_card()
{
   selected_card->play(this, opponent);
   player_hand.remove(selected_card);
   draw_pile->insert(selected_card);
}

// Sets the player's opponent. Used to pass a pointer to the opponent when
// playing cards.
void Player::set_opponent(Player * new_opponent)
{
   opponent = new_opponent;
}

// Applies damage mitigation to the player. Absorbs physical damage.
void Player::apply_mitigation(int amount)
{
   physical_mit += amount;
   display_status();
}

// Applied avoidance to the player. Gives the player a chance to completely
// avoid damage on their next turn.
void Player::apply_avoidance(int chance)
{
   avoid_chance = chance;
   is_avoiding = true;
   display_status();
}

// Applies a spell absorbtion value to the player. Absorbs spell damage until
// depleted.
void Player::apply_spell_absorb(int amount)
{
   spell_absorb += amount;
   display_status();
}

// Causes the player to potentially take physical damage. First checks for a
// successful avoid, then mitigates the damage from the mitigation pool. The
// remaining damage is applied to the player.
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

// Causes the player to potentially take spell damage. First absorbs damage
// with the spell absorb pool, then applies the rest of the damage to the
// player.
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

// Heals the player for an amount. I ran out of time to implement this. It
// would essentially be the opposite of harm().
void Player::heal(int amount)
{
   int old_health = health;
   health += amount;
}

// Harms the player for an amount. Updates the players health bar after damage
// is applied. I *nearly* had this animated, but it was doing weird stuff, so
// I just implemented it so that it makes all the updates, then refreshes the
// health window at the end. Kinda underwhelming.
void Player::harm(int amount)
{
   int red_hearts;
   int green_hearts;

   health -= amount;

   // Determines the number of red and green hearts the player should have after
   // damage is applied.
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

   // Complex ncurses stuff..
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
   // End complex ncurses stuff...
}

// Returns false if player's hp > 0, true if hp == 0.
bool Player::is_player_dead()
{
   return !!!health;
}

// Draws a border around a particular window. This function was moved
// to the Static class as an inline static function, but apparently
// was not removed from here.
void Player::draw_border(WINDOW * win, cchar_t border[]) {
    wborder_set(win, &border[0], &border[1], &border[2], &border[3],
               &border[4], &border[5], &border[6], &border[7]);
}

// Calls the individual display functions for the various windows.
void Player::display() 
{
   display_field();
   display_health();
   display_status();
   display_decks();
}

// Displays the "field", the outside border containing all of the
// player's windows.
void Player::display_field()
{
   Statics::draw_border(field_win, Statics::Borders::box_white);
   string name_string;
   int name_string_len;

   // Displays the players name sorta in the border. Displays in the
   // upper left of the field border for player 1, and the lower right
   // for player 2.
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

// Displays the players hearts. This is mainly used for the initial display.
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

// Displays the players status effects. Currenly only displays the value
// of the players physical mitigation, avoid chance, and spell absorption.
void Player::display_status()
{
   stringstream ss;
   ss << setw(3) << "Mitigation: " << physical_mit << '\t' 
                 << " Avoidance: " << avoid_chance << '\t'
                 << "Absorbtion: " << spell_absorb;

   Statics::draw_border(status_win, Statics::Borders::double_line_white);
   mvwaddwstr(status_win, 0, 2, L"\u2563Status Effects\u2560");
   mvwaddstr(status_win, 1, 2, ss.str().c_str());
   wrefresh(status_win);
}

// Displays the player's decks.
void Player::display_decks()
{
   wclear(draw_main_win);
   wclear(junk_main_win);

   char num_string[20];
   sprintf(num_string, "%d/%d ", draw_pile->num_left(), draw_pile->num_total());
   Statics::draw_border(draw_main_win, Statics::Borders::double_line_white);
   mvwaddwstr(draw_main_win, 0, 2, L"\u2563Draw\u2560");
   mvwaddstr(draw_main_win, 13, 2, num_string);
   wrefresh(draw_main_win);

   sprintf(num_string, "%d/%d ", junk_pile->num_left(), junk_pile->num_total());
   Statics::draw_border(junk_main_win, Statics::Borders::double_line_white);
   mvwaddwstr(junk_main_win, 0, 2, L"\u2563Junk\u2560");
   mvwaddstr(junk_main_win, 13, 2, num_string);
   wrefresh(junk_main_win);

   draw_pile->display(draw_sub_wins);
   junk_pile->display(junk_sub_wins);
}

// Displays the player's hand.
void Player::display_hand()
{
   player_hand.display(hand_wins);
}



//
// Game class implementation
//

// Game class constructor.
Game::Game() : player_one(1), player_two(2)
{
   turns = 0;
   winner = 0;
   is_game_over = false;
}

// Initializes the curses environment and displays the splash screen. Also
// calls the Statics initialization function to associate color pairs and
// border styles. Initializes the players, as well (choosing name, building
// decks, drawing initial cards, etc.).
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

   // Initialize the Statics class.
   Statics::Init();

   // Initialize player 1
   player_one.init_wins();
   player_one.choose_name();
   player_one.build_deck();
   player_one.set_opponent(&player_two);
   player_one.display();

   // Initialize player 2
   player_two.init_wins();
   player_two.choose_name();
   player_two.build_deck();
   player_two.set_opponent(&player_one);
   player_two.display();
}

// Starts and controls the game flow until the game is over. Play alternates
// between player one and player two choosing their cards first each round.
// This is accomplished using the turns counter and the modulo operator.
void Game::start()
{
   while(!is_game_over)
   {
      choose_card();
      //apply_effects();
      play_card();
      draw_card();
      winner = check_game_over();
      
      if(winner)
         game_over();
      else
         turns++;
   }
}

// Calls the player draw functions
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

// Calls the player choose_card functions.
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

// Calls the player apply_effects functions.
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

// Calls the player play_card functions.
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

// Checks if the game is over. One or both players may win the game,
// depending on who's dead at the end of a round.
int Game::check_game_over()
{
   int dead_players = 0;

   if(player_one.is_player_dead())
      dead_players += 1;

   if(player_two.is_player_dead())
      dead_players += 2;

   return dead_players;
}

// Theoretically displays a splash screen. I spent all my time implementing
// functionality that would lead to a finished game and was unable to get to
// this. :(
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
