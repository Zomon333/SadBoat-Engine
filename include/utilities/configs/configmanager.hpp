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
#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include "sb-engine.hpp"

using namespace std;

namespace SBE
{
    class ConfigManager
    {
    private:
        IDManager configsIDs;

        unordered_map<int, Config*> intConfig;
        unordered_map<string, Config*> strConfig;

    public:
        // Initializes the configManager, loading in other configs from the given initConfig.
        // This will load all configs within <Config> tags in initConfig.
        // Format: <Config desc="">filename</Config>
        ConfigManager(string initConfig)
        {
            auto otherConfigs = (this->loadConfig(initConfig, "Initialization config. Used to load other configs."))->operator[]("Config");
            for(int i = 0; i<otherConfigs.size(); i++)
            {
                this->loadConfig(
                    otherConfigs[i].getContents<string>(),
                    otherConfigs[i].getAttribs()[0].second
                );
            }
        }

        Config* loadConfig(string filename, string desc="")
        {
            int id = configsIDs.allocate();
            intConfig[id] = new Config(filename, id, desc);
            strConfig[filename] = intConfig[id];

            return intConfig[id];
        }

        void unloadConfig(string filename)
        {
            int id = strConfig[filename]->getID();
            delete intConfig[id];
            strConfig[filename]=nullptr;
            
            configsIDs.free(id);
        }

        Config* getConfig(int id)
        {
            return intConfig[id];
        }
        Config* getConfig(string filename)
        {
            return strConfig[filename];
        }


        Config* operator[](int id)
        {
            return intConfig[id];
        }
        Config* operator[](string filename)
        {
            return strConfig[filename];
        }

    };
};
#endif