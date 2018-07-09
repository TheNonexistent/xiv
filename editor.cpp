#include <iostream>
#include <ncurses.h>
#include <cctype>
#include <fstream>
#include <string>

#include "Include/file.h"

using namespace std;


int main(int argc, char* argv[])
{
   //Declaring And Initalizing Control Characters
   const int FORWARD = 108;
   const int BACKWARD = 104;
   const int UPWARD = 106;
   const int DOWNWARD = 107;
   const int ENTER = 10;
   const int ESCAPE = 27;
   const int ITRIGGER = 105;
   //Initalizing The Screen
   initscr();
   //Switching To No Echo Mode So The getch() Does Not Automatically Echo Input
   noecho();

/*
   if(argc == 2)
   {
      const char *outp;
      string STRING;
      ifstream infile;
      infile.open (argv[1]);
          while(!infile.eof())
          {
              getline(infile,STRING);
              outp = STRING.c_str();
              printw("%s\n", outp);
          }
      infile.close();
   }
   else
   {
      //start new file
   }
*/

   //Initializing Cursor Location Variables
   int y, x;
   //Declaring And Initializing Cursor Starting Location Constants
   const int INITX = 0,INITY = 0;
   //Moving Cursor To Starting Location
   move(INITY,INITX);
   //Feeding The Current Location To Location Variables
   y = INITY;
   x = INITX;

   //Default Mode
   bool insertmode = false;
   //Getting The Maximum Amount Of Rows And Columns.(Screen Size)
   int maxY, maxX;
   getmaxyx(stdscr,maxY,maxX);

   //Creating The Top Window
   WINDOW * top = newwin(2,maxX ,0,0);
   refresh();
   wattron(top, A_REVERSE);
   wmove(top,0,(maxX-1)/2);
   wprintw(top, "XIV");
   wattroff(top, A_REVERSE);
   wrefresh(top);

   //Creating The Status Window
   WINDOW * status = newwin(2,maxX ,maxY - 1,0);
   refresh();
   wattron(status, A_REVERSE);
   wmove(status,0,0);
   wprintw(status, "[Normal Mode]");
   wattroff(status, A_REVERSE);
   wrefresh(status);

   //Moving Cursor To Starting Location
   move(INITY,INITX);

   //Moving The Cursor With Captured Input From The User
   int input;
   while(true)
   {
      input = getch();
      if(!insertmode)
      {
         if(input == FORWARD)
         {
            if(x < maxX - 1) { x += 1; }
         }
         else if(input == BACKWARD)
         {
            if(x > 0) {  x -= 1; }
         }
         else if(input == UPWARD)
         {
            if(y < maxY - 2) { y += 1; }
         }
         else if(input == DOWNWARD)
         {
            if(y > 0) { y -= 1; }
         }
         else if(input == ITRIGGER)
         {
            insertmode = true;
            wattron(status, A_REVERSE);
            wmove(status,0,0);
            wprintw(status, "[Insert Mode]");
            wattroff(status, A_REVERSE);
            wrefresh(status);
         }
      }
      else
      {
         if(input == ENTER)
         {
         	if(y < maxY - 2)
         	{
               y += 1;
         	   x = 0;
         	}
         	else
         	{
               y = 0;
         	   x = 0;
         	}
         }
         else if(input == ESCAPE)
         {
            insertmode = false;
            wattron(status, A_REVERSE);
            wmove(status,0,0);
            wprintw(status, "[Normal Mode]");
            wattroff(status, A_REVERSE);
            wrefresh(status);
         }
         else
         {
            printw("%c", input);
         	if(x < maxX - 1)
         	{
               x += 1;
         	}
         	else if(y < maxY - 2)
         	{
               x = 0;
               y += 1;
         	}
         	else
         	{
               x = 0;
         	   y = 0;
         	}
         }
      }
      move(y,x);
   }

   getch();

   endwin();



   return 0;
}
