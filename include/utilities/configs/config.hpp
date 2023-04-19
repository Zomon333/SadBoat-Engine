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
#ifndef CONFIG_H
#define CONFIG_H

#include "sb-engine.hpp"

using namespace std;
using namespace rapidxml;

namespace SBE
{
    class Config
    {
    private:
        // ID of the config file
        int id;
        // Name and path of the config file
        string filename;

        // Description of the config
        string desc;

        // Nodes from the parsed file
        vector<ConfigNode> nodes;

        // Config Callbacks
        unordered_map<string, Event<void*, ConfigNode>*>* callbacks;

        // Raw XML document
        // Will be used in future implementations for config writing
        xml_document<> doc;
        char* docContents;

        // A recursive node parser for an XML file.
        ConfigNode parseNode(rapidxml::xml_node<char>* node)
        {
            // The data we're preparing to return
            ConfigNode toReturn;


            // Get the node name
            toReturn.setName(node->name());
            
            // Get the node attributes
            auto attrib = node->first_attribute();
            while(attrib!=0)
            {
                toReturn.addAttrib(make_pair<string,string>(attrib->name(),attrib->value()));
                attrib = attrib->next_attribute();
            }

            // Recursive case: Node contains more nodes. Call the node parser on the contained nodes.
            if(node->first_node()->type()!=node_data && node->first_node()->type()!=node_cdata)
            {
                // Create a store for those nodes
                vector<ConfigNode> children;

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
        void load(string filename)
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
                cout<<"RapidXML Parsing error thrown.\nStatus: "<<e->what()<<endl;
                return;
            }

        }

    public:
        
        // Constructors
        //----------------------------------

        // Load a config given a file name and an ID
        Config(string filename, int id, string desc="", unordered_map<string, Event<void*, ConfigNode>*>* callbacks=nullptr)
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
        void setFilename(string filename, bool reload=true)
        {
            this->filename=filename;
            
            if(reload)
            {
                load(filename);
            }
        }
        // Sets the nodes in the config.
        void setNodes(vector<ConfigNode> nodes)
        {
            this->nodes=nodes;
        }
        // Sets the document's contents in the config. Reparses by default.
        void setDocContents(char* newContent, bool reparse=true)
        {
            this->docContents=newContent;
            if(reparse)
            {
                this->doc.parse<0>(docContents);
            }
        }
        // Sets the config's description.
        void setDescription(string newDesc)
        {
            this->desc=newDesc;
        }

        // Adds nodes to the config.
        void addNodes(vector<ConfigNode> nodes)
        {
            for(int i=0; i<nodes.size(); i++)
            {
                this->nodes.push_back(nodes[i]);
            }
        }
        // Adds a single node to the config.
        void addNode(ConfigNode node)
        {
            this->nodes.push_back(node);
        }
        
        // Accessors
        //----------------------------------

        // Returns the config's ID.
        auto getID()
        {
            return id;
        }
        // Returns the config's filename.
        auto getFilename()
        {
            return filename;
        }
        // Returns the nodes in the config.
        auto getNodes()
        {
            return nodes;
        }
        // Returns the documents raw contents.
        auto getDocContents()
        {
            return docContents;
        }
        // Returns the config's description
        auto getDesc()
        {
            return desc;
        }

        // Prints the config to the console.
        void print()
        {
            for(int i=0; i<nodes.size(); i++)
            {
                nodes[i].print();
            }
        }

        // Operators
        //----------------------------------
        
        // Returns the n-th element from the backing array
        ConfigNode operator[](int rhs)
        {
            return nodes[rhs];
        }
        // Searches the backing array for XML tags with names matching the parameter.
        vector<ConfigNode> operator[](string rhs)
        {
            vector<ConfigNode> toReturn;
            for(int i=0; i<nodes.size(); i++)
            {
                if(nodes[i].getName()==rhs)
                {
                    toReturn.push_back(nodes[i]);
                }
            }

            if(toReturn.size()==0)
            {
                throw new runtime_error((string("").append("Parameter of ").append(rhs).append(" not found within Config.")));
            }
            return toReturn;
        }
    };
};
#endif