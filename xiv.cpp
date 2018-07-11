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
   const int BACKSPACE = 127;
   const int ESCAPE = 14;//n
   const int ITRIGGER = 105;
   const int CTRIGGER = 47;
   const int TAB = 9;

   //Delcating And Intializing Command Characters
   const int LINE = 108;//l - show line numbers
   const int SAVE = 115;//s - save the file
   const int QUIT = 113;//q - quit the program
   const int CANCEL = 99;//c - cancel command input
   const int BEGINNING = 98;//b - go to beginning
   const int END = 101;//e - go to end




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
   int starty = 0;
   int startx = 0;
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
      //printbuffer(int starty,int startxint minY,int maxY, int minX)
      int linenumber = printbuffer(starty,startx,minY,maxY,minX,maxX);
      refresh();
      wattron(top, A_REVERSE);
      wmove(top,0,0);
      wprintw(top, "[Filename : %s]", ofilename.c_str());
      wprintw(top,"(%dL)", linenumber);
      wmove(top,0,(maxX-1)/2);
      wprintw(top, "XIV");
      wattroff(top, A_REVERSE);
      wrefresh(top);
      move(y,x);
      input = getch();
      if(!insertmode)
      {
         switch(input)
         {
         case FORWARD:
         case KEY_RIGHT:
            if(x+1 < COLS) { if(x+1 <= lines[y - minY + (starty)].length() + minX - startx) { x += 1; } } else { if(startx + maxX < lines[y - minY + (starty)].length() + 1) { startx++; } }
            break;
         case BACKWARD:
         case KEY_LEFT:
            if(x > minX) {  x -= 1; } else if(startx > 0) { startx--; }
            break;
         case UPWARD:
         case KEY_DOWN:
            if(y+1 < LINES-1) { if(y+1 < lines.size() - starty) { y += 1; } } else if(y+1 < lines.size() - starty) { if(starty < lines.size()) { starty++;} }
            if(x + startx >= lines[y - minY + (starty)].length())
               x = lines[y - minY + (starty)].length();
            if(x + startx >= lines[y - minY + (starty)].length() && startx > 0)
            {
               if(lines[y - minY + (starty)].length() > maxX)
               {
                  startx = lines[y - minY + (starty)].length() - 1;
                  x = 1;
               }
               else
               {
               startx = 0;
               x = lines[y - minY + (starty)].length();
               }
            }
            break;
         case DOWNWARD:
         case KEY_UP:
            if(y > minY) { y -= 1; } else { if(starty > minY) { starty--;} }
            if(x + startx >= lines[y - minY + (starty)].length())
               x = lines[y - minY + (starty)].length();
            if(x + startx >= lines[y - minY + (starty)].length() && startx > 0)
            {
               if(lines[y - minY + (starty)].length() > maxX)
               {
                  startx = lines[y - minY + (starty)].length() - 1;
                  x = 1;
               }
               else
               {
               startx = 0;
               x = lines[y - minY + (starty)].length();
               }
            }
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
               case CANCEL:
               clearstatus(maxX,status);
               wmove(status,0,0);
               wattron(status, A_REVERSE);
               wprintw(status, "[Normal Mode]");
               wattroff(status, A_REVERSE);
               break;
               case BEGINNING:
               startx = 0;
               starty = 0;
               x = minX;
               y = minY;
               clearstatus(maxX,status);
               wmove(status,0,0);
               wattron(status, A_REVERSE);
               wprintw(status, "[Normal Mode]");
               wattroff(status, A_REVERSE);
               break;
               case END:
               if(lines.size() > maxY)
               {
               starty = lines.size() - maxY + 1;
               y = lines.size() - starty - minY;
               }
               else
               {
                  starty = 0;
                  y = lines.size() - minY;
               }
               /*
               if(lines[y - minY + (starty)].length() > maxX)
               {
                  startx = lines[y - minY + (starty)].length() - 1;
                  x = 1;
               }
               else
               {
               startx = 0;
               x = lines[y - minY + (starty)].length();
               }
               */
               startx = 0;
               x= minX;
               clearstatus(maxX,status);
               wmove(status,0,0);
               wattron(status, A_REVERSE);
               wprintw(status, "[Normal Mode]");
               wattroff(status, A_REVERSE);
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
            if(x+1 < COLS) { if(x+1 <= lines[y - minY + (starty)].length() + minX - startx) { x += 1; } } else { if(startx + maxX < lines[y - minY + (starty)].length() + 1) { startx++; } }
         }
         else if(input == KEY_LEFT)
         {
            if(x > minX) {  x -= 1; } else if(startx > 0) { startx--; }
         }
         else if(input == KEY_DOWN)
         {
            if(y+1 < LINES-1) { if(y+1 < lines.size() - starty) { y += 1; } } else if(y+1 < lines.size() - starty) { if(starty < lines.size()) { starty++;} }
            if(x + startx >= lines[y - minY + (starty)].length())
               x = lines[y - minY + (starty)].length();
            if(x + startx >= lines[y - minY + (starty)].length() && startx > 0)
            {
               if(lines[y - minY + (starty)].length() > maxX)
               {
                  startx = lines[y - minY + (starty)].length() - 1;
                  x = 1;
               }
               else
               {
               startx = 0;
               x = lines[y - minY + (starty)].length();
               }
            }
         }
         else if (input == KEY_UP)
         {
            if(y > minY) { y -= 1; } else { if(starty > minY) { starty--;} }
            if(x + startx >= lines[y - minY + (starty)].length())
               x = lines[y - minY + (starty)].length();
            if(x + startx >= lines[y - minY + (starty)].length() && startx > 0)
            {
               if(lines[y - minY + (starty)].length() > maxX)
               {
                  startx = lines[y - minY + (starty)].length() - 1;
                  x = 1;
               }
               else
               {
               startx = 0;
               x = lines[y - minY + (starty)].length();
               }
            }
         }
         else if(input == ENTER || input == KEY_ENTER)
         {
            //Bring The Line Down From Position if not at the end of the line
            if(x + startx < lines[y - minY + (starty)].length())
            {
                //Put the rest of the line on a new line
                insertline(lines[y - minY + (starty)].substr(x + startx, lines[y - minY + (starty)].length() - (x+startx) ), y - minY + 1 + (starty));
                //Remove that part of the line
                lines[y - minY + (starty)].erase(x + startx, lines[y - minY + (starty)].length() - (x+startx));
                startx = 0;
            }
            else //if at the end of the line
            {
                insertline("", y-minY+1 + (starty));
                startx = 0;
            }
            if(y < maxY - 2)
            {
               x = 0;
               y += 1;
            }
            else
            {
               starty++;
               x = 0;
            }
         }
         else if(input == KEY_BACKSPACE || input == BACKSPACE)
         {
            if (x > minX)
            {
               lines[y - minY + (starty)].erase(--x + startx, 1);
            }
            else if(y > minY && startx == 0)
            {
                if (lines[y - minY - 1 + (starty)].length() > maxX)
                {
                startx = lines[y - minY - 1 + (starty)].length() - maxX + lines[y - minY + (starty)].length();
                }
                x = lines[y - minY - 1 + (starty)].length() - startx;
                //Bring the line down
                lines[y - minY - 1 + (starty)] += lines[y - minY + (starty)];
                //Delete the current line
                removeline(y - minY + (starty));
                y -= 1;
            }
            else if(starty > minY && startx == 0)
            {
               if (lines[y - minY - 1 + (starty)].length() > maxX)
               {
               startx = lines[y - minY - 1 + (starty)].length() - maxX + lines[y - minY + (starty)].length();
               }
               x = lines[y - minY - 1 + (starty)].length() - startx;
               //Bring the line down
               lines[y - minY - 1 + (starty)] += lines[y - minY + (starty)];
               //Delete the current line
               removeline(y - minY + (starty));
               starty -= 8;
               y += 7;

            }
            if(x == minX && startx > 0)
            {
               lines[y - minY + (starty)].erase(x + startx, 1);
               startx--;
            }
            move(y,x);
         }
         else if(input ==KEY_DC)
         {
            if(x == lines[y - minY + (starty)].length() && y != lines.size() - 1)
               {
                   //Bring the line down
                   lines[y - minY + (starty)] += lines[y - minY + 1];
                   //Delete the line
                   removeline(y+1);
               }
               else
               {
                   lines[y - minY + (starty)].erase(x + startx, 1);
               }
         }
         else if(input == TAB || input == KEY_BTAB || input == KEY_CTAB || input == KEY_CATAB || input == KEY_STAB)
         {
            lines[y - minY + (starty)].insert(x + startx, 3, ' ');
            if(x < maxX - 4)
            {
               x = x + 3;
            }
            else
            {
               startx = startx + 3;
            }

         }
         else
         {
            lines[y - minY + (starty)].insert(x + startx, 1, char(input));
            //Implement H Scrolling.
            if(x < maxX - 1)
            {
               x += 1;
            }
            else
            {
               startx++;
            }
         }
      }
   }


   endwin();



   return 0;
}
