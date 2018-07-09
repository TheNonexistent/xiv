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
