// 
// File: effect.h
// Author: Nick Short
// Class: CS202, Project 1
// Date: 01/24/2022
// Description: Contains the class definitions for the Effect class.
//              The Effect class contains the information for status
//              effects that deal or heal damage over time.
//

#ifndef EFFECT_H_
#define EFFECT_H_

// Effect class. Used to implement effects with durations.
class Effect {
   private:
      int time_left;
      int value;
      bool damaging_effect;

   public:
      void set_value(int new_value);
      void set_time_left(int new_time_left);
      void set_damaging_effect(bool new_damaging_effect);
      int get_value();
      int get_time_left();
      bool get_damaging_effect();
};

// The nodes contained within the StatusEffect ALLL
class EffectNode
{
   public:
      EffectNode();
      EffectNode(const EffectNode& in_node);
      ~EffectNode();

      EffectNode * get_next() const;
      void set_next(EffectNode * in_next);
      Effect * get_effect() const;
      void set_effect(Effect * in_effect);
      void tick();

   private:
      EffectNode * next;
      Effect * effect;
};

#endif
