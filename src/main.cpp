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
    
    
    __asm__("mov %%ecx, %0\n\t":"=r" (results[2]));

    
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

    int i=0;
    int j=0;
    
    //  Feature Flags ECX & EDX Breakout
    //
    //  ECX FEATURE FLAG BREAKOUT
    //--------------------------------------------------
    updatable->SSE3 = parsed[i][j]; j++;                //  0   sse3
    updatable->PCLMULQDQ = parsed[i][j]; j++;           //  1   pclmulqdq
    updatable->DTES64 = parsed[i][j]; j++;              //  2   dtes64
    updatable->MONITOR = parsed[i][j]; j++;             //  3   monitor
    updatable->DS_CPI = parsed[i][j]; j++;              //  4   ds-cpi
    updatable->VMX = parsed[i][j]; j++;                 //  5   vmx
    updatable->SMX = parsed[i][j]; j++;                 //  6   smx
    updatable->EST = parsed[i][j]; j++;                 //  7   est
    updatable->TM2 = parsed[i][j]; j++;                 //  8   tm2
    updatable->SSSE3 = parsed[i][j]; j++;               //  9   ssse3
    updatable->CNXT_ID = parsed[i][j]; j++;             //  10  cnxt-id
    updatable->SDBG = parsed[i][j]; j++;                //  11  sdbg
    updatable->FMA = parsed[i][j]; j++;                 //  12  fma
    updatable->CX16 = parsed[i][j]; j++;                //  13  cx16
    updatable->XTPR = parsed[i][j]; j++;                //  14  xtpr
    updatable->PDCM = parsed[i][j]; j++;                //  15  pdcm
    updatable->RESERVED1 = parsed[i][j]; j++;           //  16  Reserved
    updatable->PCID = parsed[i][j]; j++;                //  17  pcid
    updatable->DCA = parsed[i][j]; j++;                 //  18  dca
    updatable->SSE4_1 = parsed[i][j]; j++;              //  19  sse4.1
    updatable->SSE4_2 = parsed[i][j]; j++;              //  20  sse4.2
    updatable->X2APIC = parsed[i][j]; j++;              //  21  x2apic
    updatable->MOVBE = parsed[i][j]; j++;               //  22  movbe
    updatable->POPCNT = parsed[i][j]; j++;              //  23  popcnt
    updatable->TSC_DEADLINE = parsed[i][j]; j++;        //  24  tsc-deadline
    updatable->AES = parsed[i][j]; j++;                 //  25  aes
    updatable->XSAVE = parsed[i][j]; j++;               //  26  xsave
    updatable->OSXSAVE = parsed[i][j]; j++;             //  27  osxsave
    updatable->AVX = parsed[i][j]; j++;                 //  28  avx
    updatable->F16C = parsed[i][j]; j++;                //  29  f16c
    updatable->RDRND = parsed[i][j]; j++;               //  30  rdrnd
    updatable->HYPERVISOR = parsed[i][j]; j=0; i++;     //  31  hypervisor
    //
    //  EDX FEATURE FLAG BREAKOUT
    //--------------------------------------------------
    updatable->FPU = parsed[i][j]; j++;                 //  0   fpu
    updatable->VME = parsed[i][j]; j++;                 //  1   vme
    updatable->DE = parsed[i][j]; j++;                  //  2   de
    updatable->PSE = parsed[i][j]; j++;                 //  3   pse
    updatable->TSC = parsed[i][j]; j++;                 //  4   tsc
    updatable->MSR = parsed[i][j]; j++;                 //  5   msr
    updatable->PAE = parsed[i][j]; j++;                 //  6   pae
    updatable->MCE = parsed[i][j]; j++;                 //  7   mce
    updatable->CX6 = parsed[i][j]; j++;                 //  8   cx8
    updatable->APIC = parsed[i][j]; j++;                //  9   apic
    updatable->RESERVED2 = parsed[i][j]; j++;           //  10  RESERVED
    updatable->SEP = parsed[i][j]; j++;                 //  11  sep
    updatable->MTRR = parsed[i][j]; j++;                //  12  mtrr
    updatable->PGE = parsed[i][j]; j++;                 //  13  pge
    updatable->MCA = parsed[i][j]; j++;                 //  14  mca
    updatable->CMOV = parsed[i][j]; j++;                //  15  cmov
    updatable->PAT = parsed[i][j]; j++;                 //  16  pat
    updatable->PSE_36 = parsed[i][j]; j++;              //  17  pse-36
    updatable->PSN = parsed[i][j]; j++;                 //  18  psn
    updatable->CLFSH = parsed[i][j]; j++;               //  19  clfsh
    updatable->RESERVED3 = parsed[i][j]; j++;           //  20  RESERVED
    updatable->DS = parsed[i][j]; j++;                  //  21  ds
    updatable->ACPI = parsed[i][j]; j++;                //  22  acpi   
    updatable->MMX = parsed[i][j]; j++;                 //  23  mmx
    updatable->FXSR = parsed[i][j]; j++;                //  24  fxsr
    updatable->SSE = parsed[i][j]; j++;                 //  25  sse
    updatable->SSE2 = parsed[i][j]; j++;                //  26  sse2
    updatable->SS = parsed[i][j]; j++;                  //  27  ss
    updatable->HTT = parsed[i][j]; j++;                 //  28  htt
    updatable->TM = parsed[i][j]; j++;                  //  29  tm
    updatable->IA64 = parsed[i][j]; j++;                //  30  ia64
    updatable->PBE = parsed[i][j]; j++;                 //  31  pbe

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
