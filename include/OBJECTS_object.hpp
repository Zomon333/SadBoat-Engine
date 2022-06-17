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

#include <unordered_map>
#include <vector>
#include <string>

#include "EVENTS_ALLEVENTS.hpp"

#include "GAMEPLAY_hitmesh.hpp"

#include "GRAPHICS_sprite.hpp"
#include "GRAPHICS_animation.hpp"

#include "SYSTEMS_iddeclares.hpp"
#include "SYSTEMS_idman.hpp"

#include "PHYSICS_energy.hpp"
#include "PHYSICS_material.hpp"
#include "PHYSICS_interactions.hpp"
#include "PHYSICS_explosioninteraction.hpp"
#include "PHYSICS_impactinteraction.hpp"
#include "PHYSICS_penetrationinteraction.hpp"

using namespace std;

class Object : public Hitmesh
{
private:
    int id;
    
    vector<string> spriteNames;
    unordered_map<string, Sprite> sprites;

    vector<string> animationNames;
    unordered_map<string, Animation> animations;

    vector<string> functionNames;
    unordered_map<string, Function*> functions;

    Material composition;

public:
    static IdManager objectIDManager;
    
    // Constructors
    //----------------------------------
    Object();
    Object(Hitmesh initializerHitmesh);
    Object(Hitmesh initializerHitmesh, Sprite defaultSprite);
    Object(Hitmesh initializerHitmesh, Sprite defaultSprite, pair<int, int> location);

    
    // Event handlers
    //----------------------------------
    void throwEvent(Function* toThrow);
    void throwEvent(int idToThrow);
    void throwEvent(string eventNameToThrow);

    // Mutators
    //----------------------------------
    void newFunction(Function* toAdd, string addedName);
    void removeFunction(Function* toRemove);
    void removeFunction(int idToRemove);

    // Physics
    //----------------------------------
    void react(Energy toReactTo);
    void react(std::variant<Interaction, ExplosionInteraction, ImpactInteraction, PenetrationInteraction> toReactTo);
    void react(Object collider);
};

#endif