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

#include "rapidxml-1.13/rapidxml_utils.hpp"
#include "rapidxml-1.13/rapidxml.hpp"

#include "utilities/logging/startup_logger.hpp"
#include "utilities/configs/config.hpp"
#include "events/event.hpp"

#include <string>
#include <vector>
#include <unordered_map>

namespace SBE
{
    // A recursive node parser for an XML file.
    ConfigNode Config::parseNode(rapidxml::xml_node<char>* node)
    {
        // The data we're preparing to return
        ConfigNode toReturn;

        // Get the node name
        toReturn.setName(node->name());
        
        // Get the node attributes
        auto attrib = node->first_attribute();
        while(attrib!=0)
        {
            toReturn.addAttrib(std::make_pair<std::string,std::string>(attrib->name(),attrib->value()));
            attrib = attrib->next_attribute();
        }

        // Recursive case: Node contains more nodes. Call the node parser on the contained nodes.
        if(node->first_node()->type()!=rapidxml::node_type::node_data && node->first_node()->type()!=rapidxml::node_type::node_cdata)
        {
            // Create a store for those nodes
            std::vector<ConfigNode> children;

            auto next = node->first_node();
            while(next!=0)
            {
                // Find the nodes and recursively parse and store them
                children.push_back(parseNode(next));
                next = next->next_sibling();
            }

            // Set the results of the node hunt to the storage in the node
            toReturn.setContents(children);
            
            if((*callbacks)[toReturn.getName()]!=nullptr)
            {
                (*callbacks)[toReturn.getName()]->call(toReturn);
            }

            // Return the node
            return toReturn;
        }
        // Base case: Node just contains a string, just parse as a normal value.
        else
        {
            toReturn.setContents(node->value());
            if((*callbacks)[toReturn.getName()]!=nullptr)
            {
                (*callbacks)[toReturn.getName()]->call(toReturn);
            }
        }
        
        return toReturn;
    }
    

    // Loads a file into the config.
    void Config::load(std::string filename)
    {
        try
        {
            rapidxml::file<> xmlFile(filename.c_str());
            this->docContents=xmlFile.data();
            this->doc.parse<0>(docContents);

            auto workingNode = doc.first_node();
            while(workingNode!=0)
            {
                this->nodes.push_back(this->parseNode(workingNode));
                workingNode = workingNode->next_sibling();
            }


        }
        catch(rapidxml::parse_error* e)
        {
            SBE::log->error("RapidXML Parsing error thrown.");
            SBE::log->error(std::string("Status: ").append(e->what()));
            return;
        } 
    }

    // Constructors
    //----------------------------------

    // Load a config given a file name and an ID
    Config::Config(std::string filename, int id, std::unordered_map<std::string, Event<void*, ConfigNode>*>* callbacks, std::string desc)
    {
        this->id=id;
        this->desc=desc;
        this->callbacks=callbacks;

        load(filename);
        return;
    }

    // Mutators
    //----------------------------------

    // Update the filename. Reloads file by default.
    void Config::setFilename(std::string filename, bool reload)
    {
        this->filename=filename;
            
        if(reload)
        {
            load(filename);
        }
    }

    // Sets the nodes in the config.
    void Config::setNodes(std::vector<ConfigNode> nodes)
    {
        this->nodes=nodes;
    }

    // Sets the document's contents in the config. Reparses by default.
    void Config::setDocContents(char* newContent, bool reparse)
    {
        this->docContents=newContent;
        if(reparse)
        {
            this->doc.parse<0>(docContents);
        }
    }

    // Sets the config's description.
    void Config::setDescription(std::string newDesc)
    {
        this->desc=newDesc;
    }

    // Adds nodes to the config.
    void Config::addNodes(std::vector<ConfigNode> nodes)
    {
        for(unsigned int i=0; i<nodes.size(); i++)
        {
            this->nodes.push_back(nodes[i]);
        }
    }

    // Adds a single node to the config.
    void Config::addNode(ConfigNode node)
    {
        this->nodes.push_back(node);
    }
    
    // Accessors
    //----------------------------------

    // Returns the config's ID.
    int Config::getID()
    {
        return id;
    }

    // Returns the config's filename.
    std::string Config::getFilename()
    {
        return filename;
    }

    // Returns the nodes in the config.
    std::vector<ConfigNode> Config::getNodes()
    {
        return nodes;
    }

    // Returns the documents raw contents.
    char* Config::getDocContents()
    {
        return docContents;
    }

    // Returns the config's description
    std::string Config::getDesc()
    {
        return desc;
    }

    // Prints the config to the console.
    void Config::print()
    {
        for(unsigned int i=0; i<nodes.size(); i++)
        {
            nodes[i].print();
        }
    }

    // Operators
    //----------------------------------
    
    // Returns the n-th element from the backing array
    ConfigNode Config::operator[](int rhs)
    {
        return nodes[rhs];
    }

    // Searches the backing array for XML tags with names matching the parameter.
    std::vector<ConfigNode> Config::operator[](std::string rhs)
    {
        std::vector<ConfigNode> toReturn;
        for(unsigned int i=0; i<nodes.size(); i++)
        {
            if(nodes[i].getName()==rhs)
            {
                toReturn.push_back(nodes[i]);
            }
        }

        if(toReturn.size()==0)
        {
            throw new std::runtime_error((std::string("").append("Parameter of ").append(rhs).append(" not found within Config.")));
        }
        return toReturn;
    }
};