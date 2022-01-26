#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <fstream>
#include <unordered_map>
#include <deque>
#include <string>

#include "globals.h"

#include "cfgutility.h"

#include "pixel.h"
#include "sprite.h"
#include "frame.h"
#include "framebuffer.h"

using namespace std;

int main()
{
    cout<<"Hello world!";

    // Changing configPtr to your own function will override the default config loading utility. Use with caution!
    unordered_map<string, pair<string, string>> (*configPtr)(string fileName);
    configPtr = &readCFG;


    unordered_map<string, pair<string, string>> config = configPtr("config.cfg");

    //Initialize the display size
    const int X_RES = 1920;
    const int Y_RES = 1080;
    const int RESCOUNT = X_RES * Y_RES;

    //Initialize the framebuffer
    framebuffer* screen;
    screen = new framebuffer(X_RES, Y_RES);





    cout<<"Goodbye world!";
    return 0;
}