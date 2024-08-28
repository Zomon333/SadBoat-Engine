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
#ifndef PDC_H
#define PDC_H

#include "./vulkan/vulkan.hpp"

#include "utilities/logging/startup_logger.hpp"
#include "utilities/configs/config.hpp"
#include "vulkan/instance.hpp"
#include "vulkan/physical_device.hpp"

namespace SBE
{
    // PhysicalDeviceCollection: Graphics
    // Generalized container for the querying and generation of PhysicalDevices using a given instance
    class PhysicalDeviceCollection
    {
    private:
        Instance *host;

        uint32_t numDevices;
        std::vector<PhysicalDevice *> devices;

    public:
        // Constructors
        //----------------------------------
        PhysicalDeviceCollection(Instance *newHost);

        // Mutators
        //----------------------------------

        // Gets how many devices are in this instance, and stores in numDevices. Also returns numDevices.
        uint32_t updateCount();

        // Gets devices currently in the instance, and stores in devices. Also returns devices.
        std::vector<PhysicalDevice *> updateDevices();

        // Accessors
        //----------------------------------

        Instance *getHost();
        uint32_t getNumDevices();
        std::vector<PhysicalDevice *> getDevices();

        PhysicalDevice *getOptimal(Config *deviceConfig = nullptr);
    };
};
#endif