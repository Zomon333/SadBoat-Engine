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

//STD Includes
//----------------------------------
#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <fstream>
#include <unordered_map>
#include <deque>
#include <string>
#include <cmath>
#include <thread>

//Utility Includes
//----------------------------------
#include "idman.h"
#include "globals.h"
#include "cfgutility.h"
#include "registry.h"
#include "event.h"

//OpenGL Includes
//----------------------------------
#include "../includes/glad/glad.h"
#include "../includes/GLFW/glfw3.h"
#include "openglhandlers.h"

//OpenAL Includes
//----------------------------------
#include "../includes/OpenAL/al.h"
#include "../includes/OpenAL/alc.h"
#include "../includes/OpenAL/efx-creative.h"
#include "../includes/OpenAL/EFX-Util.h"
#include "../includes/OpenAL/efx.h"
#include "../includes/OpenAL/xram.h"
#include "../includes/OpenAL/alut.h"

//Physics related includes
//----------------------------------
#include "range.h"
#include "hitbox.h"
#include "object.h"

//Graphics framework includes
//----------------------------------
#include "color.h"
#include "pixel.h"
#include "sprite.h"
#include "frame.h"
#include "framebuffer.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

void joinAvailable()
{
    if(callStack.size()>0)
    {
        while(callStack.top()->joinable())
        {
            callStack.top()->join();
        }
    }
}

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
        configRegistry = new Registry<string, pair<string, pair<string, string>>>("CONFIG","Registry for Engine Configuration Information");
        configRegistry->setThisRegistry(configPtr("config.cfg"));

        //LOADING FOR REGISTRY LISTING FILE
        cout<<"\n\nLoading registries.cfg using 0x"<<configPtr<<"\n";
        registriesRegistry = new Registry<string, pair<string, pair<string, string>>>("REGISTRIES","Registry for Registry Loading Information");
        
        registriesRegistry->setThisRegistry(
            configPtr(
                configRegistry->getItem("REGISTRY_FILE").second.second
            )
        );

        cout<<"\n";


        //INITIALIZING FOR OTHER REGISTRIES
        for( auto i = registriesRegistry->getThisRegistry()->begin(); i!=registriesRegistry->getThisRegistry()->end(); i++)
        {
            string registryFileName = (*i).second.second.second;
            string registryName = (*i).first;
            
            cout<<"Loading "<<registryFileName<<" as "<<registryName<<" into registriesRegistry using 0x"<<configPtr<<"\n";
            Registry<string, pair<string, pair<string, string>>>* tempRegistry = new Registry<string, pair<string, pair<string, string>>>(registryName, registryFileName);
            tempRegistry->setThisRegistry(configPtr(registryFileName));
        }
    }

    
    /*
--------------------------------------------------------------------------------
        AUDIO ENVIRONMENT SETUP
        OPENAL

--------------------------------------------------------------------------------
    */
   bool openALAudioEnvironment = configRegistry->getItem("PLAYBACK").second.second == "OPENAL";
  
   if(openALAudioEnvironment)
   {
        cout<<"Initializing OpenAL Audio Environment.\n";

        //Get Default Device    
        Device = alcOpenDevice(NULL);

        //If Default Device is Open
        if(Device)
        {
            //Make Context Current
            Context = alcCreateContext(Device, NULL);
            alcMakeContextCurrent(Context);
        }

        //Test if EAX2.0 extension is loaded
        auto g_bEAK = alIsExtensionPresent("EAX2.0");
        if(!g_bEAK)
        {
            cout<<"EAX2.0 Extension is not present!\n";
            return -1;
        }
        else
        {
            cout<<"EAX2.0 Extension is present.\n";
        }

        //Clear error state
        auto error = alGetError();

        //Generate buffers
        g_buffers = new ALuint;
        alGenBuffers(1, g_buffers);

        //Test if there's an error with the buffer generation
        if(
            (
                error = alGetError()
            ) != AL_NO_ERROR)
        {
            cout<<"Error loading buffer.\n";
            return -1;
        }



        cout<<"OpenAL Initialization complete.\n";
   }

    
    
    /*
--------------------------------------------------------------------------------
        GRAPHICS ENVIRONMENT SETUP
        OPENGL / VULKAN

--------------------------------------------------------------------------------
    */
   X_RES=1920;
   Y_RES=1080;
   int RESCOUNT = X_RES * Y_RES;
   
    //bool openGLGraphicsEnvironment = config["RENDERER"].second.second == "OPENGL";
    bool openGLGraphicsEnvironment = configRegistry->getItem("RENDERER").second.second == "OPENGL";

    if(openGLGraphicsEnvironment)
    {

        cout<<"\nCalculating X_RES. \n";
        X_RES = stoi(configRegistry->getItem("X_RES").second.second); 

        cout<<"Calculating Y_RES. \n";
        Y_RES = stoi(configRegistry->getItem("Y_RES").second.second);

        cout<<"Calculating RESCOUNT\n";
        RESCOUNT = X_RES * Y_RES;


        const char* defaultVertexShaderSource=
            "#version 330 core\n"
            "layout (location = 0) in vec3 aPos;\n"
            "layout (location = 1) in vec3 aColor;\n"
            "out vec3 pixelColor;\n"
            "void main()\n"
            "{\n"
            "   gl_Position = vec4(aPos, 1.0);\n"
            "   pixelColor = aColor;\n"
            "}\0";

        const char* defaultFragmentShaderSource=
            "#version 330 core\n"
            "out vec4 FragColor;\n"
            "in vec3 pixelColor;\n"
            "void main()\n"
            "{\n"
            "   FragColor = vec4(pixelColor, 1.0f);\n"
            "}\n\0";

        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        #ifdef __APPLE__
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        #endif

        GLFWwindow* window = glfwCreateWindow(X_RES, Y_RES, "SadBoat", NULL, NULL);
        if(window==NULL)
        {
            cout<<"Window creation failed!\n";
            glfwTerminate();
            return -1;
        }
    
        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            cout<<"GLAD init failed!\n";
            return -1;
        }

        //----------------------
        //  Vertex Shader Compilation
        //----------------------

        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &defaultVertexShaderSource, NULL);
        glCompileShader(vertexShader);

        int success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            cout<<"Vertex shader compilation failed: "<<infoLog<<endl;
            return -1;
        }
        cout<<"Default vertex shader compilation was successful.\n";

        //----------------------
        //  Fragment Shader Compilation
        //----------------------

        unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &defaultFragmentShaderSource, NULL);
        glCompileShader(fragmentShader);
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            cout<<"Fragment shader compilation failed: "<<infoLog<<endl;
            return -1;
        }
        cout<<"Default fragment shader compilation was successful.\n";

        //----------------------
        //  Shader Program Creation & Shader Linking
        //----------------------

        unsigned int shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            cout<<"Shader program linking failed: "<<infoLog<<endl;
            return -1;
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);


        /*
--------------------------------------------------------------------------------
        DEFAULT RASTER SYSTEM

--------------------------------------------------------------------------------
    */
        bool defaultRasterSystem = true;
        if(defaultRasterSystem)
        {
            framebuffer screen = framebuffer(X_RES,  Y_RES);
            frame renderable = screen.getFinal();
            frame testFrame = frame(X_RES, Y_RES);

            for(int x = 0; x< (X_RES / 2); x++)
            {
                for(int y = 0; y< (Y_RES/2); y++)
                {
                    testFrame.setcolorClass(x, y, colorClass("FF0000FF"));
                }
            }

            //----------------------
            //  Set up vertex data and buffers
            //----------------------

            // set up vertex data (and buffer(s)) and configure vertex attributes
            // ------------------------------------------------------------------

            struct bufferContainer{
                int ARRAY_QUANTITY;
                float* vertices;

                unsigned int VBO;
                unsigned int VAO;

                void setColor(const int X_RES, const int Y_RES, int x, int y, tuple<float, float, float> rgb) {
                    int offset = 0;
                    offset = y * X_RES;
                    offset += x;
                    offset *= 6;

                    vertices[offset+3]=get<0>(rgb);
                    vertices[offset+4]=get<1>(rgb);
                    vertices[offset+5]=get<2>(rgb);
                }

                void init(const int X_RES, const int Y_RES)
                {
                    ARRAY_QUANTITY = (X_RES*Y_RES*2);
                    vertices = new float[ARRAY_QUANTITY*6];

                    glGenVertexArrays(1, &VAO);
                    glGenBuffers(1, &VBO);

                    glBindVertexArray(VAO);
                    glBindBuffer(GL_ARRAY_BUFFER, VBO);

                    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*6, vertices, GL_STATIC_DRAW);

                    //Positioning attributes
                    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
                    glEnableVertexAttribArray(0);
                }

            };

            bufferContainer currentFrame;
            currentFrame.init(X_RES, Y_RES);

            //see notes

            glUseProgram(shaderProgram);

            while(!glfwWindowShouldClose(window))
            {

                processInput(window);

                glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);


                /*for(int i = 0; i<ARRAY_QUANTITY; i++)
                {
                    glBindVertexArray(VAOs[i]);
                    glDrawArrays(GL_TRIANGLES, 0, 3);
                }*/
                
                glDrawArrays(GL_TRIANGLES, 0, 3);

                // render the triangle
                //glBindVertexArray(VAO);
                //glDrawArrays(GL_TRIANGLES, 0, 3);

                // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
                // -------------------------------------------------------------------------------
                glfwSwapBuffers(window);
                glfwPollEvents();
            }
            
           
            
        }
    }
    
    Context = alcGetCurrentContext();
    Device = alcGetContextsDevice(Context);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(Context);
    alcCloseDevice(Device);

    cout<<"Terminating engine";
    return 0;
}