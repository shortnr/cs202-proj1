// 
// File: status.h
// Author: Nick Short
// Class: CS202, Project 1
// Date: 01/24/2022
// Description: Contains the class definitions for the Status class.
//              The Status class contains the information for status
//              effects that deal or heal damage over time.
//

#ifndef STATUS_H_
#define STATUS_H_

class Status {
   public:
      int time_left;
      int value;
      bool damaging_effect;
      std::string text;
}
