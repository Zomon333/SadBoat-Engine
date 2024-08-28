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
#ifndef IMAGE_H
#define IMAGE_H

#include "./vulkan/vulkan.hpp"

#include "vulkan/logical_device.hpp"
#include "vulkan/vulkan_result_lookup.hpp"

namespace SBE
{
    class Image
    {
    private:
        LogicalDevice *parent;
        VkImageCreateInfo *creationInfo;
        VkImage self;

        VkImageFormatProperties imageProperties;

        VkMemoryAllocateInfo allocationInfo;
        VkMemoryRequirements memReqs;
        VkDeviceMemory internalBacking;

        bool sparse;
        bool mapped;
        void *mapLocation;

        // Allocate memory into internalBacking
        void allocate();

        void allocate(VkDeviceSize imageSize);

        // Bind the internalBacking to the VkImage self
        void bind();

    public:
        // Constructors
        //----------------------------------

        // Assuming you know everything you want to do, just do it.
        Image(LogicalDevice *parent, VkImageCreateInfo *creationInfo, bool sparse = false);

        // Mutators
        //----------------------------------

        // Accessors
        //----------------------------------
        VkImage getSelf();
        LogicalDevice *getParent();
        VkImageCreateInfo *getCreationInfo();
        bool isMapped();
        void *getMappedLocation();

        void *map();
        void *map(std::pair<unsigned int, unsigned int> toMap);

        void unmap();

        // Operators
        //----------------------------------

        // Destructors
        //----------------------------------
        ~Image();
    };
};
#endif