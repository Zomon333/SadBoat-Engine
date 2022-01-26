#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <fstream>
#include <unordered_map>
#include <deque>
#include <string>

//For textures registry
#include "globals.h"

#include "cfgutility.h"

#include "pixel.h"
#include "sprite.h"
#include "frame.h"
#include "framebuffer.h"

using namespace std;

int main()
{
    cout<<"Initializing engine.\n";

    // Changing configPtr to your own function will override the default config loading utility. Use with caution!
    unordered_map<string, pair<string, pair<string, string>>> (*configPtr)(string fileName);
    cout<<"configPtr created.\n";

    configPtr = &readCFG;
    cout<<"configPtr assigned: 0x"<<configPtr<<"\n";

    cout<<"Loading config.cfg using 0x"<<configPtr<<"\n";
    unordered_map<string, pair<string, pair<string, string>>> config = configPtr("config.cfg");
    cout<<"Config loaded.\n";

    cout<<"Calculating X_RES. \n";
    int X_RES = stoi(config["X_RES"].second.second);

    cout<<"Calculating Y_RES. \n";
    int Y_RES = stoi(config["Y_RES"].second.second);

    cout<<"Calculating RESCOUNT\n";
    int RESCOUNT = X_RES * Y_RES;

    cout<<"\n X_RES: "<<X_RES<<"\n Y_RES: "<<Y_RES<<"\n";

    //Initialize the framebuffer
    framebuffer* screen;
    screen = new framebuffer(X_RES, Y_RES);





    cout<<"Goodbye world!";
    return 0;
}