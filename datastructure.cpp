#include <iostream>
#include <ctime>
#include <cstdlib>
#include "datastructure.h"

Deck::Deck()
{
   srand(time(0));
   tail = nullptr;
   num_cards_total = 0;
   num_cards_left = 0;
}

Deck::Deck(const Deck&)
{

}

Deck::~Deck()
{
   if(tail != nullptr)
      delete_list(tail);
}

void Deck::insert(Card * card)
{
   //if(!card)
      //return;

   Node * temp = new Node(card);
   
   if(tail == nullptr)
   {
      tail = temp;
      tail->set_next(tail);
   }
   else
   {
      temp->set_next(tail->get_next());
      tail->set_next(temp);
   }

   num_cards_left += 1;

   if(num_cards_left > num_cards_total)
      num_cards_total = num_cards_left;
}

void Deck::remove(Card * card)
{
   remove(card, tail);

   num_cards_left -= 1;
}

void Deck::remove(Card * card_to_remove, Node * current)
{
   Node * temp;

   if(current->get_next()->card != card_to_remove)
      remove(card_to_remove, current->get_next());
   else
   {
      temp = current->get_next();
      current->set_next(temp->get_next());
      temp->set_next(nullptr);
      temp->card = nullptr;
      if(temp == tail)
         tail = current;
      delete temp;
   }
}

Card * Deck::draw_card()
{
   Card * drawn_card;
   if(!num_cards_left)
   {
      drawn_card = nullptr;
   }
   else
   {
      int random_number = rand() % num_cards_left;
      drawn_card = draw_card(tail, random_number);
      remove(drawn_card);
   }
   return drawn_card;
}

Card * Deck::draw_card(Node * current, int steps_left)
{
   Card * drawn_card;

   if(steps_left != 0)
      drawn_card = draw_card(current->get_next(), --steps_left);
   
   else
      drawn_card = current->card;

   return drawn_card;
}

void Deck::display(WINDOW * wins[])
{
   int num_cards = (int)(5 * num_cards_left / num_cards_total);
   if(num_cards)
   {
      Statics::draw_border(wins[0], Statics::Borders::single_line_white);
      wrefresh(wins[0]);

      for(int i = 1; i < num_cards; i++)
      {
         char num_total_string[10];
         sprintf(num_total_string, "%d", num_cards_total);
         Statics::draw_border(wins[i], Statics::Borders::single_line_white);
         wrefresh(wins[i]);
         // Add connecting characters
      }
   }
}

void Deck::swap_decks(Deck & to_swap)
{
   Node * temp_tail;
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

void Deck::delete_list(Node * current)
{
   if(current->get_next() != tail)
      delete_list(current->get_next());
   current->set_next(nullptr);
   delete current;
}

Hand::Hand()
{
   for(int i = 0; i < 3; i++)
      heads[i] = nullptr;

   current_list = 0;
   current_card = nullptr;
}

Hand::Hand(const Hand&)
{

}

Hand::~Hand()
{
   for(Node * head : heads)
      if(head != nullptr)
         delete_list(head);
}

void Hand::insert(Card * card)
{
   int card_type = -1; 

   Action * action = dynamic_cast<Action *>(card);
   Spell * spell = dynamic_cast<Spell *>(card);
   Defense * defense = dynamic_cast<Defense *>(card);

   if(action != nullptr)
      card_type = 0;
   else if(spell != nullptr)
      card_type = 1;
   else if(defense != nullptr)
      card_type = 2;

   if(card_type < 0)
   {
      std::cout << "Invalid Card Type" << std::endl;
   }
   else
   {
      Node * new_card = new Node(card);
      if(heads[card_type] != nullptr);
         new_card->set_next(heads[card_type]);
      heads[card_type] = new_card;
   }
}

/*
void Hand::insert(Card * card, Node * current)
{
   Node * new_card = new Node(card);
   new_card->set_next(head
}
*/

void Hand::remove(Card * card)
{
   int card_type = -1;

   Action * action = dynamic_cast<Action *>(card);
   Spell * spell = dynamic_cast<Spell *>(card);
   Defense * defense = dynamic_cast<Defense *>(card);

   if(action != nullptr)
      card_type = 0;
   else if(spell != nullptr)
      card_type = 1;
   else if(defense != nullptr)
      card_type = 2;

   if(card_type < 0)
   {
      std::cout << "Invalid Card Type" << std::endl;
   }
   else
   {
      remove(card, heads[card_type]);
   }
}

void Hand::remove(Card * card, Node * current)
{
   
}

Card * Hand::next()
{
   if(current_card == nullptr)
      current_card == heads[0];

   if(current_card->get_next() == nullptr)
   {
      current_list += 1;

      if(current_list > 3)
         current_list == 0;

      current_card = heads[current_list];
   }
   else
      current_card = current_card->get_next();

   return current_card->card;
}

void Hand::display(WINDOW * card_wins[])
{
   
}

Card * Hand::take_card()
{
   Card * card;
   return card;
}

void Hand::delete_list(Node * current)
{
   if(current->get_next() != nullptr)
      delete_list(current->get_next());
   current->set_next(nullptr);
   delete current;
}

ListHand::ListHand()
{
   index = cards.begin();
}

void ListHand::insert(Card * card)
{
   cards.push_front(card);
}

void ListHand::remove(Card * card)
{
   cards.remove(card);
}

Card * ListHand::begin()
{
   index = cards.begin();
   return *index;
}

Card * ListHand::next()
{
   if(index != cards.end())
      ++index;

   if(dynamic_cast<Card *>(*index))
      return *index;
   else
      return nullptr;
}

Card * ListHand::prev()
{
   if(index != cards.begin())
      --index;

   if(dynamic_cast<Card *>(*index))
      return *index;
   else
      return nullptr;
}

void ListHand::display(WINDOW * card_wins[5])
{
   int i = 0;

   for(index = cards.begin(); index != cards.end(); ++index)
   {
      (*index)->display(card_wins[i]);
      wrefresh(card_wins[i]);
      ++i;
   }

   index = cards.end();
}

Node::Node(Card * new_card) : card(new_card), next(nullptr) {}

Node::Node(const Node& in_node) : card(in_node.card), next(nullptr) {}

Node::~Node()
{
   delete card;
}

Node * Node::get_next()
{
   if(next)
      return next;
   else
      return nullptr;
}

void Node::set_next(Node * new_next)
{
   next = new_next;
}

