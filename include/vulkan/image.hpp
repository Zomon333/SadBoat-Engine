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
        VkImageCreateInfo* creationInfo;
        VkImage self;

        VkImageFormatProperties imageProperties;

        VkMemoryAllocateInfo allocationInfo;
        VkMemoryRequirements memReqs;
        VkDeviceMemory internalBacking;

        bool sparse;
        bool mapped;
        void* mapLocation;

        // Allocate memory into internalBacking
        void allocate()
        {
            // Check the memory types supported by the device
            auto mem = parent->getParent()->getMem();
            int optimalMemIndex = -1;

            // Search through them all for one that is on the GPU but CPU visible
            for(int i=0; i<mem->memoryTypeCount; i++)
            {
                auto flags = mem->memoryTypes[i].propertyFlags;
                if(flags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT && flags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
                {
                    // Save it
                    optimalMemIndex = i;
                    break;
                }
            }
            if(optimalMemIndex==-1)
            {
                // If there aren't any, then throw an exception and don't allocate.
                log->error("Required memory type not supported-- should support VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT and VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT");
                throw new bad_alloc();
            }

            // Verify we're even *able* to allocate more memory types.
            auto allocs = parent->incAllocs();
            auto maxAllocs = parent->getParent()->getProperties()->limits.maxMemoryAllocationCount;
            log->info(string("Allocating memory for image, allocation number ").append(to_string(allocs)).append(" / ").append(to_string(maxAllocs)));
            if(allocs>maxAllocs)
            {
                parent->decAllocs();
                log->error("Maximum number of resource allocations exceeded.");
                throw new bad_alloc();
            }

            if(this->memReqs.size > imageProperties.maxResourceSize)
            {
                stringstream error;
                error<<"Image exceeds maximum image size. Attempted to use "<<this->memReqs.size<<" out of "<<imageProperties.maxResourceSize<<" bytes.";
                log->critical(&error);
                throw new exception();
            }

            // Assign allocation info
            allocationInfo = {
                VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
                nullptr,
                this->memReqs.size,
                (unsigned int)(optimalMemIndex) // memoryTypeIndex, determining what type of memory we need, which should be chosen from vkGetPhysicalDeviceMemoryProperties.
            };

            // Attempt to allocate, output result, throw if invalid.
            auto result = vkAllocateMemory((parent->getSelf()), &allocationInfo, (parent->getHost()->getAllocationInfo()), &internalBacking);
            log->info(string("Image memory allocated with size of ").append(to_string((this->memReqs.size))).append(", resulting: ").append(VkResultLookup(result)));
            if(result!=VK_SUCCESS) throw new bad_alloc();
        }
    
        void allocate(VkDeviceSize imageSize)
        {
            // Check the memory types supported by the device
            auto mem = parent->getParent()->getMem();
            int optimalMemIndex = -1;

            // Search through them all for one that is on the GPU but CPU visible
            for(int i=0; i<mem->memoryTypeCount; i++)
            {
                auto flags = mem->memoryTypes[i].propertyFlags;
                if(flags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT && flags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
                {
                    // Save it
                    optimalMemIndex = i;
                    break;
                }
            }
            if(optimalMemIndex==-1)
            {
                // If there aren't any, then throw an exception and don't allocate.
                log->error("Required memory type not supported-- should support VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT and VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT");
                throw new bad_alloc();
            }

            // Verify we're even *able* to allocate more memory types.
            auto allocs = parent->incAllocs();
            auto maxAllocs = parent->getParent()->getProperties()->limits.maxMemoryAllocationCount;
            log->info(string("Allocating memory for image, allocation number ").append(to_string(allocs)).append(" / ").append(to_string(maxAllocs)));
            if(allocs>maxAllocs)
            {
                parent->decAllocs();
                log->error("Maximum number of resource allocations exceeded.");
                throw new bad_alloc();
            }

            // Assign allocation info
            allocationInfo = {
                VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
                nullptr,
                imageSize,
                (unsigned int)(optimalMemIndex) // memoryTypeIndex, determining what type of memory we need, which should be chosen from vkGetPhysicalDeviceMemoryProperties.
            };

            // Attempt to allocate, output result, throw if invalid.
            auto result = vkAllocateMemory((parent->getSelf()), &allocationInfo, (parent->getHost()->getAllocationInfo()), &internalBacking);
            log->info(string("Image memory allocated with size of ").append(to_string((imageSize))).append(", resulting: ").append(VkResultLookup(result)));
            if(result!=0) throw new bad_alloc();
        }

        // Bind the internalBacking to the VkImage self
        void bind()
        {
            auto result = vkBindImageMemory(parent->getSelf(), self, internalBacking, 0);

            stringstream tmpStream;
            tmpStream<<"Binding image memory to image. Bitcheck is not currently functional. Bind result: "<<VkResultLookup(result);
            log->info(&tmpStream);

            mapped=false;
        }
    public:
        // Constructors
        //----------------------------------

        // Assuming you know everything you want to do, just do it.
        Image(LogicalDevice* parent, VkImageCreateInfo* creationInfo, bool sparse=false)
        {
            this->creationInfo=creationInfo;
            this->parent=parent;
            this->sparse=sparse;

            stringstream debuglog;
            if(!sparse && (creationInfo->flags & VK_IMAGE_CREATE_SPARSE_ALIASED_BIT || creationInfo->flags & VK_IMAGE_CREATE_SPARSE_RESIDENCY_BIT || creationInfo->flags & VK_IMAGE_CREATE_SPARSE_BINDING_BIT))
            {
                debuglog<<"Attempting to use flags for sparse binding without indicating image is sparse. Overwriting.";
                log->warn(&debuglog);
            }

            if(sparse && (!(creationInfo->flags & VK_IMAGE_CREATE_SPARSE_ALIASED_BIT) || !(creationInfo->flags & VK_IMAGE_CREATE_SPARSE_RESIDENCY_BIT) || !(creationInfo->flags & VK_IMAGE_CREATE_SPARSE_BINDING_BIT)))
            {
                debuglog<<"Attempting to indicate image is sparse without clarifying sparse usage.";
                log->critical(&debuglog);
                throw new exception();
            }

            VkResult result = vkGetPhysicalDeviceImageFormatProperties(
                this->parent->getParent()->getDevice(),
                creationInfo->format,
                creationInfo->imageType,
                creationInfo->tiling,
                creationInfo->usage,
                creationInfo->flags,
                &imageProperties
            );
            debuglog<<"Querying device format properties. Result from creationInfo: "<<VkResultLookup(result);
            log->info(&debuglog);

            if(creationInfo->extent.depth > imageProperties.maxExtent.depth || creationInfo->extent.width > imageProperties.maxExtent.width || creationInfo->extent.height > imageProperties.maxExtent.height)
            {
                debuglog<<"Creation info is outside of maximum extent bounds. Using ("<<creationInfo->extent.depth<<","<<creationInfo->extent.height<<","<<creationInfo->extent.width<<") out of ";
                debuglog<<"("<<imageProperties.maxExtent.depth<<","<<imageProperties.maxExtent.height<<","<<imageProperties.maxExtent.width<<")";
                log->critical(&debuglog);
                throw new exception();
            }
            if(creationInfo->mipLevels > imageProperties.maxMipLevels)
            {
                debuglog<<"Creation info is outside of maximum mip levels. Using "<<creationInfo->mipLevels<<" out of "<<imageProperties.maxMipLevels<<" mipmap levels.";
                log->critical(&debuglog);
                throw new exception();
            }
            if(creationInfo->arrayLayers > imageProperties.maxArrayLayers)
            {
                debuglog<<"Creation info is outside of maximum array layers. Using "<<creationInfo->arrayLayers<<" out of "<<imageProperties.maxArrayLayers<<" layers.";
                log->critical(&debuglog);
                throw new exception();
            }
            if((creationInfo->samples & imageProperties.sampleCounts)==0)
            {
                debuglog<<"Creation info uses unsupported sample count. Using sample mode "<<creationInfo->samples<<" when device only supports "<<imageProperties.sampleCounts<<".";
                log->critical(&debuglog);
                throw new exception();
            }
            
            result = vkCreateImage(parent->getSelf(), creationInfo, parent->getHost()->getAllocationInfo(), &self);
            log->info(string("Attempted to create an image with result: ").append(VkResultLookup(result)));
            
            if(result!=VK_SUCCESS){
                stringstream error;
                error<<"Failed to create image. Result: "<<VkResultLookup(result);
                log->critical(&error);

                throw new exception();
            }

            vkGetImageMemoryRequirements(parent->getSelf(),self,&memReqs);
            this->allocate();
            this->bind();
        }

        // Mutators
        //----------------------------------

        // Accessors
        //----------------------------------
        auto getSelf(){return self;}
        auto getParent(){return parent;}
        auto getCreationInfo(){return creationInfo;}
        bool isMapped(){return mapped;}
        void* getMappedLocation(){return mapLocation;}


        void* map()
        {
            if((parent->getParent()->getMem()->memoryTypes[allocationInfo.memoryTypeIndex].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)==0)
            {
                return nullptr;
            }
            if(mapped) 
            {
                return nullptr;
            }

            this->mapped=true;

            void* data;
            auto result = vkMapMemory(this->parent->getSelf(), internalBacking, 0, (unsigned int)(memReqs.size), 0, &data);
            if(result!=VK_SUCCESS)
            {
                stringstream errorLog;
                errorLog<<"Failed to map Image class. Error: "<<VkResultLookup(result);
                log->critical(&errorLog);
            }
            this->mapLocation=data;
            return data;
        }
        void* map(Range toMap)
        {
            if((parent->getParent()->getMem()->memoryTypes[allocationInfo.memoryTypeIndex].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)==0)
            {
                stringstream errorLog;
                errorLog<<"Failed to map Image class. Error: "<<"Map not possible, Host visible bit not set";
                log->error(&errorLog);
                return nullptr;
            }

            if(mapped || toMap.getMax()>allocationInfo.allocationSize) 
            {
                stringstream errorLog;
                errorLog<<"Failed to map Image class. Error: "<<"Map not possible, allocations out of bounds";
                log->error(&errorLog);
                return nullptr;
            }
            mapped=true;

            auto result = vkMapMemory(parent->getSelf(), internalBacking, toMap.getMin(), toMap.getMax(), {}, &mapLocation);
            if(result!=VK_SUCCESS)
            {
                stringstream errorLog;
                errorLog<<"Failed to map Image class. Error: "<<VkResultLookup(result);
                log->critical(&errorLog);
                throw new exception();
            }
            return mapLocation;
        }
        
        void unmap()
        {
            mapped=false;
            this->mapLocation=nullptr;
            vkUnmapMemory(this->parent->getSelf(), internalBacking);
        }

        // Operators
        //----------------------------------

        // Destructors
        //----------------------------------
        ~Image()
        {
            vkDestroyImage(parent->getSelf(), self, parent->getHost()->getAllocationInfo());
        }
        
    };
};
#endif