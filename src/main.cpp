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
#include "../include/sb-engine.hpp"


using namespace std;


//----------------------------------
//  main(int argc, char* argv[]) function:
//
//      Entry point for program. Manages testing runtimes, and launches other session managers.
//      Return point for all session managers, processes engine shutdown.
//
//----------------------------------
int main(int argc, char* argv[])
{
    int results = 0;
    char* name;

    #ifdef CONFIG_PROD
        cout<<"Launching engine...\n";
        name = (char*)"SadBoat Engine";
    #endif

    #ifdef CONFIG_TEST
        //If we ran the catch session, the results variable may change, indicating a failed test.
        //Do not launch the game if the tests fail.

        cout<<"Running tests...\n";
        results = Catch::Session().run(argc, argv);
        if(results!=0)
        {
            cout<<"Tests failed, engine aborting.\n";
            return results;
        }

        cout<<"Tests succeeded, engine launching.\n";
        name = (char*)"SadBoat Engine Test Stage";
    #endif
    


    VAO<double>* testVAO;


    //This event holds the initialization for Shaders
    //It will be called during the Window startup sequence
    Event<int, int> WindowInit(
        [&testVAO](int a)
        {
            //  1) Declare all of your shaders
            //--------------------------------------------------------------------

            Shader vertexShader = Shader(GL_VERTEX_SHADER,      "shaders/vertex.glsl");
            Shader fragmentShader = Shader(GL_FRAGMENT_SHADER,  "shaders/fragment.glsl");

            //  2) Compile your shaders
            //--------------------------------------------------------------------

            vertexShader.make();
            fragmentShader.make();

            //  3) Link shaders into shader program
            //--------------------------------------------------------------------

            testVAO = new VAO<double>();

            return a;
        }
    );

    Window engineWindow(    //Window: Initializes with properties...
                            1920, 1080,                 //Width: 1920, Height: 1080
                            &argc,                      //Parses argc as an argument.
                            name,                       //Sets Window name as name.
                            WindowInit               //ShaderHandler. Event to handle shader compilation.
                        );

    //Open initialized window. open() returns reference to current window status.                    
    auto windowStatus = engineWindow.open();

    
    vector<double> test(5);
    test.resize(15);
    
    for(int i = 0; i<15; i++)
    {
        test[i]=i;
    }

    
    //testVAO->update(test);

    Event<int, int> testA(
        F(int a)
        {
            return (a*10);
        }
    );

    Event<int, int> testB(
        F(int b)
        {
            return (b-5);
        }
    );

    Event<int, int> none = *(testA + testB);
    
    cout<<testA(5)<<"=50"<<endl;
    cout<<testB(10)<<"=5"<<endl;
    cout<<none(3)<<"=25"<<endl;

    testA += &testB;

    cout<<testA(3)<<"=25"<<endl;

    Event<int, int> testC = *(testB * 2);

    cout<<testC(20)<<"=10"<<endl;




    TimedEvent<int, int> ender(
        F(int a)
        {
            return a;
        },
        steady_clock::now() + seconds(2)
    );
    ender.defer(0);
    return ender.getResult();
}
