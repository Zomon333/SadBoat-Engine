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
#ifndef WINDOW_H
#define WINDOW_H

#include "../../sb-engine.hpp"

using namespace std;


class Window
{
    private:
        GLuint windowID = 0;

        int argc = 0;
        char* windowName = (char*)"";

        int x = 0;
        int y = 0;

        unordered_map<GLuint, Shader*>* shaderMap;
        Event<int, int>* declareShaders;

        std::promise<bool> closer;
        std::future<bool> isFinished;

    public:
        Window(int x, int y, int* argc, char* name, Event<int, int> &ShaderEvent, unordered_map<GLuint, Shader*> &shaderMap)
        {
            this->x = x;
            this->y = y;
            this->argc = *argc;
            this->windowName = name;

            this->shaderMap = &shaderMap;
            this->declareShaders = &ShaderEvent;
        }

        static void display()
        {
            //Do something
        }

        std::future<bool>* open()
        {
            isFinished = closer.get_future();

            Event<int, int> openWindow(
                lF(int a)
                {
                                        
                    glutInit(&argc, &windowName);

                    glutInitDisplayMode(GLUT_RGBA);
                    glutInitWindowSize(x,y);

                    glutInitContextVersion(4,3);
                    glutInitContextProfile(GLUT_CORE_PROFILE);
                    
                    windowID = glutCreateWindow(windowName);
                    GLenum glewInitResults = glewInit();

                    declareShaders->operator()(0);
                    glutDisplayFunc(display);

                    this->closer.set_value_at_thread_exit(true);

                    glutMainLoop();
                    return 0;
                }
            );

            openWindow.launch(0);
            return &isFinished;
        }

        void setX(int nX) {x=nX;}
        void setY(int nY) {y=nY;}

        int getX(){return x;}
        int getY(){return y;}

        GLuint getWindowID(){return windowID;}

        auto getShaderMap(){return shaderMap;}
        auto getStoredShaders()
        {
            vector<Shader*> shaders;
            
            for(auto sPair : (*shaderMap))
            {
                shaders.push_back(sPair.second);
            }

            return shaders;
        }
};



#endif