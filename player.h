//
// File: player.h
// Author: Nick Short
// Class: CS202, Project 1
// Date: 01/12/2022
// Description: Contains the class definitions for the Player class.
//

#ifndef PLAYER_H_
#define PLAYER_H_

#include <string>
#include <fstream>
#include <ncursesw/ncurses.h>
#include "card.h"
#include "datastructure.h"

class Player {
   private:
      std::string name;
      int player_number;

      // Player's decks
      Deck * draw_pile;
      Deck * junk_pile;

      // Players hand
      Hand player_hand;

      // The currently selected card. This pointer exists so the current
      // card can be passed around outside of a data structure without
      // being lost.
      Card * selected_card;
      int selected_card_int;

      // A pointer to the player's opponent
      Player * opponent;

      // ncurses WINDOW pointers
      WINDOW * field_win;
      WINDOW * health_win;
      WINDOW * status_win;
      WINDOW * name_win;
      WINDOW * arrow_win;
      WINDOW * hand_wins[5];
      WINDOW * draw_main_win;
      WINDOW * draw_sub_wins[5];
      WINDOW * junk_main_win;
      WINDOW * junk_sub_wins[5];

      // Data structure containing the status effect effecting the player.
      StatusEffects status_effects;
      int health;

      // Variables to determine damage resistance and avoidance.
      int spell_absorb;
      int physical_mit;
      int avoid_chance;
      bool is_avoiding;

      // Card building functions. Receive an input file stream and parse
      // data specific to the card type.
      Card * build_action(std::ifstream & stream);
      Card * build_spell(std::ifstream & stream);
      Card * build_defense(std::ifstream & stream);

      // Private display functions for individual windows. These are called
      // in order in the public display function. Really just used for setting
      // up the initial borders and window titles.
      void display_field();
      void display_health();
      void display_status();
      void display_decks();
      void display_hand();

   public:
      Player(int player_number);
      ~Player();

      // Initializes player windows
      void init_wins();

      // Player chooses a name.
      void choose_name();

      // Builds player deck.
      void build_deck();

      // Swaps the draw and junk decks.
      void swap_decks();

      // Removes (draws) a card from the draw Deck CLL and inserts
      // it into the Hand ALLL.
      void draw_card();
   
      // Allows the player to select a card (ideally graphically,
      // but textually, if necessary) to play or discard. Sets the 
      // selected_card variable.
      void choose_card();

      // Adds a status effect to the vector of effects on the player.
      void add_effect(Effect * new_effect);

      // Applies the effects from the vector of effects to the player.
      void apply_effects();

      // Plays a card from the player's hand, then discards the card
      // to the junk Deck.
      void play_card();

      // Sets the players opponent. This will allow the play_card
      // function to apply damage to the opponent through the use of
      // the opponent's harm function.
      void set_opponent(Player * new_opponent);
      
      // Applies an amount of physical mitigation.
      void apply_mitigation(int amount);

      // Applies an amount of avoidance.
      void apply_avoidance(int chance);
      
      // Applies an amount of spell absorbtion.
      void apply_spell_absorb(int amount);

      // Takes an amount of physical damage. Checks for mitigation and
      // avoidance.
      void take_physical_damage(int damage = 0);

      // Takes an amount of spell damage.
      void take_spell_damage(int damage);

      // Adds an amount of health to the player's life pool.
      void heal(int amount);

      // Removes an amount of health from the player's life pool.
      void harm(int amount);

      // Returns true if the players' health is zero.
      bool is_player_dead();

      // Displays the players health pool and calls the Hand and Deck's
      // display functions.
      void display();

      // Draws a border around an ncurses WINDOW.
      void draw_border(WINDOW * win, cchar_t border[]);
};

#endif
