#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <fstream>
#include <unordered_map>
#include <deque>
#include <string>
//


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
    cout<<"configPtr assigned: 0x"<<configPtr<<"\n\n";

    cout<<"Loading config.cfg using 0x"<<configPtr<<"\n";
    unordered_map<string, pair<string, pair<string, string>>> config = configPtr("config.cfg");

    cout<<"\nCalculating X_RES. \n";
    int X_RES = stoi(config["X_RES"].second.second);

    cout<<"Calculating Y_RES. \n";
    int Y_RES = stoi(config["Y_RES"].second.second);

    cout<<"Calculating RESCOUNT\n";
    int RESCOUNT = X_RES * Y_RES;

    //This loads the registry config into the registriesConfig Registry. Changing this value in config.cfg will change what registry file to load!
    cout<<"\n\nLoading registries.cfg using 0x"<<configPtr<<"\n";
    unordered_map<string, pair<string, pair<string, string>>> registriesRegistry = configPtr(config["REGISTRY_FILE"].second.second);
    cout<<"\n";


    unordered_map<string, unordered_map<string, pair<string, pair<string, string>>>> allRegistries;

    //This loads the phyiscs config into the physicsRegistry Registry. Changing this value in registries.cfg will change what physics properties to load!
    for(unordered_map<string, pair<string, pair<string, string>>>::iterator i = registriesRegistry.begin(); i!=registriesRegistry.end(); i++)
    {
        string registryFileName = (*i).second.second.second;
        string registryName = (*i).first;
        
        cout<<"Loading "<<registryFileName<<" as "<<registryName<<" into registriesRegistry using 0x"<<configPtr<<"\n";
        allRegistries[registryName] = configPtr(registryFileName);
    }

    




    cout<<"Terminating engine";
    return 0;
}