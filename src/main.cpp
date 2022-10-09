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
        cout<<"-";
        i--;
    }
    cout<<endl;

    
    //This code only gets included into our program if we run "make" or "make prod" during compilation.
    #ifdef CONFIG_PROD
        cout<<"Launching engine...\n";
        name = possibleName;
    #endif

    //This code only gets included into our program if we run "make test" during compilation.
    #ifdef CONFIG_TEST
        //If we ran the catch session, the results variable may change, indicating a failed test.
        //Do not launch the game if the tests fail.

        cout<<"Running tests...\n";
        results = Catch::Session().run(argc, argv);
        if(results!=0)
        {
            cout<<"Tests failed, engine aborting.\n";
            throw new exception();
        }

        cout<<"Tests succeeded, engine launching.\n";
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

    
    rapidxml::xml_document<> document;
    rapidxml::xml_node<>* root;

    ifstream testData("./config/test_xml.xml");

    vector<char> buffer((istreambuf_iterator<char>(testData)),istreambuf_iterator<char>());
    buffer.push_back('\0');

    document.parse<0>(&buffer[0]);
    root = document.first_node("QuotesList");
    
    auto Fishing = root->first_node("Fishing");
    auto Real = root->first_node("Real");

    cout<<"Fishing Quotes: "<<endl;
    for(int i = 0; i<50; i++) 
    {
        cout<<"-";
    }
    cout<<endl;
    
    //For some node "quote" as a child of Fishing
    //  Where quote exists and is not a nullptr, do some thing
    //  And move to it's sibling
    for(rapidxml::xml_node<>* quote = Fishing->first_node("Quote"); quote; quote=quote->next_sibling())
    {
        cout<<quote->value()<<endl;
    }

    cout<<"\n\nReal Quotes: "<<endl;
    for(int i = 0; i<50; i++) 
    {
        cout<<"-";
    }
    cout<<endl;

    //For some node "quote" as a child of Real
    //  Where quote exists and is not a nullptr, do some thing
    //  And move to it's sibling.
    for(rapidxml::xml_node<>* quote = Real->first_node("Quote"); quote; quote=quote->next_sibling())
    {
        cout<<quote->value()<<endl;
    }





    

    return 0;
}
