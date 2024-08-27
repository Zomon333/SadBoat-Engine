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
#ifndef CONFIG_H
#define CONFIG_H

#include "rapidxml-1.13/rapidxml.hpp"
#include "utilities/configs/config_node.hpp"
#include "events/event.hpp"

#include <string>
#include <vector>
#include <unordered_map>

namespace SBE
{
    class Config
    {
    private:
        // ID of the config file
        int id;
        // Name and path of the config file
        std::string filename;

        // Description of the config
        std::string desc;

        // Nodes from the parsed file
        std::vector<ConfigNode> nodes;

        // Config Callbacks
        std::unordered_map<std::string, Event<void*, ConfigNode>*>* callbacks;

        // Raw XML document
        // Will be used in future implementations for config writing
        rapidxml::xml_document<> doc;
        char* docContents;

        // A recursive node parser for an XML file.
        ConfigNode parseNode(rapidxml::xml_node<char>* node);

        // Loads a file into the config.
        void load(std::string filename);

    public:
        
        // Constructors
        //----------------------------------

        // Load a config given a file name and an ID
        Config(std::string filename, int id, std::unordered_map<std::string, Event<void*, ConfigNode>*>* callbacks, std::string desc="");
    
        // Mutators
        //----------------------------------

        // Update the filename. Reloads file by default.
        void setFilename(std::string filename, bool reload=true);

        // Sets the nodes in the config.
        void setNodes(std::vector<ConfigNode> nodes);

        // Sets the document's contents in the config. Reparses by default.
        void setDocContents(char* newContent, bool reparse=true);

        // Sets the config's description.
        void setDescription(std::string newDesc);

        // Adds nodes to the config.
        void addNodes(std::vector<ConfigNode> nodes);

        // Adds a single node to the config.
        void addNode(ConfigNode node);
        
        // Accessors
        //----------------------------------

        // Returns the config's ID.
        int getID();

        // Returns the config's filename.
        std::string getFilename();

        // Returns the nodes in the config.
        std::vector<ConfigNode> getNodes();

        // Returns the documents raw contents.
        char* getDocContents();

        // Returns the config's description
        std::string getDesc();

        // Prints the config to the console.
        void print();

        // Operators
        //----------------------------------
        
        // Returns the n-th element from the backing array
        ConfigNode operator[](int rhs);

        // Searches the backing array for XML tags with names matching the parameter.
        std::vector<ConfigNode> operator[](std::string rhs);
    };
};
#endif