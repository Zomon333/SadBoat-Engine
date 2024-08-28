/*
Copyright 2024 Dagan Poulin, Justice Guillory
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

#include "vulkan.hpp"

#include <vector>

#include "vulkan/physical_device.hpp"

namespace SBE
{
    // LayerCollection: Graphics
    // Generalized container for querying and accessing the various layers available on the PhysicalDevice provided

    class LayerCollection
    {
    private:
        VkResult result;
        PhysicalDevice *parent;

        unsigned int count;
        std::vector<VkLayerProperties> properties;

    public:
        // Constructors
        LayerCollection(PhysicalDevice *parent);

        // Accessors
        std::vector<VkLayerProperties> getProps();
        VkLayerProperties getProp(int which);

        // Mutators
        void update();
    };
};
#endif