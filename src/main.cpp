/*
Copyright 2024 Dagan Poulin, Justice Guillory
   Licensed under the Apache License, Version 2.0 (the "License")
    {
        
    }
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at
       http://www.apache.org/licenses/LICENSE-2.0
   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include <iostream>

#include "events/event.hpp"
#include "events/timed_event.hpp"
#include "events/recurring_event.hpp"
#include "events/event_pool.hpp"

#include "utilities/defines.hpp"
#include "utilities/configs/config_node.hpp"
#include "utilities/configs/config.hpp"
#include "utilities/configs/config_manager.hpp"
#include "utilities/logging/log_handle.hpp"
#include "utilities/logging/log_manager.hpp"
#include "utilities/logging/startup_logger.hpp"
SBE::LogManager* SBE::logger;
SBE::LogHandle* SBE::log;

#include "resources/id_manager.hpp"
#include "resources/resource_manager.hpp"
#include "resources/resource.hpp"
#include "resources/resource_handle.hpp"

#include "vulkan.hpp"


//A short test driver to determine window name and whether to boot Catch.
std::string testSetup(int argc, char* argv[], std::string possibleName)
{
    //Declare an empty name.
    std::string name = "";

    //Write a long line for visual clarity.
    std::string tmp="";
    int i = 100;
    while(i>0)
    {
        tmp+="-";
        i--;
    }
    // std::cout<<endl;
    SBE::log->info(tmp);
    
    //This code only gets included into our program if we run "make" or "make prod" during compilation.
    #ifndef CONFIG_TEST
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
        int results = Catch::Session().run(argc, argv);
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


int main(int argc, char* argv[])
{
    // Logging setup
    //----------------------------------

    SBE::logger = new SBE::LogManager("./log.txt");
    SBE::log = SBE::logger->allocateHandle(0b11111);

    //  Unit Test Setup
    //----------------------------------

    //gameName: A constant identifier for what our game should be called.
    const std::string gameName = "SadBoat Engine";

    //name: An identifier for what our game will actually be called. Dependent on test cases and compilation status.
    std::string name = testSetup(argc, argv, gameName);
    
    // Config Callback Setup
    // ---
    // Creates manager classes, creates callbacks for configs
    // Loads certain assets and physics materials
    //----------------------------------

    SBE::ResourceManager resources;
    SBE::ConfigManager configs;

    // Create necessary callbacks to handle processing of data once loaded
    SBE::Event<void*, SBE::ConfigNode> resourceCallback(
        [&resources](SBE::ConfigNode n)
        {
            auto datum = resources.createResource(
                n.getContents<std::string>(),
                (n.getAttribs()[0].first=="persistence") ? (n.getAttribs()[0].second=="true") : false,
                (n.getAttribs()[1].first=="preload") ? (n.getAttribs()[1].second=="true") : false
            );

            SBE::log->info("Resource created.");

            return datum;
        }
    );
    configs.assignCallback("Resource", &resourceCallback);

    // Create config callback
    SBE::Event<void*, SBE::ConfigNode> configCallback(
        [&configs](SBE::ConfigNode n)
        {
            configs.loadConfig(
                n.getContents<std::string>(),
                (n.getAttribs()[0].first=="desc") ? n.getAttribs()[0].second : "Empty description."
            );      
            return nullptr;
        }
    );
    configs.assignCallback("Config", &configCallback);

    // Create callback to process materials into MaterialManager
    SBE::Event<void*, SBE::ConfigNode> materialCallback(
        [](SBE::ConfigNode n)
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
    // VulkanDispatchables vulkanEnvironment;
    // vulkanEnvironment.deviceConfig=(configs.getConfig("./assets/config/graphicsOptions.xml"));
    // vulkanEnvironment.setup(&vulkanEnvironment);

    // CommandPoolManager testPool(vulkanEnvironment.vulkanLogicalDevice);
    // CommandPool* cmdPool = testPool.getPools()[0];
    // vector<CommandBuffer*> cmdBuffers = cmdPool->getBuffers();

    return 1;
}