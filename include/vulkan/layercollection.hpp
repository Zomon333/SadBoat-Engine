/*
Copyright 2023 Dagan Poulin, Justice Guillory
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
#ifndef LAYER_COLLECTION_H
#define LAYER_COLLECTION_H

#include "sb-engine.hpp"
#include "./vulkan/vulkan.hpp"

using namespace std;

namespace SBE
{
    class LayerCollection
    {
    private:
        VkResult result;
        PhysicalDevice* parent;

        unsigned int count;
        vector<VkLayerProperties> properties;

    public:
        // Constructors
        LayerCollection(PhysicalDevice* parent)
        {
            this->parent=parent;
            update();
        }


        // Accessors
        auto getProps(){return properties;}
        auto getProp(int which){return properties[which];}

        // Mutators
        void update()
        {
            result = vkEnumerateDeviceLayerProperties(parent->getDevice(), &count, nullptr);
            properties.resize(count);
            result = vkEnumerateDeviceLayerProperties(parent->getDevice(), &count, (properties.data()));
            
        }

        
    };
};
#endif