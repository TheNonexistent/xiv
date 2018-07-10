#include <iostream>
#include <ncurses.h>
#include <cctype>
#include <fstream>
#include <sstream>
#include <string>
#include <unistd.h>

#include "Include/file.h"
#include "Include/buffer.h"

using namespace std;



int main(int argc, char* argv[])
{
   //Declaring And Initalizing Control Characters
   const int FORWARD = 108;
   const int BACKWARD = 104;
   const int UPWARD = 106;
   const int DOWNWARD = 107;
   const int ENTER = 10;
   const int ESCAPE = 14;//n
   const int ITRIGGER = 105;
   const int CTRIGGER = 47;
   const int TAB = 9;

   //Delcating And Intializing Command Characters
   const int LINE = 108;//l - show line numbers
   const int SAVE = 115;//s - save the file
   const int QUIT = 113;//q - quit the program


   //Initalizing The Screen
   initscr();
   keypad(stdscr, TRUE);
   //Switching To No Echo Mode So The getch() Does Not Automatically Echo Input
   noecho();
   //NO LINE BUFFER!
   cbreak();

   //Defeining Minimum x and y
   int minY = 1, minX = 0;

   //Initializing Cursor Location Variables
   int y, x;
   //Declaring And Initializing Cursor Starting Location Constants
   const int INITX = 0,INITY = 1;
   //Moving Cursor To Starting Location
   move(INITY,INITX);


   //Open Called File
   if(argc == 2)
   {
      /*const char *outp;
      string outstr;
      ifstream infile;
      infile.open (argv[1]);
      if(infile.good())
      {
         ofilename = argv[1];
         while(!infile.eof())
         {
            getline(infile,outstr);
            outp = outstr.c_str();
            printw("%s\n", outp);
         }
         infile.close();
         */
      ifstream infile;
      infile.open (argv[1]);
      if(infile.good())
      {
         openfile(argv[1]);

      }
      else
      {
         opennewfile(argv[1]);
      }
      infile.close();
   }
   else
   {
      opennewfile("newfile");
   }

   //Feeding The Current Location To Location Variables
   y = INITY;
   x = INITX;

   //Default Mode
   bool insertmode = false;
   //Setting Variables
   bool linenumber = false;
   bool lined = false;
   //Getting The Maximum Amount Of Rows And Columns.(Screen Size)
   int maxY, maxX;
   getmaxyx(stdscr,maxY,maxX);

   //Creating The Top Window
   WINDOW * top = newwin(1,maxX ,0,0);
   refresh();
   wattron(top, A_REVERSE);
   wmove(top,0,0);
   wprintw(top, "[Filename : %s]", ofilename.c_str());
   wmove(top,0,(maxX-1)/2);
   wprintw(top, "XIV");
   wattroff(top, A_REVERSE);
   wrefresh(top);

   //Creating The Status Window
   WINDOW * status = newwin(1,maxX ,maxY - 1,0);
   refresh();
   clearstatus(maxX,status);
   wmove(status,0,0);
   wattron(status, A_REVERSE);
   wprintw(status, "[Normal Mode]");
   wattroff(status, A_REVERSE);
   wrefresh(status);


   //Moving Cursor To Starting Location
   move(INITY,INITX);

   //Moving The Cursor With Captured Input From The User
   int input;
   while(true)
   {
      //Checking For Line numbers
      if(linenumber && !lined)
      {
         int holdx = x;
         int holdy = y;
         move(0,0);
         int spaces, idigit = 0;
         int nmaxy = maxY - 2;
         int digitcount = 0; while ((nmaxy) != 0) { (nmaxy) /= 10; digitcount++; }
         int eachi;
         string spacestr;
         for(int i = minY; i < maxY-1; i++)
         {
            eachi = i;
            while (eachi != 0) { eachi /= 10; idigit++; }
            spaces = digitcount - idigit;
            move(i,0);
            spacestr = string(spaces, ' ');
            if (spaces > 0)
               printw("%s", spacestr.c_str());
            printw("%d|", i);
            idigit = 0;
         }
         lined = true;
         y = holdy;
         x = holdx + digitcount+1;
         minX = minX + digitcount + 1;
         move(y,x);
      }
      if(!linenumber && lined)
      {
            int holdx = x;
            int holdy = y;
            move(0,0);
            int spaces;
            int nmaxy = maxY - 2;
            int digitcount = 0; while ((nmaxy) != 0) { (nmaxy) /= 10; digitcount++; }
            for(int i = minY; i < maxY-1; i++)
            {
               if (digitcount > 0)
               {
                  for(int y = digitcount; y >= 0; y--)
                  {
                     move(i,y);
                     delch();
                  }
               }
            }
            lined = false;
            y = holdy;
            x = holdx + -digitcount-1;
            minX = minX - digitcount - 1;
            move(y,x);

      }
      printbuffer(minY,maxY,minX);
      move(y,x);
      input = getch();
      if(!insertmode)
      {
         switch(input)
         {
         case FORWARD:
         case KEY_RIGHT:
            if(x+1 < COLS && x+1 <= lines[y - minY].length() + minX) { x += 1; }
            break;
         case BACKWARD:
         case KEY_LEFT:
            if(x > minX) {  x -= 1; }
            break;
         case UPWARD:
         case KEY_DOWN:
            if(y+1 < LINES-1 && y+1 < lines.size()) { y += 1; }
            if(x >= lines[y - minY].length())
               x = lines[y - minY].length();
            break;
         case DOWNWARD:
         case KEY_UP:
            if(y > minY) { y -= 1; }
            if(x >= lines[y - minY].length())
               x = lines[y - minY].length();
            break;
         case ITRIGGER:
            insertmode = true;
            wattron(status, A_REVERSE);
            wmove(status,0,0);
            wprintw(status, "[Insert Mode]");
            wattroff(status, A_REVERSE);
            wrefresh(status);
            break;
          case CTRIGGER:
            wmove(status,0,0);
            clearstatus(maxX,status);
            wmove(status,0,0);
            wattron(status, A_REVERSE);
            wprintw(status, "/");
            wrefresh(status);
            wmove(status, 0, 1);
            int command = getch();
            wprintw(status,"%c" ,command);
            wattroff(status, A_REVERSE);
            switch (command)
            {
               case LINE:
               if(!linenumber)
               {
                  linenumber = true;
                  usleep(200000);
                  clearstatus(maxX,status);
                  wmove(status,0,0);
                  wattron(status, A_REVERSE);
                  wprintw(status, "[Normal Mode]");
                  wattroff(status, A_REVERSE);
               }
               else
               {
                  linenumber = false;
                  usleep(200000);
                  clearstatus(maxX,status);
                  wmove(status,0,0);
                  wattron(status, A_REVERSE);
                  wprintw(status, "[Normal Mode]");
                  wattroff(status, A_REVERSE);
               }
                  break;
               case SAVE:
               savefile(status, maxX);
               break;
               case QUIT:
               endwin();
               return 0;
               break;
               default:
               wmove(status, 0, 0);
               wattron(status, A_REVERSE);
               wprintw(status,"Command Not Found!");
               wattroff(status, A_REVERSE);
               wrefresh(status);
               usleep(2000000);
               clearstatus(maxX,status);
               wmove(status,0,0);
               wattron(status, A_REVERSE);
               wprintw(status, "[Normal Mode]");
               wattroff(status, A_REVERSE);
               break;

            }
            wrefresh(status);
            break;
         }
      }
      else
      {
         if(input == ESCAPE)
         {
            insertmode = false;
            wattron(status, A_REVERSE);
            wmove(status,0,0);
            wprintw(status, "[Normal Mode]");
            wattroff(status, A_REVERSE);
            wrefresh(status);
         }
         else if(input == KEY_RIGHT)
         {
            if(x+1 < COLS && x+1 <= lines[y - minY].length() + minX) { x += 1; }
         }
         else if(input == KEY_LEFT)
         {
            if(x > minX) {  x -= 1; }
         }
         else if(input == KEY_DOWN)
         {
            if(y+1 < LINES-1 && y+1 < lines.size()) { y += 1; }
            if(x >= lines[y - minY].length())
               x = lines[y - minY].length();
         }
         else if (input == KEY_UP)
         {
            if(y > minY) { y -= 1; }
            if(x >= lines[y - minY].length())
               x = lines[y - minY].length();
         }
         else if(input == ENTER || input == KEY_ENTER)
         {
            //Bring The Line Down From Position if not at the end of the line
            if(x < lines[y - minY].length())
            {
                //Put the rest of the line on a new line
                insertline(lines[y - minY].substr(x, lines[y - minY].length() - x), y - minY + 1);
                //Remove that part of the line
                lines[y - minY].erase(x, lines[y - minY].length() - x);
            }
            else //if at the end of the line
            {
                insertline("", y-minY+1);
            }
            if(y < maxY - 2)
            {
               x = 0;
               y += 1;
            }
         }
         else if(input == KEY_BACKSPACE)
         {
            if (x > minX)
            {
               lines[y - minY].erase(--x, 1);
            }
            else if(y > minY)
            {
                x = lines[y - minY - 1].length();
                //Bring the line down
                lines[y - minY - 1] += lines[y - minY];
                //Delete the current line
                removeline(y - minY);
                y -= 1;
            }
            move(y,x);
         }
         else if(input ==KEY_DC)
         {
            if(x == lines[y - minY].length() && y != lines.size() - 1)
               {
                   //Bring the line down
                   lines[y - minY] += lines[y - minY + 1];
                   //Delete the line
                   removeline(y+1);
               }
               else
               {
                   lines[y - minY].erase(x, 1);
               }
         }
         else if(input == TAB || input == KEY_BTAB || input == KEY_CTAB || input == KEY_CATAB || input == KEY_STAB)
         {
            lines[y - minY].insert(x, 3, ' ');
            x = x + 3;
         }
         else
         {
            lines[y - minY].insert(x, 1, char(input));
            if(x < maxX - 1)
            {
               x += 1;
            }
            else if(y < maxY - 2)
            {
               x = minX;
               y += 1;
            }
            else
            {
               x = minX;
               y = minY;
            }
         }
      }
   }


   endwin();



   return 0;
}