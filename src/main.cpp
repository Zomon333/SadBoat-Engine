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
    int i = 100;
    while(i>0)
    {
        std::cout<<"-";
        i--;
    }
    std::cout<<endl;

    
    //This code only gets included into our program if we run "make" or "make prod" during compilation.
    #ifdef CONFIG_PROD
        std::cout<<"Launching engine...\n";
        name = possibleName;
    #endif

    //This code only gets included into our program if we run "make test" during compilation.
    #ifdef CONFIG_TEST
        //If we ran the catch session, the results variable may change, indicating a failed test.
        //Do not launch the game if the tests fail.

        std::cout<<"Running tests...\n";
        results = Catch::Session().run(argc, argv);
        if(results!=0)
        {
            std::cout<<"Tests failed, engine aborting.\n";
            throw new exception();
        }

        std::cout<<"Tests succeeded, engine launching.\n";
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
    //  Unit Test Setup
    //----------------------------------

    //gameName: A constant identifier for what our game should be called.
    const string gameName = "SadBoat Engine";

    //name: An identifier for what our game will actually be called. Dependent on test cases and compilation status.
    string name = testSetup(argc, argv, gameName);
    
    Instance vulkanInstance;
    PhysicalDeviceCollection vulkanDevices = PhysicalDeviceCollection(&vulkanInstance);

    //auto devices = vulkanDevices.getDevices();
    
    // Todo: Implement device choice workflow
    // 1) Prefer device listed in config
    // 2) If no config, Intelligently choose best device as backup, save to config
    // 3) If no device to choose, blindly choose 0th device 
    //auto chosenDevice = devices[0];


    // We need to initialize our LogicalDevice, so we're setting up some boilerplate structs to store the info for the constructor.
    // 
    // 
    VkDeviceCreateInfo initInfo;

    // What type of struct are we initializing our logical device with?
    initInfo.sType=VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    // Does it have any extensions?
    initInfo.pNext=nullptr;

    // Or any flags?
    initInfo.flags=0;


    // We need some info on the Queues we're initializing inside of it, too.
    VkDeviceQueueCreateInfo initDevQueue;
    // What type of struct is this?
    initDevQueue.sType=VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    // Any extensions? (no)
    initDevQueue.pNext=nullptr;
    // Any flags? (no)
    initDevQueue.flags=0;

    // Find some queue family in the collection of queue families.
    // Ideally, we'd do more work here to find the *right* queue family but for this boilerplate
    // we're just going to assume the first queue fam is fine.
    QueueFamilyCollection deviceQueues = QueueFamilyCollection(vulkanDevices.getDevices()[0]);

    // Specifies the family of the queues we want to create
    // Set to the index in the array of queue families stored in QueueFamilyCollection
    initDevQueue.queueFamilyIndex=0;

    // Use as many queues as we can, as long as it's not zero
    initDevQueue.queueCount=(deviceQueues.describeQueue(0)->queueCount-1 > 0) ? deviceQueues.describeQueue(0)->queueCount-1 : 1;

    // An optional pointer to an array of floats representing priority of work submitted to each of the queues. These values are normalized.
    // Use this later for prioritizing rendering for high mobility objects and the player model
    // Setting this to nullptr has the device treat every queue the same
    float priorities[initDevQueue.queueCount];
    
    // This for loop creates a piecewise function to weight queue priorities based on how far they are
    // It has an intentional bias so 20% of the queues are 0.5 priorities
    for(int i=0; i<initDevQueue.queueCount;i++)
    {
        float progress = ((float)(i)/(float)(initDevQueue.queueCount));

        Range mids = Range(0.4, 0.6);


        if(mids[progress])
        {   
            priorities[i]= 0.5;
        }   
        else
        {
            priorities[i] = (1.0-(1.25 * progress));
        }

        if(priorities[i]<0.1)
        {
            priorities[i]=0.1;
        }
    }
    initDevQueue.pQueuePriorities=priorities;
    





    // initInfo.queueCreateInfoCount=
    // initInfo.pQueueCreateInfos=




    // initInfo.enabledLayerCount=
    // initInfo.ppEnabledLayerNames=
    // initInfo.enabledExtensionCount=
    // initInfo.ppEnabledExtensionNames=
    // initInfo.pEnabledFeatures=



    LogicalDevice testDevice = LogicalDevice(vulkanDevices.getDevices()[0], &initInfo);

    return 0;
}
