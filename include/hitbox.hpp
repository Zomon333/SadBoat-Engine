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
#ifndef HB_H
#define HB_H

#include "boundingbox.hpp"
#include "idman.hpp"
#include "function.hpp"

using namespace std;

// For use in Hitbox
class Hitmesh;

class Hitbox : public BoundingBox
{
private:
    pair<int, int> relativePos;
    int depth;

    int id;

    Function<void, Hitbox *> collisionCallback;
    Hitmesh *parentHitmesh;

    virtual void nullable() = 0;

public:
    static IdManager hitboxIDManager;

    // Constructors
    //----------------------------------
    Hitbox()
    {
        BoundingBox();
        relativePos = make_pair<int, int>(0, 0);
        depth = 0;
        id = hitboxIDManager.allocate();
    }
    Hitbox(Hitbox &copy)
    {
        BoundingBox(copy.getWidth(), copy.getHeight());
        this->id = copy.id;
        this->relativePos = copy.relativePos;
        this->depth = copy.depth;
    }

    Hitbox(Range xRange, Range yRange, pair<int, int> relativePos, int depth = 0)
    {
        BoundingBox(xRange, yRange);
        this->relativePos = relativePos;
        id = hitboxIDManager.allocate();
        this->depth = depth;

        Function<void, void> someFunction = Function<void, void>(&nullable);

    }

    ~Hitbox()
    {
        hitboxIDManager.free(id);
    }

    Hitbox(const Hitbox &) = default;

    // Accessors
    //----------------------------------
    pair<int, int> getPos()
    {
        return this->relativePos;
    }

    int getDepth()
    {
        return depth;
    }

    int getID()
    {
        return this->id;
    }

    auto getCollisionCallback()
    {
        return collisionCallback;
    }

    auto getParentHitmesh()
    {
        return this->parentHitmesh;
    }

    // Mutators
    //----------------------------------
    void setCollisionCallback(Function<void, Hitbox *> newCallback)
    {
        collisionCallback = newCallback;
    }

    void setParentHitmesh(Hitmesh *parent)
    {
        this->parentHitmesh = parent;
    }

    // Viable movement confirmation functions
    //----------------------------------

    bool canShift(pair<int, int> change, int depth)
    {
        change.first += relativePos.first;
        change.second += relativePos.second;

        // if collisionRegistry[change.first][change.second][depth]==nullptr return true else return false
        return false;
    }

    // Movement functions
    //----------------------------------
    void goTo(const pair<int, int> location, int depth, bool collisionOverride = false)
    {
        // if(!collisionOverride && collisionRegistry[location.first][location.second][depth]!=nullptr) //Collide!
        // if(collisionOverride || collisionRegistry[location.first][location.second][depth]==nullptr)
        //{
        // collisionRegistry[location.first][location.second][depth]=this;
        // collisionRegistry[relativePos.first][relativePos.second][depth]=nullptr;
        // relativepos=location;
        //}
    }

    void shift(const pair<int, int> change, int depth)
    {
        int newDepth = this->depth + depth;
        if (canShift(change, depth))
        {
            relativePos.first += change.first;
            relativePos.second += change.second;
            goTo(relativePos, newDepth);
        }
        else
        {
            // Throw a new event with high priority as collision callback
            return;
        }
    }
};

#endif