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
#ifndef HITMESH_H
#define HITMESH_H

#include <vector>
#include "GAMEPLAY_hitbox.hpp"
#include "SYSTEMS_idman.hpp"

using namespace std;

class Hitmesh
{
private:
    int id;
    vector<Hitbox> mesh;

public:
    static IdManager hitmeshIDManager;

    // Constructors
    //----------------------------------
    Hitmesh()
    {
        id = hitmeshIDManager.allocate();
        mesh = vector<Hitbox>(0);
    }
    Hitmesh(vector<Hitbox> mesh)
    {
        hitmeshIDManager.free(id);
        mesh.clear();
    }

    // Accessors
    //----------------------------------
    vector<Hitbox> getMesh()
    {
        return mesh;
    }

    // Mutators
    //----------------------------------
    void addToMesh(Hitbox toAdd)
    {
        mesh.push_back(toAdd);
    }

    void addToMesh(vector<Hitbox> toAdd)
    {
        while (!toAdd.empty())
        {
            mesh.push_back(toAdd.back());
            toAdd.pop_back();
        }
    }

    void setMesh(vector<Hitbox> toSet)
    {
        mesh = toSet;
    }

    // Movement functions
    //----------------------------------
    void moveAll(pair<int, int> translation, int depth)
    {
        for (int i = 0; i < mesh.size(); i++)
        {
            mesh[i].shift(translation, depth);
        }
    }
    void gotoAll(pair<int, int> translation, int depth, bool collisionOverride = false)
    {
        for (int i = 0; i < mesh.size(); i++)
        {
            mesh[i].goTo(translation, depth, collisionOverride);
        }
    }
};

#endif