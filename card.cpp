// 
// File name: card.cpp
// Author: Nick Short
// Purpose: Implementation of the Card, Action, Spell and Defense
//          classes.
//

#include <iostream>
#include <string>
#include <list>
#include "card.h"

// 
// Card class implementation.
//

Card::Card()
{
   name = nullptr;
   description = "";
   value = 0;
}

Card::Card(const Card & card_to_copy)
{
   name = new char[sizeof(card_to_copy.name)];
   strncpy(name, card_to_copy.name, sizeof(card_to_copy.name));
   description = card_to_copy.description;
   value = card_to_copy.value;
}

Card::~Card()
{
   delete[] name;
   name = nullptr;
}

void Card::change_name(const char * new_name)
{
   int length = strlen(new_name) + 1;
   name = new char[length];
   strncpy(name, new_name, length);
}

void Card::change_description(const std::string &new_description)
{
   description = new_description;
}

void Card::change_value(int new_value)
{
   value = new_value;
}

// 
// Action class implementation.
//

Action::Action()
{
   target = Statics::Target::SELF;
}

Action::Action(const Action & action_to_copy) :
   Card(action_to_copy)
{

}

void Action::change_target(Statics::Target new_target)
{
   target = new_target;
}

void Action::play()
{

}

void Action::display(WINDOW * win)
{
   int height, width;
   getmaxyx(win, height, width);

   Statics::draw_border(win, Statics::Borders::double_line_white);
   //box(win, 0, 0);
  
   // Print card name.
   mvwaddstr(win, 1, 2, name);

   // Horizontal line dividing name and description.
   mvwhline(win, 2, 1, ACS_HLINE, width - 2);

   // Print card type.
   mvwaddstr(win, height - 2, width - 8, "Action");

   // Print word-wrapped card description.
   std::list<std::string> desc = Statics::word_wrap(description, width - 3);
   std::list<std::string>::iterator it;

   int line = 3;
   for(it = desc.begin(); it != desc.end(); ++it)
   {
      mvwaddstr(win, line, 2, (*it).c_str());
      line += 1;
   }

   mvwhline(win, height - 3, 1, ACS_HLINE, width - 2);

   // Print card value.
   char value_str[10];
   sprintf(value_str, "%d", value);
   mvwaddstr(win, height - 2, 2, value_str);
      
   wrefresh(win);
}

//
// Spell class implementation.
//

Spell::Spell()
{
   effect_text = nullptr;
   effect = Statics::SpellEffect::HEAL;
   target = Statics::Target::SELF;
   duration = 1;
}

Spell::Spell(const Spell & spell_to_copy) :
   Card(spell_to_copy)
{
   effect_text = new char[sizeof(spell_to_copy.effect_text)];
   strncpy(effect_text, spell_to_copy.effect_text, sizeof(spell_to_copy.effect_text));
   effect = spell_to_copy.effect;
   target = spell_to_copy.target;
   duration = spell_to_copy.duration;
}

Spell::~Spell()
{
   delete[] effect_text;
   effect_text = nullptr;
}

void Spell::change_effect(Statics::SpellEffect new_effect)
{
   effect = new_effect;
}

void Spell::change_target(Statics::Target new_target)
{
   target = new_target;
}

void Spell::change_effect_text(const char * new_effect_text)
{
   delete effect_text;
   effect_text = new char[sizeof(new_effect_text)];
   strncpy(effect_text, new_effect_text, sizeof(new_effect_text));
}

void Spell::change_duration(int new_duration)
{
   duration = new_duration;
}

void Spell::play()
{

}

void Spell::display(WINDOW * win)
{
   int height, width;
   char temp[20];
   getmaxyx(win, height, width);

   box(win, 0, 0);
   
   // Print card name.
   mvwaddstr(win, 1, 2, name);

   // Print card target.
   switch(target)
   {
      case Statics::Target::SELF:
         sprintf(temp, "(%s)", "Self");
         mvwaddstr(win, 2, 2, temp);
         break;
      case Statics::Target::OPPONENT:
         sprintf(temp, "(%s)", "Opponent");
         mvwaddstr(win, 2, 2, temp);
         break;
      default:
         break;
   }

   // Horizontal line dividing name/target and description.
   mvwhline(win, 3, 1, ACS_HLINE, width - 2);

   // Print word-wrapped card description.
   std::list<std::string> desc = Statics::word_wrap(description, width - 3);
   std::list<std::string>::iterator it;

   int line = 4;
   for(it = desc.begin(); it != desc.end(); ++it)
   {
      mvwaddstr(win, line, 2, (*it).c_str());
      line += 1;
   }

   // Print card duration.
   switch(duration)
   {
      case 0:
         mvwaddstr(win, height - 4, 2, "Duration: Instant");
         break;
      default:
         sprintf(temp, "Duration: %d", duration);
         mvwaddstr(win, height - 4, 2, temp);
   }

   // Horizontal line dividing description and value/card type.
   mvwhline(win, height - 3, 1, ACS_HLINE, width - 2);

   // Print card value.
   sprintf(temp, "%d", value);
   mvwaddstr(win, height - 2, 2, temp);

   // Print card type.
   mvwaddstr(win, height - 2, width - 8, "Spell");
      
   wrefresh(win);
}

// 
// Defense class implementation.
//

Defense::Defense()
{
   type = Statics::DefenseType::ABSORB;
}

Defense::Defense(const Defense & defense_to_copy) :
   Card(defense_to_copy)
{
   type = defense_to_copy.type;
}

void Defense::change_type(Statics::DefenseType new_type)
{
   type = new_type;
}

void Defense::play()
{

}

void Defense::display(WINDOW * win)
{
   int height, width;
   char temp[20];
   getmaxyx(win, height, width);

   box(win, 0, 0);
  
   // Print card name.
   mvwaddstr(win, 1, 2, name);

   // Print defense type.
   switch(type)
   {
      case Statics::DefenseType::AVOID:
         mvwaddstr(win, 2, 2, "(Avoidance)");
         break;
      case Statics::DefenseType::ABSORB:
         mvwaddstr(win, 2, 2, "(Absorbtion)");
         break;
      default:
         break;
   }

   // Horizontal line dividing name and description.
   mvwhline(win, 3, 1, ACS_HLINE, width - 2);

   // Print card type.
   mvwaddstr(win, height - 2, width - 9, "Defense");

   // Print word-wrapped card description.
   std::list<std::string> desc = Statics::word_wrap(description, width - 3);
   std::list<std::string>::iterator it;

   int line = 4;
   for(it = desc.begin(); it != desc.end(); ++it)
   {
      mvwaddstr(win, line, 2, (*it).c_str());
      line += 1;
   }

   mvwhline(win, height - 3, 1, ACS_HLINE, width - 2);

   // Print card value.
   char value_str[10];
   sprintf(value_str, "%d", value);
   mvwaddstr(win, height - 2, 2, value_str);
      
   wrefresh(win);
}




















