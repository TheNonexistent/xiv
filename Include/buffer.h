#ifndef BUFFER_H
#define BUFFER_H

#include <string>
#include <vector>

using namespace std;

extern vector<string> lines;

void insertline(string, int);
void appendline(string);
void removeline(int);

void printbuffer(int,int,int);

string removetabs(string);



#endif//BUFFER_H
