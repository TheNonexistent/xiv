#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ncurses.h>
#include <unistd.h>

#include "file.h"
#include "buffer.h"

using namespace std;

string ofilename;
string ofilepath;

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

void opennewfile(string filename)
{
    ofilename = filename;
    ofilepath = filename;

    appendline("");appendline(""); //Preventing Seg Fault
}

void openfile(string filename)
{
   ofilename = filename;
   ofilepath = filename;

   ifstream infile(ofilepath.c_str());
   if(infile.is_open())
   {
     while(!infile.eof())
     {
         string fline;
         getline(infile, fline);
         appendline(fline);
     }
   }
   else
   {
     cerr<< "Error: Can't Open File '" << ofilename << "'\n";
     appendline(" ");
   }
}

void savefile(WINDOW * statwin, int maxx)
{
    ofstream f(ofilename.c_str());
    if(f.is_open())
    {
        for(int i=0; i<lines.size() - 1; i++)
        {
            f << lines[i] << endl;
        }
        wmove(statwin, 0, 0);
        wattron(statwin, A_REVERSE);
        wprintw(statwin,"Saved To File: %s.", ofilename.c_str());
        wattroff(statwin, A_REVERSE);
        wrefresh(statwin);
        usleep(2000000);
        clearstatus(maxx,statwin);
        wmove(statwin,0,0);
        wattron(statwin, A_REVERSE);
        wprintw(statwin, "[Normal Mode]");
        wattroff(statwin, A_REVERSE);
    }
    else
    {
        wmove(statwin, 0, 0);
        wattron(statwin, A_REVERSE);
        wprintw(statwin,"Error: Cannot Open File In Order To Save!");
        wattroff(statwin, A_REVERSE);
        wrefresh(statwin);
        usleep(2000000);
        clearstatus(maxx,statwin);
        wmove(statwin,0,0);
        wattron(statwin, A_REVERSE);
        wprintw(statwin, "[Normal Mode]");
        wattroff(statwin, A_REVERSE);
    }
    f.close();
}

string tos(int i)
{
    stringstream ss;
    ss << i;
    return ss.str();
}
