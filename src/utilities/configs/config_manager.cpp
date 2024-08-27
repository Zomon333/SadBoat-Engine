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
#include <unordered_map>
#include <string>

#include "utilities/configs/config_manager.hpp"
#include "utilities/logging/startup_logger.hpp"
#include "utilities/configs/config_node.hpp"
#include "utilities/configs/config.hpp"
#include "resources/id_manager.hpp"
#include "events/event.hpp"

namespace SBE
{
    ConfigManager::ConfigManager()
    {
        // Intentionally blank
    }

    // Initializes the configManager, loading in other configs from the given initConfig.
    // This will load all configs within <Config> tags in initConfig.
    // Format: <Config desc="">filename</Config>
    ConfigManager::ConfigManager(std::string initConfig)
    {
        this->loadConfig(initConfig, "Initialization config. Used to load other configs.");
    }

    Config* ConfigManager::loadConfig(std::string filename, std::string desc)
    {
        SBE::log->info(std::string("Loading ").append(filename).append(" as \"").append(desc).append("\""));

        int id = configsIDs.allocate();

        intConfig[id] = new Config(filename, id, &callbacks, desc);
        strConfig[filename] = intConfig[id];

        return intConfig[id];
    }

    void ConfigManager::unloadConfig(std::string filename)
    {
        int id = strConfig[filename]->getID();
        delete intConfig[id];
        strConfig[filename]=nullptr;
        
        configsIDs.free(id);
    }

    Config* ConfigManager::getConfig(int id)
    {
        return intConfig[id];
    }

    Config* ConfigManager::getConfig(std::string filename)
    {
        return strConfig[filename];
    }

    void ConfigManager::assignCallback(std::string xmlTag, Event<void*, ConfigNode>* callback)
    {
        (callbacks)[xmlTag]=callback;
    }

    Event<void*, ConfigNode>* ConfigManager::getCallback(std::string xmlTag)
    {
        return callbacks[xmlTag];
    }

    Config* ConfigManager::operator[](int id)
    {
        return intConfig[id];
    }

    Config* ConfigManager::operator[](std::string filename)
    {
        return strConfig[filename];
    }
};