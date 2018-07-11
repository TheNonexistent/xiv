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


int printbuffer(int starty,int startx,int minY,int maxY, int minX, int maxX)
{
   for(int i = 0; i<LINES-2; i++)
    {
        if(i + minY+starty >= lines.size())
        {
            move(i + minY, minX);
            clrtoeol();//The clrtoeol() and wclrtoeol() functions erase the current line from the cursor to the end of the line, inclusive, in the current or specified window. These functions do not update the cursor.
        }
        else
        {
              if(startx < lines[i+starty].length() + 1)
              {
                 //mvprintw(i + minY, minX, lines[i+starty].c_str());
                 mvaddstr(i + minY, minX, lines[i+starty].substr(startx,maxX).c_str());
              }
              else
              {
                 move(i + minY, minX);
                 delch();
                 clrtoeol();
              }
        }
        clrtoeol();
    }
  for(int i = lines.size() - starty ; i < maxY - 1;i++)
  {
     if(!has_colors())
     {
        mvprintw(i, minX, "~");
     }
     else
     {
        start_color();
        use_default_colors();
        init_pair(1, COLOR_CYAN, -1);
        attron(COLOR_PAIR(1));
        mvprintw(i, minX, "~");
        attroff(COLOR_PAIR(1));
     }
  }



  int linecount = 0;
  for(int i = minY; i<LINES-1; i++)
   {
       if(!(i >= lines.size()))
       {
             linecount++;
       }
       clrtoeol();
   }
  return linecount;
}
