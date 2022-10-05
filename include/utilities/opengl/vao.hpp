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
#ifndef VAO_H
#define VAO_H

#include <utility>
#include <cmath>
#include "sb-engine.hpp"

using namespace std;

template <class Data>
class VAO
{
    private:
        //The name that OpenGL uses to identify this VAO
        GLuint index;

        //The target type for this VAO
        GLenum target;
        /*
        ----BUFFER BINDING TARGET-------------------PURPOSE---------------------------
        ------------------------------------------------------------------------------
            GL_ARRAY_BUFFER                         Vertex attributes
            GL_ATOMIC_COUNTER_BUFFER                Atomic counter storage
            GL_COPY_READ_BUFFER                     Buffer copy source
            GL_COPY_WRITE_BUFFER                    Buffer copy destination
            GL_DISPATCH_INDIRECT_BUFFER             Indirect compute dispatch commands
            GL_DRAW_INDIRECT_BUFFER                 Indirect command arguments
            GL_ELEMENT_ARRAY_BUFFER                 Vertex array indices
            GL_PIXEL_PACK_BUFFER                    Pixel read target
            GL_PIXEL_UNPACK_BUFFER                  Texture data source
            GL_QUERY_BUFFER                         Query result buffer
            GL_SHADER_STORAGE_BUFFER                Read-write storage for shaders
            GL_TEXTURE_BUFFER                       Texture data buffer
            GL_TRANSFORM_FEEDBACK_BUFFER            Transform feedback buffer
            GL_UNIFORM_BUFFER                       Uniform block storage
        */

        //The expected usage pattern of the data store.
        GLenum usage;
        /*
        ----USAGE-----------------------------------PURPOSE----------------------------
        -------------------------------------------------------------------------------
            GL_STREAM_DRAW                          Stream: Modified once, used once.   Draw: Modified by app, used for draw & image commands.
            GL_STREAM_READ                          Stream: Modified once, used once.   Read: Modified by app & OpenGL.
            GL_STREAM_COPY                          Stream: Modified once, used once.   Copy: Modified by OpenGL, used by OpenGL.

            GL_STATIC_DRAW                          Static: Modified once, used many.   Draw: Modified by app, used for draw & image commands.
            GL_STATIC_READ                          Static: Modified once, used many.   Read: Modified by app & OpenGL.
            GL_STATIC_COPY                          Static: Modified once, used many.   Copy: Modified by OpenGL, used by OpenGL.

            GL_DYNAMIC_DRAW                         Dynamic: Modified many, used many.  Draw: Modified by app, used for draw & image commands.
            GL_DYNAMIC_READ                         Dynamic: Modified many, used many.  Read: Modified by app & OpenGL.
            GL_DYNAMIC_COPY                         Dynamic: Modified many, used many.  Copy: Modified by OpenGL, used by OpenGL.
        */

        Data* mappedData;
        bool mapped;

        const Event<int, int> noEvent = Event<int, int>(
            lF(int a)
            {
                return a;
            }
        );

        Event<int, int>* current;

        void copyInto(vector<Data> toCopy)
        {
            (*mappedData) = (*(toCopy.data()));
        }

    public:
        //Constructors
        //----------------------------------

        //Create a new VAO of GL_ARRAY_BUFFER, GL_STATIC_DRAW type.
        VAO()
        {
            this->current = &noEvent;

            //Get a name that is not in use
            glGenVertexArrays(1, &index);

            //Bind a VAO to the name
            glBindVertexArray(index);

            //Assign a default target and usage
            target = GL_ARRAY_BUFFER;
            usage = GL_STATIC_DRAW;
        }

        

        //Create a new VAO of given target and type.
        VAO(GLenum target, GLenum usage) : VAO()
        {
            this->target=target;
            this->usage=usage;
        }

        

        //Unbind and delete the given VAO. Unmap its data and delete it from this VAO object.
        ~VAO()
        {
            if(mapped)
            {
                delete[] mappedData;
            }

            glBindVertexArray(0);
            glDeleteVertexArrays(1, &index);
        }

        

        //Accessors
        //----------------------------------

        //Return the VAO's index (OpenGL name).
        GLuint getIndex() 
        { 
            return index;
        }

        //Return a pointer to the mapped data.
        Data* getMap() 
        {
            return mappedData;
        }

        //Return if this is mapped.
        bool isMapped()
        {
            return mapped;
        }

 

        //Mutators
        //----------------------------------

        //Map the data to the given range.
        void map(Range toMap)
        {
            if(!mapped)
            {
                mappedData = glMapBufferRange(
                    target,
                    (toMap.getMin()*sizeof(Data)),    
                    (toMap.getMax()*sizeof(Data))+1,
                    GL_MAP_UNSYNCHRONIZED_BIT
                );
                mapped=!mapped;
            }
        }

        void test()
        {
            Event<int,int>* tempEvent = new Event<int, int>(F(int a){return (a+1);});
            *(this->current) += *tempEvent;
        }

        //Map all of the data.
        void map()
        {
            if(!mapped)
            {
                mappedData = glMapBuffer(target, GL_READ_WRITE);
                mapped=!mapped;
            }
        }   

        //Unmap the data from the given range.
        void unmap(Range toUnmap)
        {
            if(mapped)
            {
                glFlushMappedBufferRange(
                    target,
                    (mappedData+(toUnmap.getMin()*sizeof(Data))),
                    (mappedData+(toUnmap.getMax()*sizeof(Data))+1)
                );
                mapped=!mapped;
            }
        }

        //Unmap all of the data.
        void unmap()
        {
            if(mapped)
            {
                glUnmapBuffer(target);
                mapped=!mapped;
            }
        }

        //Replace all of the data in the VAO with new data.
        void update(vector<Data> newData)
        {
            Event<int, int> toQueue(
                [this, &newData](int a)
                {
                    //Ask OpenGL to move all of the VAO into RAM instead of VRAM.
                    this->map();

                    //Copy the provided data on top of OpenGL's data.
                    this->copyInto(newData);

                    //Ask OpenGL to move the data back into VRAM.
                    this->unmap();

                    return a;
                }
            );

            this->push(
                &toQueue
            );
            //this->queuedEvents.emplace(&toQueue);
        }

        //Replace a range of the data in the VAO with new data.
        void update(Range toUpdate, vector<Data> newData)
        {
            Event<int, int> toQueue(
                [this, &toUpdate, &newData](int a)
                {
                    //Ask OpenGL to move a range of it's VAO into RAM instead of VRAM.
                    map(toUpdate);

                    //Copy the provided data on top of OpenGL's data.
                    copyInto(newData);

                    //Ask OpenGL to move the data back into VRAM.
                    unmap(toUpdate);

                    return a;
                }
            );

            this->push(
                &toQueue
            );
            //this->queuedEvents.emplace(&toQueue);
        }

};

#endif