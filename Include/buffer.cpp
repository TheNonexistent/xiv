#include "buffer.h"
#include <ncurses.h>

vector<string> lines;

string removetabs(string line)
{
   int tab = line.find("\t");
    if(tab != line.npos)// npos = no position, not found
        return removetabs(line.replace(tab, 1, "   "));//Recursive // replace tab with 3 white space
    else
        return line;
}

void insertline(string line, int ln)
{
   line = removetabs(line);
   lines.insert(lines.begin()+ln, line);//insert into vector in position n
}

void appendline(string line)
{
   line = removetabs(line);
   lines.push_back(line);
}

void removeline(int ln)
{
   lines.erase(lines.begin()+ln);
}


void printbuffer(int minY,int maxY, int minX)
{
   for(int i = minY; i<LINES-1; i++)
    {
        if(i >= lines.size())
        {
            move(i, minX);
            clrtoeol();//The clrtoeol() and wclrtoeol() functions erase the current line from the cursor to the end of the line, inclusive, in the current or specified window. These functions do not update the cursor.
        }
        else
        {
              mvprintw(i, minX, lines[i-minY].c_str());
        }
        clrtoeol();
    }
  for(int i = lines.size(); i < maxY - 1;i++)
  {
     mvprintw(i, minX, "~");
  }
}