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
#ifndef IMAGE_H
#define IMAGE_H

#include "sb-engine.hpp"
#include "./vulkan/vulkan.hpp"

#include "./logicaldevice.hpp"
#include "./vulkanresultlookup.hpp"

using namespace std;

namespace SBE
{
    class Image
    {
    private:
        LogicalDevice* parent;
        VkImage self;

        VkImageCreateInfo* creationInfo;

    public:
        // Constructors
        //----------------------------------

        // Assuming you know everything you want to do, just do it.
        Image(LogicalDevice* parent, VkImageCreateInfo* creationInfo)
        {
            this->creationInfo=creationInfo;
            this->parent=parent;

            VkResult result = vkCreateImage(parent->getSelf(), creationInfo, parent->getHost()->getAllocationInfo(), &self);
            log->debug(string("Attempted to create an image with result: ").append(VkResultLookup(result)));
            
            if(result!=VK_SUCCESS) throw new exception();

            int texelCount = creationInfo->extent.depth * creationInfo->extent.width * creationInfo->extent.height;
            


        }

        // Mutators
        //----------------------------------

        // Accessors
        //----------------------------------
        auto getSelf(){return self;}
        auto getParent(){return parent;}
        auto getCreationInfo(){return creationInfo;}

        // Operators
        //----------------------------------

        // Destructors
        //----------------------------------

        
    };
};
#endif