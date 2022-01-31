//
//  File name: card.h
//  Author: Nick Short
//  Class: CS202, Project 1
//  Date: 01/19/2022
//  Description: Header file for the card class and its derived classes.
//

#include <iostream>
#include <string>
#include <cstring>
#include "statics.h"
#include <ncursesw/ncurses.h>

#ifndef CARD_H_
#define CARD_H_

//
// Card Class
//    The base class for all card types. Contains a virtual print
//    function to facilitate dynamic binding.
//
// Variables
//    name - char* containing the name of the card.
//    description - STL string containing the card description.
//    play_text - STL string containing the card pop-up text.
//    value - integer containing the value applied by the card
//            (healing, damage, etc.).
//
class Card {

   public:
      Card();
      Card(const Card& card_to_copy);
      virtual ~Card();

      // Virtual (unimplemented) functions to allow for dynamic binding
      // of derived classes.
      virtual void play()=0;
      virtual void display(WINDOW * win)=0;
      
      // Functions to set or change the name, description, and value
      // of a card.
      void change_name(const char * new_name);
      void change_description(const std::string &new_description);
      void change_value(int new_value);

   protected:
      char * name;
      std::string description;
      int value; 
};

//
// Action Class
//    Class defining Action cards. Inherits from Card.
//
// Variables
//    effect - enumeration of the Action's effect.
//    target - enumeration of the Action's target.
//
class Action : public Card {
   public:
      Action();
      Action(const Action& action_to_copy);
      
      // Dynamically bound function that performs the action associated
      // with an Action card.
      void play();

      // Dynamically bound function that displays an Action card. Currently
      // has no arguments while developing the data structures.
      void display(WINDOW * win);
      
      // Changes the target associated with the card.
      void change_target(Statics::Target new_target);

   private:
      //void play(Player * player);

      Statics::Target target;
};

//
// Spell Class
//    Class defining Spell cards. Inherits from Card.
//
// Variables
//    effect - enumeration of the Spell's effect.
//    target - enumeration of the Spell's target.
//    effect_text - char* containing the text displayed
//                  in the status window.
//    duration - integer containing the duration of the
//               Spell's effect.
//
class Spell : public Card {
   public:
      Spell();
      Spell(const Spell& spell_to_copy);
      ~Spell();
     
      void change_effect(Statics::SpellEffect new_effect);
      void change_target(Statics::Target new_target);
      void change_effect_text(const char * new_effect_text);
      void change_duration(int new_duration);

      // Dynamically bound function that casts the spell associated
      // with a Spell card.
      void play();

      // Dynamically bound function that displays a Spell card. Currently
      // has no arguments while developing the data structures.
      //void display();
      void display(WINDOW * win);

   private:
      //void play(Player * player);
      
      Statics::SpellEffect effect;
      Statics::Target target;
      char * effect_text;
      int duration;
};

//
// Defense Class
//    Class defining Defense cards. Inherits from Card.
//
// Variables
//    type - enumeration of the defense type.
//
class Defense : public Card {
   public:
      Defense();
      Defense(const Defense& defense_to_copy);

      void change_type(Statics::DefenseType new_type);

      // Dynamically bound function that performs the defense associated
      // with a Defense card.
      void play();

      // Dynamically bound function that displays a Defense card. Currently
      // has no arguments while developing the data structures.
      //void display();
      void display(WINDOW * win);
 
   private:
      //void play(Player * player);

      Statics::DefenseType type;
};

#endif
