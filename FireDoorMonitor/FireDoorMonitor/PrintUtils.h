#ifndef _PRINTUTILS
#define _PRINTUTILS
#include <string>
#include <iostream>

using namespace std;

extern const string cutoff_rule;

void logging(const string &logging, bool end);
void warning(const string &warning, bool end);

#endif