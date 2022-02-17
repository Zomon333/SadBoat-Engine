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
#ifndef IDMAN_H
#define IDMAN_H

#include <random>
#include <unordered_map>
#include <ctime>
#include <iostream>

using namespace std;

enum IDMAN_RESIZE_STATE
{
    DEFAULT=0,
    CUSTOM=1,
    NONE=2
};

class IDMan {
    private:
        int seed;
        int length;
        IDMAN_RESIZE_STATE resize;

       typedef int (*callback_function)(void);
       callback_function resizeCallback;

        std::unordered_map<int, bool> isUsed;

    public:
        //Constructors
        //----------------------------------
        IDMan()
        {
            IDMan(std::time(nullptr), 5, DEFAULT);
        }

        IDMan(int seed=std::time(nullptr), int length=5, IDMAN_RESIZE_STATE resize=DEFAULT)
        {
            setSeed(seed);
            setLength(length);
            this->resize = resize;
        }

        //Callback manager
        //----------------------------------

        void setCallback( callback_function rParam)
        {
            resize=CUSTOM;
            this->resizeCallback = rParam;
        }

        auto getCallback()
        {
            return resizeCallback;
        }

        //Usage checking
        //----------------------------------
        bool isUsable(int id)
        {
            return !isUsed[id];
        }

        //Usage mutators
        //----------------------------------
        void setIDState(int id, bool state)
        {
            isUsed[id] = state;
        }

        //ID Generation functions
        //----------------------------------
        int genDigit()
        {
            return rand() % 10;
        }

        int generateID()
        {
            int toReturn = 0;
            
            int i = 0;
            int resolution = pow(10, length);

            do
            {
                toReturn=0;
            
                for(int i=0; i<length; i++)
                {
                    toReturn *= 10;             //If a value exists, shift it left
                    toReturn += genDigit();     //Then append the current one
                }

                i++;

            } while (isUsed[toReturn] && i<resolution);

            if(i>resolution)
            {
                if(resize==DEFAULT)
                {
                    cout<<"No ID within range, resizing and trying again."<<endl;
                    setLength(length + 1);
                    return generateID();
                }
                else
                {
                    if(resize==CUSTOM)
                    {
                        resizeCallback();
                    }
                    else
                    {
                        cout<<"Exception thrown: ID Overflow without resize callback. Terminating engine.\n";
                        throw new exception("Exception thrown: ID Overflow without resize callback. Terminating engine.\n");
                    }
                }
            }

            isUsed[toReturn] = true;

            return toReturn;
        }

        //Length mutators
        //----------------------------------
        void setLength(int length)
        {
            this->length = length;
        }

        int getLength()
        {
            return length;
        }

        //Seeding functions
        //----------------------------------
        void reseed()
        {
            seed = std::time(nullptr);
            srand(seed);
        }

        void setSeed(int seed)
        {
            this->seed = seed;
            srand(seed);
        }

};

#endif