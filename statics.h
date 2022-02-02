//
//   File: statics.h
//   Author: Nick Short
//   Class: CS202, Project 1
//   Date: 01/12/2022
//   Description: Contains a class, Statics, whose (static) member variables
//                are used for various ASCII GUI rendering.
//

#ifndef STATICS_H_
#define STATICS_H_

// Window dimension defines.
#define CARD_WIN_BUFF 40
#define CARD_WIN_H 20
#define CARD_WIN_W 26
#define TITLE_WIN_H 15
#define TITLE_WIN_W 54
#define HEALTH_WIN_H 8
#define HEALTH_WIN_W 50
#define STATUS_WIN_H 6
#define STATUS_WIN_W 50
#define DECK_M_WIN_H 15
#define DECK_M_WIN_W 25
#define DECK_C_WIN_H 7
#define DECK_C_WIN_W 10
#define FIELD_WIN_H 21
#define FIELD_WIN_W 110

#include <string>
#include <list>
#include <ncursesw/ncurses.h>

class Statics {
   public:
      // Initializes the colors and borders for the ncurses environment.
      static void Init();

      // Draws a border, using specific unicode characters, around the
      // stdwin (standard window) in the ncurses environment.
      static void draw_border(cchar_t border[8]);

      // Draws a border, using specific unicode characters, around a 
      // specific window in the ncurses environment.
      static void draw_border(WINDOW * win, cchar_t border[8]);

      // Takes a long string and wraps it into an array of strings
      // at a certain length.
      static std::list<std::string> word_wrap(std::string to_wrap, int length);
      
      // Enumerations for different actions, spells, defenses, and
      // targets.
      enum ActionEffect {PUNCH, KICK, STAB, SHOOT};
      enum SpellEffect {HURT, HEAL};
      enum Target {SELF, OPPONENT};
      enum DefenseType {AVOID, ABSORB};
      
      // Contains definitions and functions associated with colors in
      // the ncurses environment.
      class Colors {
         public:
            // Initializes color pairs in the ncurses environment.
            static void Init();

            inline static int white_on_black = 0;
            inline static int red_on_black = 1;
            inline static int green_on_black = 2;
            inline static int black_on_red = 4;
            inline static int magenta_on_black = 5;
            inline static int blue_on_black = 6;
            inline static int yellow_on_black = 7;
      };
      
      class Borders {
         public:
            // Initializes the cchar_t arrays that handle border definitions
            // for the ncurses environment.
            static void Init();

            //static cchar_t single_line_white[8];
            //static cchar_t single_line_red[8];
            inline static cchar_t single_line_white[8];
            inline static cchar_t double_line_white[8];
            inline static cchar_t double_line_red[8];
            //static cchar_t dotted_line_white[8];
            //static cchar_t dotted_line_red[8];
            inline static cchar_t box_yellow[8];
            inline static cchar_t box_white[8];
            inline static cchar_t box_red[8];
      };

      // Painfully transcribed unicode graphics generated from large print
      // unicode character fonts from the internet. This will most likely
      // be the only graphic included in the program unless I find myself
      // with an overabundance of time on my hands at the end of the project.
      class Graphics {
         public:
            inline static const wchar_t title_graphics[] =  
               L"   \u2588\u2588\u2588\u2588\u2588\u2557       \u2588\u2588\u2588\u2588\u2588\u2588\u2557  "
                "\u2588\u2588\u2588\u2588\u2588\u2557 \u2588\u2588\u2588\u2557   \u2588\u2588\u2588"
                "\u2557\u2588\u2588\u2588\u2588\u2588\u2588\u2588\u2557\n  \u2588\u2588\u2554\u2550"
                "\u2550\u2588\u2588\u2557     \u2588\u2588\u2554\u2550\u2550\u2550\u2550\u255D \u2588"
                "\u2588\u2554\u2550\u2550\u2588\u2588\u2557\u2588\u2588\u2588\u2588\u2557 \u2588\u2588"
                "\u2588\u2588\u2551\u2588\u2588\u2554\u2550\u2550\u2550\u2550\u255D\n  \u2588\u2588"
                "\u2588\u2588\u2588\u2588\u2588\u2551     \u2588\u2588\u2551  \u2588\u2588\u2588\u2557"
                "\u2588\u2588\u2588\u2588\u2588\u2588\u2588\u2551\u2588\u2588\u2554\u2588\u2588\u2588"
                "\u2588\u2554\u2588\u2588\u2551\u2588\u2588\u2588\u2588\u2588\u2557\n  \u2588\u2588"
                "\u2554\u2550\u2550\u2588\u2588\u2551     \u2588\u2588\u2551   \u2588\u2588\u2551\u2588"
                "\u2588\u2554\u2550\u2550\u2588\u2588\u2551\u2588\u2588\u2551\u255A\u2588\u2588\u2554"
                "\u255D\u2588\u2588\u2551\u2588\u2588\u2554\u2550\u2550\u255D\n  \u2588\u2588"
                "\u2551  \u2588\u2588\u2551     \u255A\u2588\u2588\u2588\u2588\u2588\u2588\u2554\u255D"
                "\u2588\u2588\u2551  \u2588\u2588\u2551\u2588\u2588\u2551 \u255A\u2550\u255D \u2588"
                "\u2588\u2551\u2588\u2588\u2588\u2588\u2588\u2588\u2588\u2557\n  \u255A\u2550\u255D  "
                "\u255A\u2550\u255D      \u255A\u2550\u2550\u2550\u2550\u2550\u255D \u255A\u2550\u255D  "
                "\u255A\u2550\u255D\u255A\u2550\u255D     \u255A\u2550\u255D\u255A\u2550\u2550\u2550"
                "\u2550\u2550\u2550\u255D\n                                                     \n"
                " \u2588\u2588\u2557    \u2588\u2588\u2557  \u2588\u2588\u2557\u2588\u2588\u2557\u2588"
                "\u2588\u2588\u2557   \u2588\u2588\u2557\u2588\u2588\u2588\u2588\u2588\u2588\u2557  "
                "\u2588\u2588\u2588\u2588\u2588\u2557     \u2588\u2588\u2557 \n\u2588\u2588\u2554"
                "\u255D    \u2588\u2588\u2551 \u2588\u2588\u2554\u255D\u2588\u2588\u2551\u2588\u2588"
                "\u2588\u2588\u2557  \u2588\u2588\u2551\u2588\u2588\u2554\u2550\u2550\u2588\u2588\u2557"
                "\u2588\u2588\u2554\u2550\u2550\u2588\u2588\u2557    \u255A\u2588\u2588\u2557\n\u2588"
                "\u2588\u2551     \u2588\u2588\u2588\u2588\u2588\u2554\u255D \u2588\u2588\u2551\u2588"
                "\u2588\u2554\u2588\u2588\u2557 \u2588\u2588\u2551\u2588\u2588\u2551  \u2588\u2588\u2551"
                "\u2588\u2588\u2588\u2588\u2588\u2588\u2588\u2551     \u2588\u2588\u2551\n\u2588\u2588"
                "\u2551     \u2588\u2588\u2554\u2550\u2588\u2588\u2557 \u2588\u2588\u2551\u2588\u2588"
                "\u2551\u255A\u2588\u2588\u2557\u2588\u2588\u2551\u2588\u2588\u2551  \u2588\u2588\u2551"
                "\u2588\u2588\u2554\u2550\u2550\u2588\u2588\u2551     \u2588\u2588\u2551\n\u255A\u2588"
                "\u2588\u2557    \u2588\u2588\u2551  \u2588\u2588\u2557\u2588\u2588\u2551\u2588\u2588"
                "\u2551 \u255A\u2588\u2588\u2588\u2588\u2551\u2588\u2588\u2588\u2588\u2588\u2588\u2554"
                "\u255D\u2588\u2588\u2551  \u2588\u2588\u2551    \u2588\u2588\u2554\u255D\n \u255A\u2550"
                "\u255D    \u255A\u2550\u255D  \u255A\u2550\u255D\u255A\u2550\u255D\u255A\u2550\u255D  "
                "\u255A\u2550\u2550\u2550\u255D\u255A\u2550\u2550\u2550\u2550\u2550\u255D \u255A\u2550"
                "\u255D  \u255A\u2550\u255D    \u255A\u2550\u255D\n\n                    By: Nick Short";
      };
};

#endif
