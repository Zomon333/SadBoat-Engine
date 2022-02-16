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
#ifndef HIT_H
#define HIT_H

#include <unordered_map>
#include <algorithm>

#include "idman.h"
#include "event.h"
#include "range.h"


using namespace std;

class Hitbox
{
private:
    range width;
    range height;
    pair<int, int> relativePos;

    int id;

    auto (*collisionCallback)()

public:

    static IDMan hitboxIDManager;
    static std::unordered_map<int, Hitbox*> hitboxRegistry;
    static std::unordered_map<pair<int, int>, Hitbox*> collisionRegistry;

    //Constructors
    //----------------------------------
    Hitbox()
    {
        Hitbox(range(0,1), range(0,1), make_pair<int, int>(0, 0));
    }

    Hitbox(range xRange, range yRange, pair<int, int> relativePos)
    {
        id = hitboxIDManager.generateID();
        hitboxRegistry[id] = this;

        width = xRange;
        height = yRange;
        this->relativePos = relativePos;
    }

    ~Hitbox()
    {
        collisionRegistry[relativePos] = nullptr;
        hitboxRegistry[id] = nullptr;
        hitboxIDManager.setState(id, false);
    }

    //Viable movement confirmation functions
    //----------------------------------
    bool isWithin(pair<int, int> toTest)
    {
        return (width.inRange(toTest.first) && height.inRange(toTest.second));
    }
    
    bool canShift(pair<int, int> change)
    {
        change.first += relativePos.first;
        change.second += relativePos.second;

        if(collisionRegistry[change]==nullptr)
        {
            return true;
        }
        else
        {
            return false;
        }

    }

    //Movement functions
    //----------------------------------
    void goTo(const pair<int, int> location, bool collisionOverride=false)
    {
        if(!validityOverride && collisionRegistry[location]!=nullptr)
        {
            //Throw a new event with high priority as collision callback
        }
        if(validityOverride || collisionRegistry[location]==nullptr)
        {
            collisionRegistry[location] = this;
            collisionRegistry[relativePos] = nullptr;
            relativePos = location;
        }
    }
    void shift(const pair<int, int> change)
    {
        if(canShift(change))
        {
            relativePos.first += change.first;
            relativePos.second += change.second;
            goTo(relativePos);
        }
        else
        {
            //Throw a new event with high priority as collision callback
            return;
        }
    }

};

#endif