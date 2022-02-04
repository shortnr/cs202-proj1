// 
// File: datastrutures.h
// Author: Nick Short
// Class: CS202, Project 1
// Date: 01/19/2022
// Description: Contains the class definitions for the Deck class, implementing
//              a CLL, and the Hand class, implementing an array of LLL.
//

#ifndef DATASTRUCTURE_H_
#define DATASTRUCTURE_H_

#include "effect.h"
#include "card.h"

class CardNode {
   private:
      CardNode * next;

   public:
      Card * card;

      CardNode(Card * new_card);
      CardNode(const CardNode& in_node);
      ~CardNode();

      CardNode * get_next() const;
      void set_next(CardNode * new_next);
};

// The Deck is the CLL containing all of the Cards available to the player.
class Deck {
   public:
      Deck();
      Deck(const Deck&);
      ~Deck();

      // Inserts a dynamically created card into the CLL.
      void insert(Card * card);

      // Removes a card from the CLL.
      void remove(Card * card);

      // Draws a card using a random number. A number is generated between 0
      // and num_cards_left - 1 and the CLL is stepped through that many nodes.
      // The final node reached will be returned.
      Card * draw_card();

      // Displays a stack of cards (0-5) proportional to the percentage of cards
      // left in the deck (i.e. a 20 card deck with 12 cards left will display 3
      // cards.
      void display(WINDOW * wins[]);
      
      // Swaps deck with another one. Used to "shuffle" the junk pile back into
      // the draw pile. Removed in favor of converting Decks to pointers to deck
      // objects, then just swapping their pointers. All for naught, though, as
      // the two deck (draw and discard) system proved difficult to manage, so
      // played cards are just reinserted into the draw pile. This comes at a risk
      // of drawing the card that was just played, but it's a risk I'll accept.
      void swap_decks(Deck & to_swap);

      // Returns true if there are no more cards in the deck.
      bool is_empty();

      // Returns the total number of cards in the deck.
      int num_total();

      // Returns the number of cards left in the deck.
      int num_left();

   private:
      CardNode * tail;
      int num_cards_total;
      int num_cards_left;

      // Recursively removes a card from the Deck.
      void remove(Card * card, CardNode * current);

      // Recursively draws a random card from the Deck.
      Card * draw_card(CardNode * current, int steps_left);

      // Recursively deletes the deck.
      void delete_list(CardNode * current);
};

// StatusEffects class to contain an array of linked lists of
// status effects. Unfortunately, much of this class has been left inimplemented.
// this is partially due to a "late game" decision to rework the Hand class using
// an STL List DS, which pidgeonholed the StatusEffects class into an array of
// linear linked list. This class was originally going to be implemented as an
// STL Vector.
class StatusEffects {
   public:
      StatusEffects();
      StatusEffects(const StatusEffects&);
      ~StatusEffects();

      // Inserts a card into the players hand (ALLL).
      void insert(Effect * effect);

      // Removes a card from the players hand.
      void remove(EffectNode * node);

      // Finds a card in the players hand by name.
      Effect * next();

      // Displays all of the cards in the players hand.
      void display(WINDOW * win);

      enum Type
      {
         POSITIVE,
         NEGATIVE
      };

   private:
      EffectNode * heads[2];
  
      // Recursive functions called by their public counterparts.
      void insert(Effect * status, EffectNode * current);
      void remove(Effect * status, EffectNode * current);
      void apply_effect(Effect * effect);
      void delete_list(EffectNode * current);
};

// Hand class. Implemented with an STL list DS. This was chosen to make
// it easier to navigate forward and backward through the player hand.
class Hand
{
   public:
      Hand();

      // Inserts a card into the list.
      void insert(Card * card);

      // Removes a card from the list.
      void remove(Card * card);

      // Points the list iterator to the beginning of the list.
      Card * begin();

      // Points the list iterator to the next item in the list.
      Card * next();

      // Points the list iterator to the previous item in the list.
      Card * prev();

      // Displays the player's hand in its curses window.
      void display(WINDOW * card_wins[]);

   private:
      // The STL list DS member.
      std::list<Card *> cards;

      // The list iterator.
      std::list<Card *>::iterator index;
};

#endif
