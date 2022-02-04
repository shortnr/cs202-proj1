//
// File: datastructure.cpp
// Author: Nick Short
// Class: CS202, Project 1
// Date: 01/19/2022
// Description: Contains the implementations of the Deck, Hand, and StatusEffects
//              classes, as well as the node classes associated with the Deck and
//              StatusEffect classes. Deck is implemented as a circularly linked
//              list, StatusEffects is implemented as an array of linear linked
//              list, and Hand is implemented with the STL List class.

#include <iostream>
#include <ctime>
#include <cstdlib>
#include "datastructure.h"

// Deck class constructor
Deck::Deck()
{
   srand(time(0));
   tail = nullptr;
   num_cards_total = 0;
   num_cards_left = 0;
}

// Deck class copy constructor. I forgot to implement this because I didn't use
// it while developing the rest of the program. It was only when I was rereading
// the grading matrix that I remembered that it was required. By then, it was too
// late (though and attempt was made, it was resulting in SEGFAULTs, so it was
// removed so as to have a compilable/functional code.
Deck::Deck(const Deck& to_copy)
{

}

// Deck class destructor.
Deck::~Deck()
{
   if(tail != nullptr)
      delete_list(tail);
}

// Inserts a card into the Deck. Cards are inserted immediately after the tail
// so that recursion is not required. The cards are randomly drawn from the Deck
// so the order of insertion doesn't matter.
void Deck::insert(Card * card)
{
   CardNode * temp = new CardNode(card);
  
   // Empty list
   if(tail == nullptr)
   {
      tail = temp;
      tail->set_next(tail);
   }
   else // Non-empty list
   {
      temp->set_next(tail->get_next());
      tail->set_next(temp);
   }

   // Increment the cards left counter.
   num_cards_left += 1;

   // If the cards left counter is greater than the total cards counter, make
   // them equal.
   if(num_cards_left > num_cards_total)
      num_cards_total = num_cards_left;
}

// Public function to remove a card from the Deck
void Deck::remove(Card * card)
{
   remove(card, tail);

   num_cards_left -= 1;
}

// Private recursive function to remove a card from the deck.
void Deck::remove(Card * card_to_remove, CardNode * current)
{
   CardNode * temp;

   // Looking ahead so that the next node can be bypassed if there's a match.
   Card * card_to_check = current->get_next()->card;

   // If the card isn't found in the next node, recurse and keep looking.
   if(card_to_check != card_to_remove)
      remove(card_to_remove, current->get_next());
   // The card to remove is the next node. Point around it (the pointer to the
   // card is being carried by a variable in the Player class, so the data is
   // not leaked.
   else 
   {
      temp = current->get_next();
      current->set_next(temp->get_next());
      temp->set_next(nullptr);
      temp->card = nullptr;
      if(temp == tail)
         tail = current;
   }
}

// Generates a random number that is used to recursively call the function
// in order to choose a random card.
Card * Deck::draw_card()
{
   Card * drawn_card = nullptr;
   
   int random_number = rand() % num_cards_left;
   drawn_card = draw_card(tail, random_number);
   return drawn_card;
}

// Recursively chooses a random card. Each call reduces steps_left. When
// it reaches 0, the pointer to the card is returned all the way back and
// given back to the caller.
Card * Deck::draw_card(CardNode * current, int steps_left)
{
   Card * drawn_card;

   if(steps_left != 0)
      drawn_card = draw_card(current->get_next(), --steps_left);
   
   else
      drawn_card = current->card;

   return drawn_card;
}

// Displays the Deck in a series of windows. If all the possible cards are
// currently in the deck, this will print a border to each window to simulate
// a card, resulting in 5 cards displayed. Every ~20% reduction in the current
// number of cards results in one less card being displayed.
void Deck::display(WINDOW * wins[])
{
   // If there are any cards in the deck
   if(num_cards_total)
   {
      // Calculates the number of cards (and casts to int).
      int num_cards = (int)(5 * num_cards_left / num_cards_total);
      if(num_cards)
      {
         // Draws the first border. This is currently unnecessary. It was originally
         // so that "connecting characters" could be written to remove unsightly
         // gaps between cards. I eventually decided it wasn't as bad as I though,
         // but never modified the function to remove the extra lines of code.
         Statics::draw_border(wins[0], Statics::Borders::single_line_white);
         wrefresh(wins[0]);

         // Draws the remaining number of cards.
         for(int i = 1; i < num_cards; i++)
         {
            char num_total_string[10];
            sprintf(num_total_string, "%d", num_cards_total);
            Statics::draw_border(wins[i], Statics::Borders::single_line_white);
            wrefresh(wins[i]);
         }
      }
   }
}

// Swaps decks. Was used when the draw pile was empty to make the discard pile the
// new draw pile. This seemed to inject some error that was leading to a segfault
// a couple of draws after the second swap. I later decided to just use Deck pointers
// and swap the pointers instead.
void Deck::swap_decks(Deck & to_swap)
{
   CardNode * temp_tail;
   int temp_total, temp_left;

   temp_total = num_cards_total;
   temp_left = num_cards_left;
   temp_tail = tail;

   num_cards_total = to_swap.num_cards_total;
   num_cards_left = to_swap.num_cards_left;
   tail = to_swap.tail;

   to_swap.num_cards_total = temp_total;
   to_swap.num_cards_left = temp_left;
   to_swap.tail = temp_tail; 
}

// Returns true if the deck is empty.
bool Deck::is_empty()
{
   return !num_cards_left;
}

// Returns the total number of cards left in the deck. This was used for light
// debugging.
int Deck::num_total()
{
   return num_cards_total;
}

// Returns the number of cards left in the deck. This was used for light debugging.
int Deck::num_left()
{
   return num_cards_left;
}

// Deletes the circularly linked list.
void Deck::delete_list(CardNode * current)
{
   if(current->get_next() != tail)
      delete_list(current->get_next());
   current->set_next(nullptr);
   delete current;
}

// StatusEffects class constructor initializes the LLL heads to nullptr.
StatusEffects::StatusEffects()
{
   for(int i = 0; i < 2; i++)
      heads[i] = nullptr;
}

// StatusEffects class copy constructor. Similar to the Deck class copy constructor,
// I forgot to implement this and only remembered/realized it was needed when there
// wasn't enough time left.
StatusEffects::StatusEffects(const StatusEffects&)
{

}

// StatusEffects class destructor.
StatusEffects::~StatusEffects()
{
   for(EffectNode * head : heads)
      if(head != nullptr)
         delete_list(head);
}

// Unimplemented. See datastructure.h:86 for explanation.
void StatusEffects::insert(Effect * effect)
{

}

// Unimplemented. See datastructure.h:86 for explanation.
void StatusEffects::remove(EffectNode * node)
{
   
}

// Unimplemented. See datastructure.h:86 for explanation.
void StatusEffects::display(WINDOW * win)
{
   
}

// Unimplemented. See datastructure.h:86 for explanation.
void StatusEffects::delete_list(EffectNode * current)
{
   if(current->get_next() != nullptr)
      delete_list(current->get_next());
   current->set_next(nullptr);
   delete current;
}

// Hand class constructor. This is implemented with an STL List, so the copy constructor
// is already defined within that class.
Hand::Hand()
{
   index = cards.begin();
}

// Inserts a card into the Hand
void Hand::insert(Card * card)
{
   cards.push_front(card);
}

// Removes a card from the Hand.
void Hand::remove(Card * card)
{
   cards.remove(card);
}

// Sets the list iterator to the beginning of the list.
Card * Hand::begin()
{
   index = cards.begin();
   return *index;
}

// Sets the list iterator to the next item in the list.
Card * Hand::next()
{
      index = std::next(index, 1);

      return *index;
}

// Sets the list iterator to the previous item in the list.
Card * Hand::prev()
{
      index = std::prev(index, 1);

      return *index;
}

// Displays the hand in the middle of the screen. Each players hand has its own
// array of curses WINDOWs that occupy the same space on the screen. This could
// be simplified by sharing the same array.
void Hand::display(WINDOW * card_wins[5])
{
   int i = 0;
   std::list<Card *>::iterator it;

   // Iterates through each WINDOW, calling the display function of the card
   // that the list iterator is pointing to.
   for(it = cards.begin(); it != cards.end(); ++it)
   {
      werase(card_wins[i]);
      (*it)->display(card_wins[i]);
      wrefresh(card_wins[i]);
      ++i;
   }
}

// CardNode constructor.
CardNode::CardNode(Card * new_card) : card(new_card), next(nullptr) {}

// CardNode copy constructor.
CardNode::CardNode(const CardNode& in_node) : card(in_node.card), next(nullptr) {}

// CardNode destructor.
CardNode::~CardNode()
{
   delete card;
}

// CardNode getter.
CardNode * CardNode::get_next() const
{
   if(next)
      return next;
   else
      return nullptr;
}

// CardNode setter.
void CardNode::set_next(CardNode * new_next)
{
   next = new_next;
}

// EffectNode constructor
EffectNode::EffectNode() : next(nullptr), effect(nullptr) {}

// EffectNode copy constructor
EffectNode::EffectNode(const EffectNode& in_node) : next(nullptr)
{
   effect = new Effect(*in_node.get_effect());
}

// EffectNode destructor
EffectNode::~EffectNode()
{
   if(effect)
      delete effect;

   effect = nullptr;
}

// EffectNode getter.
EffectNode * EffectNode::get_next() const
{
   return next;
}

// EffectNode setter.
void EffectNode::set_next(EffectNode * in_next)
{
   next = in_next;
}

// Returns the effect assocated with the EffectNode
Effect * EffectNode::get_effect() const
{
   return effect;
}

// Sets the effect associated with the EffectNode
void EffectNode::set_effect(Effect * in_effect)
{
   effect = in_effect;
}

// Decrements the duration of the effect
void EffectNode::tick()
{
   effect->set_time_left(effect->get_time_left() - 1);
}

// Sets the value of the effect
void Effect::set_value(int new_value)
{
   value = new_value;
}

// Sets the time left (duration) of the effect
void Effect::set_time_left(int new_time_left)
{
   time_left = new_time_left;
}

// Sets whether or not the effect is a damaging effect.
void Effect::set_damaging_effect(bool new_damaging_effect)
{
   damaging_effect = new_damaging_effect;
}

// Gets the value of the effect
int Effect::get_value()
{
   return value;
}

// Gets the time left of the effect
int Effect::get_time_left()
{
   return time_left;
}

// Gets whether the effect is damaging or not.
bool Effect::get_damaging_effect()
{
   return damaging_effect;
}
