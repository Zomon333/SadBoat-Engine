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

#include <string>
#include <vector>
#include <variant>
#include <sstream>

#include "utilities/configs/config_node.hpp"
#include "utilities/logging/startup_logger.hpp"


namespace SBE
{

        // Constructors
        //----------------------------------

        // Initialize a configuration node with default values
        ConfigNode::ConfigNode()
        {
            this->name="";
            this->attribs=std::vector<std::pair<std::string,std::string>>();
            this->contents="";
            this->contType=STRING;
        }

        // Initialize a configuration node with custom values
        ConfigNode::ConfigNode(std::string name, std::vector<std::pair<std::string, std::string>> attribs, std::variant<std::string,std::vector<ConfigNode>> contents, CONTENT_TYPE contType)
        {
            this->name=name;
            this->attribs=attribs;
            this->contents=contents;
            this->contType=contType;
        }
        

        // Mutators
        //----------------------------------

        // Set node name
        void ConfigNode::setName(std::string nName)
        {
            this->name=nName;
        }

        // Set node attributes
        void ConfigNode::setAttribs(std::vector<std::pair<std::string,std::string>> nAttribs)
        {
            this->attribs=nAttribs;
        }

        // Set node contents to a string
        void ConfigNode::setContents(std::string nCont)
        {
            this->contents=nCont;
            this->contType=STRING;
        }

        // Set node contents to an array of other nodes
        void ConfigNode::setContents(std::vector<ConfigNode> nCont)
        {
            this->contents=nCont;
            this->contType=NODES;
        }


        // Add attributes to the node
        void ConfigNode::addAttribs(std::vector<std::pair<std::string,std::string>> nAttribs)
        {
            for(unsigned int i=0; i<nAttribs.size(); i++)
            {
                this->attribs.push_back(nAttribs[i]);
            }
        }

        // Add one attribute to the node
        void ConfigNode::addAttrib(std::pair<std::string,std::string> nAttrib)
        {
            this->attribs.push_back(nAttrib);
        }

        // Add contents to the node. Optionally, overwrite it if it's a string.
        void ConfigNode::addContents(ConfigNode nContent, bool overwrite)
        {
            if(overwrite || contType==NODES)
            {
                auto vec = std::get<std::vector<ConfigNode>>(this->contents);
                vec.push_back(nContent);
                this->contents=vec;
            }
        }

        // Accessors
        //----------------------------------

        // Get the name of the node
        std::string ConfigNode::getName()
        {
            return this->name;
        }

        // Get the node's attributes
        std::vector<std::pair<std::string, std::string>> ConfigNode::getAttribs()
        {
            return this->attribs;
        }

        // Print the node's contents. Optionally, define spacing for legibility.
        void ConfigNode::print(std::string spacer, std::string prefix)
        {
            std::stringstream strstream;
            strstream<<prefix<<"Name: "<<name<<" Attribs: ";
            for(unsigned int i=0; i<attribs.size(); i++)
            {
                strstream<<"("<<attribs[i].first<<","<<attribs[i].second<<") ";
            }
            
            if(contType==STRING)
            {
                strstream<<""<<"Contents: ";
                strstream<<""<<std::get<std::string>(contents)<<std::endl;
            }

            else
            {
                std::string tmp = prefix.append(spacer);
                for(unsigned int i=0; i<std::get<std::vector<ConfigNode>>(contents).size(); i++)
                {
                    strstream<<std::endl;
                    std::get<std::vector<ConfigNode>>(contents)[i].print(spacer, tmp);
                }
            }

            SBE::log->info(strstream.str());
        }
    
        // Revert the node back to XML
        std::string ConfigNode::unparse()
        {
            std::string openingTag = "<";
            openingTag+=name;
            if(attribs.size()>0)
            {
                for(unsigned int i=0; i<attribs.size(); i++)
                {
                    openingTag+=" ";
                    openingTag+=attribs[i].first;
                    openingTag+="=\"";
                    openingTag+=attribs[i].second;
                    openingTag+="\"";
                }
            }
            openingTag+=">";
            

            std::string closingTag = "</";
            closingTag+=name;
            closingTag+=">";

            if(contType==STRING)
            {
                openingTag.append(std::get<std::string>(contents));
            }
            else
            {
                std::string innerResults="";
                std::vector<ConfigNode> children = std::get<std::vector<ConfigNode>>(contents);
                for(unsigned int i=0; i<children.size(); i++)
                {
                    innerResults+=children[i].unparse();
                }
                openingTag.append(innerResults);
            }

            openingTag.append(closingTag);
            return openingTag;
        }

        // Operators
        //----------------------------------

        // Search node's children for node of given name. Returns an array of results.
        std::vector<ConfigNode> ConfigNode::operator[](std::string rhs)
        {
            if(contType==NODES)
            {
                std::vector<ConfigNode> toReturn;
                for(unsigned int i=0; i<std::get<std::vector<ConfigNode>>(contents).size(); i++)
                {
                    if(std::get<std::vector<ConfigNode>>(contents)[i].name==rhs)
                    {
                        toReturn.push_back(std::get<std::vector<ConfigNode>>(contents)[i]);
                    }
                }
                if(toReturn.size()==0)
                {
                    throw new std::runtime_error((std::string("").append("Parameter of ").append(rhs).append(" not found within Config.")));
                }
                
                return toReturn;
            }
            
            return std::vector<ConfigNode>();
        }
    
        // Destructors
        //----------------------------------
};


    
