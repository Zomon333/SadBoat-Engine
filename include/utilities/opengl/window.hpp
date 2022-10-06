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

#include "sb-engine.hpp"

namespace SBE
{
    using namespace std;

    Event<int,int> displayCallback(F(int a){return a;});
    void display()
    {
        int results = displayCallback(0);
        Event<int,int>* tempEvent = new Event<int,int>(F(int a){return a;});
        displayCallback = *tempEvent;

    }


    //Window: OpenGL Wrapper
    //Class for instancing a game window. 
    //Holds necessary IDs and information, aswell as functions to open the window.
    //Provides no graphics processing; only windowing ability.
    //Provide your own graphics with the provided Event pointers.
    class Window
    {
        private:
            //OpenGL Window name. To understand names better, see shader.hpp
            GLuint windowID = 0;

            //Args to parse
            int argc = 0;

            //Literal window title, not OpenGL name.
            char* windowName = (char*)"";

            //Width
            int x = 0;
            
            //Height
            int y = 0;

            //Pointer to event to handle user-specific display setup
            //Generally used for Shader Compilation and Linking.
            Event<int, int>* SetupHandler;

            //Standard promise indicating when the glut loop is done
            std::promise<bool> closer;

            //Standard future holding the value of the closer promise
            std::future<bool> isFinished;

        public:
            //Constructor
            //----------------------------------
            Window(int x, int y, int* argc, char* name, Event<int, int> &SetupHandler)
            {
                this->x = x;
                this->y = y;
                this->argc = *argc;
                this->windowName = name;

                this->SetupHandler = &SetupHandler;
            }


            //Window management functions
            //----------------------------------

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

                        glewExperimental = GL_TRUE;

                        GLenum glewInitResults = glewInit();

                        SetupHandler->operator()(0);

                        glutDisplayFunc(display);

                        this->closer.set_value_at_thread_exit(true);

                        glutMainLoop();
                        return 0;
                    }
                );

                openWindow.launch(0);
                return &isFinished;
            }

            //Mutators
            //----------------------------------

            //Sets X, doesn't resize window
            void setX(int nX) {x=nX;}

            //Sets Y, doesn't resize window
            void setY(int nY) {y=nY;}

            //Accessors
            //----------------------------------

            //Gets window width
            int getX(){return x;}

            //Gets window height
            int getY(){return y;}

            //Gets window ID (OpenGL name)
            GLuint getWindowID(){return windowID;}

    };

};
#endif