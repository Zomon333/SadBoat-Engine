/*
Copyright 2022 Dagan Poulin, Justice Guillory
   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at
       http://www.apache.org/licenses/LICENSE-2.0
   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

---------------------------------------------------------------------------------------------------------------------
SadBoat Engine:
    The SadBoat Engine is an open-source, free to use game engine being designed in C++.
    Most of it's functionality is defined in it's header files, making it a mostly header only engine.
    Consequently, you do not need this main.cpp, specifically, to run your game. It is, however, recommended.
    Opting to not use this main.cpp file means you may need to reimplement the code found herein. This is up to your discretion.

Disclaimer:
    We are not responsible for what you do with our code. You can use it as intended, or make your own use of it.
    If you decide to use it within the parameters we've defined, our documentation exists to help you!
    If something is not clear in our documentation, you can email contact@sadboat.com and we'll try and clarify.
    If your usage is outside of the scope of our documentation, we may not be able to provide you any helpful results.
---------------------------------------------------------------------------------------------------------------------
*/

//
//  Engine Defines
//
//  This adds RapidXML and Vulkan to our include path
//  You can disable these if you'd like, but I'd suggest against it.
//
#define INCLUDE_RAPIDXML
#define INCLUDE_VULKAN

//
//  Engine Include
//
//  This file contains all the includes for the engine. 
//  It's all you need to get started.
//
//----------------------------------
#include "sb-engine.hpp"



using namespace std;
using namespace SBE;

//A short test driver to determine window name and whether to boot Catch.
string testSetup(int argc, char* argv[], string possibleName)
{
    //Declare an empty name.
    string name = "";
    int results;

    //Write a long line for visual clarity.
    string tmp="";
    int i = 100;
    while(i>0)
    {
        tmp+="-";
        i--;
    }
    // std::cout<<endl;
    SBE::log->info(tmp);
    
    //This code only gets included into our program if we run "make" or "make prod" during compilation.
    #ifdef CONFIG_PROD
        // std::cout<<"Launching engine...\n";
        SBE::log->info("Launching engine...");
        SBE::log->info(tmp);

        name = possibleName;
    #endif

    //This code only gets included into our program if we run "make test" during compilation.
    #ifdef CONFIG_TEST
        //If we ran the catch session, the results variable may change, indicating a failed test.
        //Do not launch the game if the tests fail.

        SBE::log->debug("Running tests...");

        results = Catch::Session().run(argc, argv);
        if(results!=0)
        {
            SBE::log->critical("Tests failed, engine aborting.");
            throw new exception();
        }

        SBE::log->debug("Tests succeeded, engine launching.");
        name = "Test Stage - ";
        name+=possibleName;
        
    #endif

    return name;
}


//----------------------------------
//  main(int argc, char* argv[]) function:
//
//      Entry point for program. Manages testing runtimes, and launches other session managers.
//      Return point for all session managers, processes engine shutdown.
//
//----------------------------------
int main(int argc, char* argv[])
{
    // Logging setup
    //----------------------------------

    SBE::logger = new LogManager("./log.txt");
    SBE::log = logger->allocateHandle(0b11111);
 

    //  Unit Test Setup
    //----------------------------------

    //gameName: A constant identifier for what our game should be called.
    const string gameName = "SadBoat Engine";

    //name: An identifier for what our game will actually be called. Dependent on test cases and compilation status.
    string name = testSetup(argc, argv, gameName);
    
    // Config Callback Setup
    // ---
    // Creates manager classes, creates callbacks for configs
    // Loads certain assets and physics materials
    //----------------------------------

    ResourceManager resources;
    ConfigManager configs;

    // Create necessary callbacks to handle processing of data once loaded
    Event<void*, ConfigNode> resourceCallback(
        [&resources](ConfigNode n)
        {
            auto datum = resources.createResource(
                n.getContents<string>(),
                (n.getAttribs()[0].first=="persistence") ? (n.getAttribs()[0].second=="true") : false,
                (n.getAttribs()[1].first=="preload") ? (n.getAttribs()[1].second=="true") : false
            );

            SBE::log->info("Resource created.");

            return datum;
        }
    );
    configs.assignCallback("Resource", &resourceCallback);

    // Create config callback
    Event<void*, ConfigNode> configCallback(
        [&configs](ConfigNode n)
        {
            configs.loadConfig(
                n.getContents<string>(),
                (n.getAttribs()[0].first=="desc") ? n.getAttribs()[0].second : "Empty description."
            );      
            return nullptr;
        }
    );
    configs.assignCallback("Config", &configCallback);

    // Create callback to process materials into MaterialManager
    Event<void*, ConfigNode> materialCallback(
        [](ConfigNode n)
        {
            return nullptr;
        }
    );
    configs.assignCallback("Material", &materialCallback);

    // Load initialization file
    configs.loadConfig("./assets/configs.xml", "Initialization Config");
    

    // Vulkan Environment Setup
    //----------------------------------

    //vulkanEnvironment: A struct containing our abstracted Vulkan classes, some data, and some parsing events.
    VulkanDispatchables vulkanEnvironment;
    vulkanEnvironment.deviceConfig=(configs.getConfig("./assets/config/graphicsOptions.xml"));
    vulkanEnvironment.setup(&vulkanEnvironment);

    CommandPoolManager testPool(vulkanEnvironment.vulkanLogicalDevice);

    return 0;
}
