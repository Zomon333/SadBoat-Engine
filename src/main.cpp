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
*/
#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <fstream>
#include <unordered_map>
#include <deque>
#include <string>
//

#include "../includes/glad/glad.h"
#include "../includes/GLFW/glfw3.h"

#include <cmath>

#include <iostream>
#include <cmath>

#include "globals.h"

#include "cfgutility.h"

#include "color.h"

#include "pixel.h"

#include "sprite.h"
#include "frame.h"
#include "framebuffer.h"

using namespace std;


int main()
{
    cout<<"Initializing engine.\n";

    /*
--------------------------------------------------------------------------------
        CONFIGURATION FILE LOADING

--------------------------------------------------------------------------------
    */
    bool defaultConfigLoading = true;
    if(defaultConfigLoading)
    {
        // Changing configPtr to your own function will override the default config loading utility. Use with caution!
        unordered_map<string, pair<string, pair<string, string>>> (*configPtr)(string fileName);
        cout<<"configPtr created.\n";
        configPtr = &readCFG;
        cout<<"configPtr assigned: 0x"<<configPtr<<"\n\n";

        //LOADING FOR CONFIG.CFG
        cout<<"Loading config.cfg using 0x"<<configPtr<<"\n";
        config = configPtr("config.cfg");

        //LOADING FOR REGISTRY LISTING FILE
        cout<<"\n\nLoading registries.cfg using 0x"<<configPtr<<"\n";
        registriesRegistry = configPtr(config["REGISTRY_FILE"].second.second);
        cout<<"\n";


        //INITIALIZING FOR OTHER REGISTRIES
        for(unordered_map<string, pair<string, pair<string, string>>>::iterator i = registriesRegistry.begin(); i!=registriesRegistry.end(); i++)
        {
            string registryFileName = (*i).second.second.second;
            string registryName = (*i).first;
            
            cout<<"Loading "<<registryFileName<<" as "<<registryName<<" into registriesRegistry using 0x"<<configPtr<<"\n";
            allRegistries[registryName] = configPtr(registryFileName);
        }
    }

    
    
    
    /*
--------------------------------------------------------------------------------
        GRAPHICS ENVIRONMENT SETUP

--------------------------------------------------------------------------------
    */
    bool openGLGraphicsEnvironment = true;
    if(openGLGraphicsEnvironment)
    {

        cout<<"\nCalculating X_RES. \n";
        int X_RES = stoi(config["X_RES"].second.second);

        cout<<"Calculating Y_RES. \n";
        int Y_RES = stoi(config["Y_RES"].second.second);

        cout<<"Calculating RESCOUNT\n";
        int RESCOUNT = X_RES * Y_RES;


        const char* defaultVertexShaderSource=
            "#version 330 core\n"
            "layout (location = 0) in vec3 aPos;\n"
            "void main()\n"
            "{\n"
            "   gl_Position = vec4(aPos, 1.0);\n"
            "}\0";

        const char* defaultFragmentShaderSource=
            "#version 330 core\n"
            "out vec4 FragColor;\n"
            "uniform vec4 ourColor;\n"
            "void main()\n"
            "{\n"
            "   FragColor = ourColor;\n"
            "}\n\0";

        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        //pixel testPixel = pixel(pair<float, float>(0.0f, 0.0f), pair<float, float>((float)(X_RES), (float)(Y_RES)), colorClass());

        framebuffer screen = framebuffer(X_RES,  Y_RES);
        //frame renderable = screen.getFrame(0);
        frame testFrame = frame(1920, 1080);

        for(int x = 0; x<X_RES; x++)
        {
            for(int y = 0; y<Y_RES; y++)
            {
                pixel testPixel = testFrame.getPixel(x, y);

                float testArray[] =
                {
                    testPixel.getBounds(0).first, testPixel.getBounds(0).second,
                    testPixel.getBounds(1).first, testPixel.getBounds(1).second,
                    testPixel.getBounds(2).first, testPixel.getBounds(2).second,
                    testPixel.getBounds(3).first, testPixel.getBounds(3).second,
                };
            }
        }
        

    }



    cout<<"Terminating engine";
    return 0;
}