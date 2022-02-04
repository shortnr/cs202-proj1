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
#include "statics.h"
#include "player.h"

// 
// Card class implementation.
//

// Card class constructor
Card::Card() 
{
   name = nullptr;
   description = "";
   value = 0;
}

// Card class copy constructor
Card::Card(const Card & card_to_copy) 
{
   name = new char[sizeof(card_to_copy.name)];
   strncpy(name, card_to_copy.name, sizeof(card_to_copy.name));
   description = card_to_copy.description;
   value = card_to_copy.value;
}

// Card class destructor
Card::~Card() 
{
   delete[] name;
   name = nullptr;
}

// Changes the name of the card
void Card::change_name(const char * new_name)
{
   int length = strlen(new_name) + 1;
   name = new char[length];
   strncpy(name, new_name, length);
}

// Changes the description of the card
void Card::change_description(const std::string &new_description)
{
   description = new_description;
}

// Changes the value of the vard. This value means something different
// depending on the type of card (physical damage, spell damage,
// mitigation, etc.).
void Card::change_value(int new_value)
{
   value = new_value;
}



// 
// Action class implementation.
//

// Action class constructor
Action::Action() : target(Statics::Target::SELF) {}

// Action class copy constructor
Action::Action(const Action & action_to_copy) : Card(action_to_copy) {}

// Changes the target of the card
void Action::change_target(Statics::Target new_target)
{
   target = new_target;
}

// Plays the action card against the opponent (so far, all action cards
// are purely physical damage based).
void Action::play(Player * player, Player * opponent)
{
   opponent->take_physical_damage(value);
}

// Displays the action card.
void Action::display(WINDOW * win, cchar_t border[8])
{
   int height, width;
   getmaxyx(win, height, width);

   Statics::draw_border(win, border);
  
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

   // Horizontal line between the description and the value.
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

// Spell class constructor
Spell::Spell()
{
   effect_text = nullptr;
   effect = Statics::SpellEffect::HEAL;
   target = Statics::Target::SELF;
   duration = 1;
}

// Spell class copy constructor
Spell::Spell(const Spell & spell_to_copy) :
   Card(spell_to_copy)
{
   effect_text = new char[sizeof(spell_to_copy.effect_text)];
   strncpy(effect_text, spell_to_copy.effect_text, sizeof(spell_to_copy.effect_text));
   effect = spell_to_copy.effect;
   target = spell_to_copy.target;
   duration = spell_to_copy.duration;
}

// Spell class destructor
Spell::~Spell()
{
   delete[] effect_text;
   effect_text = nullptr;
}

// Changes the spell's effect
void Spell::change_effect(Statics::SpellEffect new_effect)
{
   effect = new_effect;
}

// Changes the spell's target
void Spell::change_target(Statics::Target new_target)
{
   target = new_target;
}

// Changes the spell's effect text. *Effect text is not currently displayed
// anywhere.
void Spell::change_effect_text(const char * new_effect_text)
{
   delete effect_text;
   effect_text = new char[sizeof(new_effect_text)];
   strncpy(effect_text, new_effect_text, sizeof(new_effect_text));
}

// Changes the spell's duration. This is only partially implemented.
void Spell::change_duration(int new_duration)
{
   duration = new_duration;
}

// Plays the spell card on the player if it's a healing or shield spell
// or on the opponent if it's a damaging spell.
void Spell::play(Player * player, Player * opponent)
{
   if(duration)
   {
      // Creates a new effect if the spell has a duration associated with
      // it. These effects are pushed to a StatusEffect object which manages
      // an array of LLL (two arrays, one for beneficial effects and one for
      // detrimental effects.
      Effect * new_effect = new Effect();
      new_effect->set_value(value);
      new_effect->set_time_left(duration);
      if(effect == Statics::SpellEffect::HURT)
         opponent->add_effect(new_effect);
      else
         player->add_effect(new_effect);
   }
   else
   {
      // If the spell has no duration, it's an instant spell, either healing
      // or damage, routed appropriately.
      switch(target)
      {
         case Statics::Target::SELF:
            if(effect == Statics::SpellEffect::HEAL)
               player->heal(value);
            else if(effect == Statics::SpellEffect::ABSORB)
               player->apply_spell_absorb(value);
            break;

         case Statics::Target::OPPONENT:
            opponent->take_spell_damage(value);
            break;

         default:
            break;
      }
   }
}

// Displays the spell card.
void Spell::display(WINDOW * win, cchar_t border[8])
{
   int height, width;
   char temp[20];
   getmaxyx(win, height, width);

   Statics::draw_border(win, border);
   
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

// Defense class constructor
Defense::Defense() : type(Statics::DefenseType::MITIGATION) {}

// Defense class copy constructor
Defense::Defense(const Defense & defense_to_copy) :
   Card(defense_to_copy), type(defense_to_copy.type) {}

// Changes the defense type (avoidance or mitigation).
void Defense::change_type(Statics::DefenseType new_type)
{
   type = new_type;
}

// Plays the defense card. Applies the effect to the player.
void Defense::play(Player * player, Player * opponent)
{
   switch(type)
   {
      case Statics::DefenseType::MITIGATION:
         player->apply_mitigation(value);
         break;
      case Statics::DefenseType::AVOIDANCE:
         player->apply_avoidance(value);
         break;
      default:
         break;
   }
}

// Displays the defense card.
void Defense::display(WINDOW * win, cchar_t border[8])
{
   int height, width;
   char temp[20];
   getmaxyx(win, height, width);

   Statics::draw_border(win, border);
  
   // Print card name.
   mvwaddstr(win, 1, 2, name);

   // Print defense type.
   switch(type)
   {
      case Statics::DefenseType::AVOIDANCE:
         mvwaddstr(win, 2, 2, "(Avoidance)");
         break;
      case Statics::DefenseType::MITIGATION:
         mvwaddstr(win, 2, 2, "(Mitigation)");
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

