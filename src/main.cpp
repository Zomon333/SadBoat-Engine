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

#include <immintrin.h>

#include <cpuid.h>
#include <stdio.h>

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

struct HardwareAccelerationSettings
{
    string CPUName;

    bool MMX;
    bool SSE;
    
    bool SSE2;
    bool SSE3;
    bool SSSE3;
    bool SSE4_1;
    bool SSE4_2;

    bool AVX;
    bool AVX512BW;
    bool AVX512VL;
    bool AVX512VBMI;
    bool AVX512_4FMAPS;
    bool AVX512_4VNNIW;
    bool AVX512_VPOPCNTDQ;
    bool AVX512_BITALG;
    bool AVX512_VBMI2;
};

bool getHardwareAccelerationSettings(HardwareAccelerationSettings* updatable)
{
    // Get CPU Name
    int vendor[3];
    
    __asm__("mov $0, %eax\n\t");
    __asm__("cpuid\n\t");

    __asm__("mov %%ebx, %0\n\t":"=r" (vendor[0]));
    __asm__("mov %%edx, %0\n\t":"=r" (vendor[1]));
    __asm__("mov %%ecx, %0\n\t":"=r" (vendor[2]));

    // Get CPU Func Supported
    int func=0;
    __asm__("mov %%eax, %0\n\t":"=r" (func));

    updatable->CPUName=((char*)vendor); 
    updatable->CPUName.resize(12);


    cout<<updatable->CPUName<<endl;
    cout<<func<<endl;

    int results[4];
    // Model & Family
    __asm__("mov $1, %eax\n\t");
    __asm__("cpuid\n\t");
    
    /*
    EAX BREAKOUT
    ------------------------------
    [31:28] Reserved
    [27:20] Extended Family ID
    [19:16] Extended Model ID
    [15:14] Reserved
    [13:12] Processor Type
    [11:8]  Family ID
    [7:4]   Model
    [3:0]   SteppingID
    */
    __asm__("mov %%eax, %0\n\t":"=r" (results[0]));
    
    // Additional Features    
    /*
    EBX BREAKOUT
    ------------------------------
    [24:31] Brand Index
    [16:23] CLFLUSH Line Size
    [8:15]  Maximum # of Addressable IDs for logical processors
    [7:0]   Logical APIC ID
    */
    __asm__("mov %%ebx, %0\n\t":"=r" (results[1]));
    
    // Feature Flags
    /*
    ECX BREAKOUT
    ------------------------------
    0   sse3
    1   pclmulqdq
    2   dtes64
    3   monitor
    4   ds-cpi
    5   vmx
    6   smx
    7   est
    8   tm2
    9   ssse3
    10  cnxt-id
    11  sdbg
    12  fma
    13  cx16
    14  xtpr
    15  pdcm
    16  Reserved
    17  pcid
    18  dca
    19  sse4.1
    20  sse4.2
    21  x2apic
    22  movbe
    23  popcnt
    24  tsc-deadline
    25  aes
    26  xsave
    27  osxsave
    28  avx
    29  f16c
    30  rdrnd
    31  hypervisor
    */
    __asm__("mov %%ecx, %0\n\t":"=r" (results[2]));
    
    // Feature Flags
    /*
    EDX BREAKOUT
    ------------------------------
    0   fpu
    1   vme
    2   de
    3   pse
    4   tsc
    5   msr
    6   pae
    7   mce
    8   cx8
    9   apic
    10  RESERVED
    11  sep
    12  mtrr
    13  pge
    14  mca
    15  cmov
    16  pat
    17  pse-36
    18  psn
    19  clfsh
    20  RESERVED
    21  ds
    22  acpi   
    23  mmx
    24  fxsr
    25  sse
    26  sse2
    27  ss
    28  htt
    29  tm
    30  ia64
    31  pbe
    */
    __asm__("mov %%edx, %0\n\t":"=r" (results[3]));

    bool parsed[2][32];

    unsigned int vals[32] = {
        (0x0001<<0),
        (0x0001<<1),
        (0x0001<<2),
        (0x0001<<3),
        (0x0001<<4),
        (0x0001<<5),
        (0x0001<<6),
        (0x0001<<7),
        (0x0001<<8),
        (0x0001<<9),
        (0x0001<<10),
        (0x0001<<11),
        (0x0001<<12),
        (0x0001<<13),
        (0x0001<<14),
        (0x0001<<15),
        (0x0001<<16),
        (0x0001<<17),
        (0x0001<<18),
        (0x0001<<19),
        (0x0001<<20),
        (0x0001<<21),
        (0x0001<<22),
        (0x0001<<23),
        (0x0001<<24),
        (0x0001<<25),
        (0x0001<<26),
        (0x0001<<27),
        (0x0001<<28),
        (0x0001<<29),
        (0x0001<<30),
        ((unsigned int)(0x0001<<31)),
    };

    for(int i=0; i<32; i++)
    {
        //cout<<(vals[i] & results[2])<<endl;
        parsed[0][i]= ((vals[i]&results[2]) > 0);
        parsed[1][i]= ((vals[i]&results[3]) > 0);
    }

    for(int i=0; i<2; i++)
    {
        for(int j=0; j<32; j++)
        {
            cout<<parsed[i][j];
        }
        cout<<endl;
    }

    return true;
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

    auto devices = vulkanDevices.getDevices();
    
    HardwareAccelerationSettings deviceSettings;
    getHardwareAccelerationSettings(&deviceSettings);


    return 0;
}
