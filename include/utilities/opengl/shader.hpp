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
        //OpenGL uses a Client/Server architecture; we make requests, and the OpenGL server tries to fulfill them.
        //Due to this, we don't get to manage OpenGL's memory directly. Instead of using pointers, OpenGL builds lookup tables with "names".
        //"Names" are the client-side identifier we use in our application. They usually get mapped to a pointer server-side on the GPU throug a lookup table.
        
        //ShaderID is this shader's "name".
        //ShaderType is essentially, then, the *class* of the Shader.

        GLuint shaderID;
        GLenum shaderType;
        
        //Code is a purely-frontend convention for us to more-easily store the program.

        string code;

    public:
        //Constructors
        //----------------------------------

        //Default constructor: Does nothing.
        Shader();

        //Partial constructor: Sets and allocates type.
        Shader(GLenum shaderType)
        {
            shaderID = glCreateShader(shaderType);
            this->shaderType=shaderType;
        }

        //Indirect file constructor: Sets and allocates type. Opens filename, and sets shader code to contents.
        Shader(GLenum shaderType, string fileName) : Shader(shaderType)
        {
            loadCode(fileName);
        }
        
        //Direct file constructor: Sets and allocates type. Accepts an already open file, and sets shader code to contents.
        Shader(GLenum shaderType, fstream* file) : Shader(shaderType)
        {
            loadCode(file);
        }

        //Deconstructor
        //----------------------------------

        //Default deconstructor: Deletes shader if shaderID is taken.
        ~Shader()
        {
            if(isShader())
            {
                glDeleteShader(shaderID);
            }
        }

        //Shader loading
        //----------------------------------

        //Accepts an open file stream by reference, reads it into this->code, and closes it.
        void loadCode(fstream* codeFile)
        {
            string tempString = "";
            while(codeFile->peek()!=EOF)
            {
                tempString+=codeFile->get();
            }

            codeFile->close();

            this->code=tempString;
        }

        //Accepts a filename, opens the file, reads it into this-> code, and closes it.
        void loadCode(string file)
        {
            fstream codeFile;
            codeFile.open(file, ios::in);

            string tempString = "";
            while(codeFile.peek()!=EOF)
            {
                tempString+=codeFile.get();
            }

            codeFile.close();

            this->code=tempString;
        }

        //Shader compilation
        //----------------------------------

        //Compilation function for shader; returns result.
        GLint make()
        {
            //Get something to catch the result
            GLint result;

            //Cast our data to the format the OpenGL functions want to run with
            const GLchar *source = (const GLchar*)code.c_str();
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

        //Accessors
        //----------------------------------

        //Get ShaderID (AKA Shader "name")
        GLuint getID(){ return this->shaderID; }
        
        //Get Shader Type (Vertex, Fragment, etc.)
        GLenum getType(){ return this->shaderType; }
        
        //Gets Shader Status (Is the ShaderID an allocated shader name?)
        bool isShader() { return glIsShader(shaderID); }
};

#endif