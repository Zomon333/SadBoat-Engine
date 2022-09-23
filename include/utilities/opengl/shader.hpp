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
#ifndef SHADER_H
#define SHADER_H

class Shader
{
    private:
        GLuint shaderID;
        GLenum shaderType;
        
        const char* code;

    public:
        Shader()
        {
            shaderID = glCreateShader(GL_VERTEX_SHADER);
            this->shaderType = GL_VERTEX_SHADER;
            this->code = 
                "#version 430 core"
                "in vec4 vPosition;"
                "in vec4 vColor;"
                ""
                "out vec4 color;"
                ""
                "void"
                "main()"
                "{"
                "   color = vColor;"
                "}"
            ;
            
        }
        Shader(GLenum shaderType, string code)
        {
            //Allocate a name for our shader sufficient to hold our type of shader
            shaderID = glCreateShader(shaderType);

            this->shaderType=shaderType;
            this->code = code.c_str();
        }

        ~Shader()
        {
            if(isShader())
            {
                glDeleteShader(shaderID);
            }
        }

        int make(bool multiThread=true)
        {
            //Get something to catch the result
            GLint result;

            //Make an event that captures this object and the result by reference
            Event<GLint,int> makeEvent(
                [this, &result](int a=0)
                {
                    //Cast our data to the format the OpenGL functions want to run with
                    const GLchar *source = (const GLchar*)code;
                    //Provide our source code to OpenGL
                    glShaderSource(shaderID, 1, &source, 0);

                    //Try to compile the shader
                    glCompileShader(shaderID);

                    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);

                    if(result!=GL_TRUE)
                    {
                        //There's an issue in shader compilation!
                        //glGetShaderInfoLog(...)
                    }

                    //We don't actually need to return the result; we're just *going to*.
                    return result;
                }
            );
            
            if(multiThread) {           //If multithreaded
                makeEvent.launch(0);    //Launch
                makeEvent.getResult();  //And wait
            } 
            else makeEvent(0);          //Else: Just calc

            return result;              //Because result was captured by reference, we don't even care about the returns. It was updated in real time.
        }

        GLuint getID(){ return this->shaderID; }
        GLenum getType(){ return this->shaderType; }
        bool isShader() { return glIsShader(shaderID); }
};

#endif