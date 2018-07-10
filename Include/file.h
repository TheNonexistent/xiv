#include <string>

using namespace std;
#ifndef FILE_H
#define FILE_H


extern string ofilename;
extern string ofilepath;

//int writefile(string filepath);

void opennewfile(string filename);
void openfile(string filename);
void savefile(WINDOW * statwin, int maxx);



void clearstatus(int maxx, WINDOW * statwin);



string tos(int);//Thanks Internet! For the Help!(if we don't have c++11 we can use this to convert to string)

#endif //FILE_H
