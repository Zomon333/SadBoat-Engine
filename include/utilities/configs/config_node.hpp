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
#ifndef CONFIG_NODE_H
#define CONFIG_NODE_H

#include <string>
#include <vector>
#include <variant>

namespace SBE
{
    class ConfigNode
    {
    private:
        enum CONTENT_TYPE
        {
            STRING,
            NODES
        };
        CONTENT_TYPE contType;

        // std::variant allows you to store one of multiple data types in the variable at one time. It does not allow both, but can be interchanged.
        // This is used alongside the CONTENT_TYPE enum to create a discriminated variant.
        // We always know exactly what's in the variant, even though it can change.
        std::variant<std::string,std::vector<ConfigNode>> contents = std::variant<std::string,std::vector<ConfigNode>>();


        std::string name;
        std::vector<std::pair<std::string, std::string>> attribs;

    public:
        // Constructors
        //----------------------------------

        // Initialize a configuration node with default values
        ConfigNode();

        // Initialize a configuration node with custom values
        ConfigNode(std::string name, std::vector<std::pair<std::string, std::string>> attribs, std::variant<std::string,std::vector<ConfigNode>> contents, CONTENT_TYPE contType);
        

        // Mutators
        //----------------------------------

        // Set node name
        void setName(std::string nName);

        // Set node attributes
        void setAttribs(std::vector<std::pair<std::string,std::string>> nAttribs);

        // Set node contents to a string
        void setContents(std::string nCont);

        // Set node contents to an array of other nodes
        void setContents(std::vector<ConfigNode> nCont);


        // Add attributes to the node
        void addAttribs(std::vector<std::pair<std::string,std::string>> nAttribs);

        // Add one attribute to the node
        void addAttrib(std::pair<std::string,std::string> nAttrib);

        // Add contents to the node. Optionally, overwrite it if it's a string.
        void addContents(ConfigNode nContent, bool overwrite=false);

        // Accessors
        //----------------------------------

        // Get the name of the node
        std::string getName();

        // Get the node's attributes
        std::vector<std::pair<std::string, std::string>> getAttribs();

        // Get the node's contents, given their data type. String by default.
        template<class r>
        r getContents()
        {
            return std::get<r>(this->contents);
        }

        // Print the node's contents. Optionally, define spacing for legibility.
        void print(std::string spacer="-", std::string prefix="");
    
        // Revert the node back to XML
        std::string unparse();

        // Operators
        //----------------------------------

        // Search node's children for node of given name. Returns an array of results.
        std::vector<ConfigNode> operator[](std::string rhs);
    
        // Destructors
        //----------------------------------

    };
};
#endif


    
