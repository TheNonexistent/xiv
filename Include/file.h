#include <string>

using namespace std;
#ifndef FILE_H
#define FILE_H


extern string ofilename;
extern string ofilepath;

//int writefile(string filepath);

void opennewfile(string filename);
void openfile(string filename);

void clearstatus(int maxx, WINDOW * statwin);

#endif //FILE_H
