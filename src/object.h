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
#ifndef OBJ_H
#define OBJ_H

//#include <idman.h>
#include <unordered_map>
#include "sprite.h"
#include "hitbox.h"

using namespace std;

class Object {
    private:
        float x;
        float y;

        float dX;
        float dY;

        //sprite objectSprite;
        //Needs hitbox array
        //

    public:

        int id;

        static IDMan objectIDManager;
        static Registry<int, Object*> objectsRegistry;
        //static std::unordered_map<int, Object*> objectsRegistry;

        Object()
        {
            id = objectIDManager.generateID();
            objectsRegistry.setItem(id, this);

            x=0;
            y=0;
            dX=0;
            dY=0;
        }

        Object(float x, float y, float dX=0, float dY=0)
        {
            Object();
            this->x = x;
            this->y = y;
            this->dX = dX;
            this->dY = dY;
        }

        ~Object()
        {
            objectIDManager.setIDState(id, false);
            objectsRegistry.setItem(id, nullptr);
        }


};

#endif