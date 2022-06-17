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
#ifndef COMP_OBJ_H
#define COMP_OBJ_H

#include "OBJECTS_controlledentity.hpp"
#include "OBJECTS_decal.hpp"
#include "OBJECTS_entity.hpp"
#include "OBJECTS_interactable.hpp"
#include "OBJECTS_item.hpp"
#include "OBJECTS_object.hpp"
#include "OBJECTS_projectile.hpp"
#include "OBJECTS_tile.hpp"
#include "EVENTS_function.hpp"
#include <variant>

using namespace std;

class CompositeObject : public ControlledEntity
{
private:
    vector<std::variant<ControlledEntity, Interactable, Item, Entity, Decal, Tile, Projectile, Object>> composite;

public:
    // Constructors
    //----------------------------------
    CompositeObject();
    CompositeObject(vector<std::variant<ControlledEntity, Interactable, Item, Entity, Decal, Tile, Projectile, Object>> composite);
    CompositeObject(std::variant<ControlledEntity*, Interactable*, Item*, Entity*, Decal*, Tile*, Projectile*, Object*> existingObjects);

    // Object Event Suppression
    //----------------------------------
    int suppressAll();
    int suppressAllByID(int id);
    int suppress(std::variant<ControlledEntity*, Interactable*, Item*, Entity*, Decal*, Tile*, Projectile*, Object*> toSuppress)
    int suppress(std::variant<ControlledEntity*, Interactable*, Item*, Entity*, Decal*, Tile*, Projectile*, Object*> toSuppress, Function* functionToSuppress)
    int suppress(int id, Function* functionToSuppress);

    // Object Event Replacement
    //----------------------------------
    int replace(std::variant<ControlledEntity*, Interactable*, Item*, Entity*, Decal*, Tile*, Projectile*, Object*> toSuppress, Function* functionToReplace, Function* functionToReplaceWith)
    int replace(int id, Function* functionToSuppress, Function* functionToReplaceWith);

    // Object Event Release
    //----------------------------------
    int releaseAll();
    int releaseAllByID(int id);
    int release(std::variant<ControlledEntity*, Interactable*, Item*, Entity*, Decal*, Tile*, Projectile*, Object*> toRelease);
    int release(std::variant<ControlledEntity*, Interactable*, Item*, Entity*, Decal*, Tile*, Projectile*, Object*> toRelease, Function* functionToRelease)
    int release(int id, Function* functionToRelease);

    // CompositeObject Composition
    //----------------------------------
    int compose();
    int compose(int id);
    int compose(std::variant<ControlledEntity*, Interactable*, Item*, Entity*, Decal*, Tile*, Projectile*, Object*> toAdd);

    // CompositeObject Decomposition
    //----------------------------------
    int decompose();
    int decompose(int id);
    int decompose(std::variant<ControlledEntity*, Interactable*, Item*, Entity*, Decal*, Tile*, Projectile*, Object*> toRemove);
    
};

#endif