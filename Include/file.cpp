#include <iostream>
#include <fstream>
#include <string>
#include <ncurses.h>

#include "file.h"

using namespace std;

string ofilename;
string ofilepath;

void opennewfile(string filename)
{
    ofilename = filename;
    ofilepath = filename;
}

void openfile(string filename)
{
   ofilename = filename;
   ofilepath = filename;
}

void clearstatus(int maxx, WINDOW * statwin)
{
   wmove(statwin,0,0);
   wattron(statwin, A_REVERSE);
   for(int i = 0; i <= maxx; i++)
   {
      wprintw(statwin, " ");
      wmove(statwin,0,i);
   }
   wattroff(statwin, A_REVERSE);
}
