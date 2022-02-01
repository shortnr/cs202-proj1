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

#include "card.h"

class Node {
   private:
      Node * next;

   public:
      Card * card;

      Node(Card * new_card);
      Node(const Node& in_node);
      ~Node();

      Node * get_next();
      void set_next(Node * new_next);
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
      // the draw pile.
      void swap_decks(Deck & to_swap);

   private:
      Node * tail;
      int num_cards_total;
      int num_cards_left;

      void remove(Card * card, Node * current);
      Card * draw_card(Node * current, int steps_left);
      void delete_list(Node * current);
};

class Hand {
   public:
      Hand();
      Hand(const Hand&);
      ~Hand();

      // Inserts a card into the players hand (ALLL).
      void insert(Card * card);

      // Removes a card from the players hand.
      void remove(Card * card);

      // Finds a card in the players hand by name.
      Card * next();

      // Displays all of the cards in the players hand.
      void display(WINDOW * card_wins[]);

   private:
      Node * heads[3];
      Node * current_card;
      int current_list;
   
      void insert(Card * card, Node * current);
      void remove(Card * card, Node * current);
      Card * take_card();
      void delete_list(Node * current);
};

class ListHand
{
   public:
      ListHand();
      void insert(Card * card);

      void remove(Card * card);

      Card * begin();

      Card * next();
      
      Card * prev();

      void display(WINDOW * card_wins[]);

   private:
      std::list<Card *> cards;
      std::list<Card *>::iterator index;
};

#endif
