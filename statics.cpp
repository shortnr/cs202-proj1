#include <iostream>
#include <string>
#include "statics.h"

void Statics::Init() {
   Borders::Init();
   Colors::Init();
}

void Statics::Borders::Init() {
   setcchar(&single_line_white[0], L"\u2502\0", 0, Statics::Colors::white_on_black, nullptr);
   setcchar(&single_line_white[1], L"\u2502\0", 0, Statics::Colors::white_on_black, nullptr);
   setcchar(&single_line_white[2], L"\u2500\0", 0, Statics::Colors::white_on_black, nullptr);
   setcchar(&single_line_white[3], L"\u2500\0", 0, Statics::Colors::white_on_black, nullptr);
   setcchar(&single_line_white[4], L"\u250C\0", 0, Statics::Colors::white_on_black, nullptr);
   setcchar(&single_line_white[5], L"\u2510\0", 0, Statics::Colors::white_on_black, nullptr);
   setcchar(&single_line_white[6], L"\u2514\0", 0, Statics::Colors::white_on_black, nullptr);
   setcchar(&single_line_white[7], L"\u2518\0", 0, Statics::Colors::white_on_black, nullptr);

   setcchar(&double_line_white[0], L"\u2551\0", 0, Statics::Colors::white_on_black, nullptr);
   setcchar(&double_line_white[1], L"\u2551\0", 0, Statics::Colors::white_on_black, nullptr);
   setcchar(&double_line_white[2], L"\u2550\0", 0, Statics::Colors::white_on_black, nullptr);
   setcchar(&double_line_white[3], L"\u2550\0", 0, Statics::Colors::white_on_black, nullptr);
   setcchar(&double_line_white[4], L"\u2554\0", 0, Statics::Colors::white_on_black, nullptr);
   setcchar(&double_line_white[5], L"\u2557\0", 0, Statics::Colors::white_on_black, nullptr);
   setcchar(&double_line_white[6], L"\u255A\0", 0, Statics::Colors::white_on_black, nullptr);
   setcchar(&double_line_white[7], L"\u255D\0", 0, Statics::Colors::white_on_black, nullptr); 

   setcchar(&double_line_red[0], L"\u2551\0", 0, Statics::Colors::red_on_black, nullptr);
   setcchar(&double_line_red[1], L"\u2551\0", 0, Statics::Colors::red_on_black, nullptr);
   setcchar(&double_line_red[2], L"\u2550\0", 0, Statics::Colors::red_on_black, nullptr);
   setcchar(&double_line_red[3], L"\u2550\0", 0, Statics::Colors::red_on_black, nullptr);
   setcchar(&double_line_red[4], L"\u2554\0", 0, Statics::Colors::red_on_black, nullptr);
   setcchar(&double_line_red[5], L"\u2557\0", 0, Statics::Colors::red_on_black, nullptr);
   setcchar(&double_line_red[6], L"\u255A\0", 0, Statics::Colors::red_on_black, nullptr);
   setcchar(&double_line_red[7], L"\u255D\0", 0, Statics::Colors::red_on_black, nullptr);

   setcchar(&box_white[0], L"\u2588\0", 0, Statics::Colors::white_on_black, nullptr);
   setcchar(&box_white[1], L"\u2588\0", 0, Statics::Colors::white_on_black, nullptr);
   setcchar(&box_white[2], L"\u2584\0", 0, Statics::Colors::white_on_black, nullptr);
   setcchar(&box_white[3], L"\u2580\0", 0, Statics::Colors::white_on_black, nullptr);
   setcchar(&box_white[4], L"\u2584\0", 0, Statics::Colors::white_on_black, nullptr);
   setcchar(&box_white[5], L"\u2584\0", 0, Statics::Colors::white_on_black, nullptr);
   setcchar(&box_white[6], L"\u2580\0", 0, Statics::Colors::white_on_black, nullptr);
   setcchar(&box_white[7], L"\u2580\0", 0, Statics::Colors::white_on_black, nullptr);

   setcchar(&box_red[0], L"\u2588\0", 0, Statics::Colors::red_on_black, nullptr);
   setcchar(&box_red[1], L"\u2588\0", 0, Statics::Colors::red_on_black, nullptr);
   setcchar(&box_red[2], L"\u2584\0", 0, Statics::Colors::red_on_black, nullptr);
   setcchar(&box_red[3], L"\u2580\0", 0, Statics::Colors::red_on_black, nullptr);
   setcchar(&box_red[4], L"\u2584\0", 0, Statics::Colors::red_on_black, nullptr);
   setcchar(&box_red[5], L"\u2584\0", 0, Statics::Colors::red_on_black, nullptr);
   setcchar(&box_red[6], L"\u2580\0", 0, Statics::Colors::red_on_black, nullptr);
   setcchar(&box_red[7], L"\u2580\0", 0, Statics::Colors::red_on_black, nullptr);

   setcchar(&box_yellow[0], L"\u2588\0", 0, Statics::Colors::yellow_on_black, nullptr);
   setcchar(&box_yellow[1], L"\u2588\0", 0, Statics::Colors::yellow_on_black, nullptr);
   setcchar(&box_yellow[2], L"\u2584\0", 0, Statics::Colors::yellow_on_black, nullptr);
   setcchar(&box_yellow[3], L"\u2580\0", 0, Statics::Colors::yellow_on_black, nullptr);
   setcchar(&box_yellow[4], L"\u2584\0", 0, Statics::Colors::yellow_on_black, nullptr);
   setcchar(&box_yellow[5], L"\u2584\0", 0, Statics::Colors::yellow_on_black, nullptr);
   setcchar(&box_yellow[6], L"\u2580\0", 0, Statics::Colors::yellow_on_black, nullptr);
   setcchar(&box_yellow[7], L"\u2580\0", 0, Statics::Colors::yellow_on_black, nullptr);
}

void Statics::Colors::Init() {
   init_pair(white_on_black, COLOR_WHITE, COLOR_BLACK);
   init_pair(red_on_black, COLOR_RED, COLOR_BLACK);
   init_pair(green_on_black, COLOR_GREEN, COLOR_BLACK);
   init_pair(black_on_red, COLOR_BLACK, COLOR_RED);
   init_pair(magenta_on_black, COLOR_MAGENTA, COLOR_BLACK);
   init_pair(blue_on_black, COLOR_BLUE, COLOR_BLACK);
}

void Statics::draw_border(cchar_t border[8]) {
    border_set(&border[0], &border[1], &border[2], &border[3],
               &border[4], &border[5], &border[6], &border[7]);
}

void Statics::draw_border(WINDOW * win, cchar_t border[8]) {
    wborder_set(win, &border[0], &border[1], &border[2], &border[3],
               &border[4], &border[5], &border[6], &border[7]);
}

std::list<std::string> Statics::word_wrap(std::string to_wrap, int length)
{
   std::list<std::string> wrapped_string;
   std::string temp_string = to_wrap;
   std::string removed_string;
   std::size_t index = 0, temp = 0;
   bool found = false;

   while(temp_string.length() > length)
   {
      while(!found)
      {
         temp = temp_string.find(' ', index + 1);
         if(temp < length)
            index = temp;
         else
            found = true;
      }
      removed_string = temp_string.substr(0, index);
      wrapped_string.push_back(removed_string);
      temp_string = temp_string.erase(0, (int)index + 1);
      temp = 0;
      index = 0;
      found = false;
   }
   wrapped_string.push_back(temp_string);

   return wrapped_string;
}







