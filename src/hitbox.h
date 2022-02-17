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

    void (*collisionCallback)();

public:

    static IDMan hitboxIDManager;
    static inline Registry<int, Hitbox*> *hitboxRegistry = new Registry<int, Hitbox*>("HITBOX","A registry of all available hitboxes");
    static inline Registry<int, Hitbox*> *collisionRegistry = new Registry<int,Hitbox*>("COLLISION","A registry of hitboxes organized by location for collision detection");
    //static std::unordered_map<int, Hitbox*> hitboxRegistry;
    //static std::unordered_map<pair<int, int>, Hitbox*> collisionRegistry;


    //Accessors
    //----------------------------------
    range getWidth() { return this->width; }
    range getHeight() { return this->height; }
    pair<int, int> getPos() { return this->relativePos; }
    int getID() { return this->id; }
    auto getCollisionCallback() { return this->collisionCallback; }

    //Constructors
    //----------------------------------
    Hitbox()
    {
        Hitbox(range(0,1), range(0,1), make_pair<int, int>(0, 0));
    }

    Hitbox(Hitbox &copy)
    {
        this->width = copy.getWidth();
        this->height = copy.getHeight();
        this->relativePos = copy.getPos();

        this->id = copy.getID();
        this->collisionCallback = copy.getCollisionCallback();

    }

    Hitbox(range xRange, range yRange, pair<int, int> relativePos)
    {
        id = hitboxIDManager.generateID();
        hitboxRegistry->setItem(id, this);

        width = xRange;
        height = yRange;
        this->relativePos = relativePos;
    }

    Hitbox(const Hitbox&) = default;

    ~Hitbox()
    {
        collisionRegistry->setItem(pairToInt(relativePos), nullptr);
        hitboxRegistry->setItem(id, nullptr);
        hitboxIDManager.setIDState(id, false);
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

        if(collisionRegistry->getItem(pairToInt(change))==nullptr)
        {
            return true;
        }
        else
        {
            return false;
        }

    }

    int pairToInt(pair<int, int> p)
    {
        return (p.second * X_RES) + p.first;
    }

    //Movement functions
    //----------------------------------
    void goTo(const pair<int, int> location, bool collisionOverride=false)
    {
        if(!collisionOverride && collisionRegistry->getItem(pairToInt(location))!=nullptr)
        {
            //Throw a new event with high priority as collision callback
        }
        if(collisionOverride || collisionRegistry->getItem(pairToInt(location))==nullptr)
        {
            collisionRegistry->setItem(pairToInt(location), this);
            collisionRegistry->setItem(pairToInt(relativePos), nullptr);
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