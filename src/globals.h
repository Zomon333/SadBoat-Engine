#ifndef GLOBALS_H
#define GLOBALS_H

#include "pixel.h"

using namespace std;

unordered_map<string, vector<vector<pixel>>> textures;
unordered_map<string, unsigned_int> cachedColors;
unordered_map<string, bool> colorExists;

#endif