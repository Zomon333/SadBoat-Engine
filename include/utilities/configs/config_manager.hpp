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
#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <unordered_map>
#include <string>

#include "utilities/configs/config_node.hpp"
#include "utilities/configs/config.hpp"
#include "resources/id_manager.hpp"
#include "events/event.hpp"

namespace SBE
{
    class ConfigManager
    {
    private:
        IDManager configsIDs;

        std::unordered_map<int, Config*> intConfig;
        std::unordered_map<std::string, Config*> strConfig;

        std::unordered_map<std::string, Event<void*, ConfigNode>*> callbacks;

    public:
        ConfigManager();

        // Initializes the configManager, loading in other configs from the given initConfig.
        // This will load all configs within <Config> tags in initConfig.
        // Format: <Config desc="">filename</Config>
        ConfigManager(std::string initConfig);

        Config* loadConfig(std::string filename, std::string desc="Generic Config");

        void unloadConfig(std::string filename);

        Config* getConfig(int id);

        Config* getConfig(std::string filename);

        void assignCallback(std::string xmlTag, Event<void*, ConfigNode>* callback);

        Event<void*, ConfigNode>* getCallback(std::string xmlTag);

        Config* operator[](int id);

        Config* operator[](std::string filename);

    };
};
#endif