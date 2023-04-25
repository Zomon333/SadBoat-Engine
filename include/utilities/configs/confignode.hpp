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
#ifndef CONFIG_NODE_H
#define CONFIG_NODE_H

#include "sb-engine.hpp"

using namespace std;

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
        variant<string,vector<ConfigNode>> contents = variant<string,vector<ConfigNode>>();


        string name;
        vector<pair<string, string>> attribs;

    public:
        // Constructors
        //----------------------------------

        // Initialize a configuration node with default values
        ConfigNode()
        {
            this->name="";
            this->attribs=vector<pair<string,string>>();
            this->contents="";
            this->contType=STRING;
        }

        // Initialize a configuration node with custom values
        ConfigNode(string name, vector<pair<string, string>> attribs, variant<string,vector<ConfigNode>> contents, CONTENT_TYPE contType)
        {
            this->name=name;
            this->attribs=attribs;
            this->contents=contents;
            this->contType=contType;
        }
        

        // Mutators
        //----------------------------------

        // Set node name
        void setName(string nName)
        {
            this->name=nName;
        }
        // Set node attributes
        void setAttribs(vector<pair<string,string>> nAttribs)
        {
            this->attribs=nAttribs;
        }
        // Set node contents to a string
        void setContents(string nCont)
        {
            this->contents=nCont;
            this->contType=STRING;
        }    
        // Set node contents to an array of other nodes
        void setContents(vector<ConfigNode> nCont)
        {
            this->contents=nCont;
            this->contType=NODES;
        }


        // Add attributes to the node
        void addAttribs(vector<pair<string,string>> nAttribs)
        {
            for(int i=0; i<nAttribs.size(); i++)
            {
                this->attribs.push_back(nAttribs[i]);
            }
        }
        // Add one attribute to the node
        void addAttrib(pair<string,string> nAttrib)
        {
            this->attribs.push_back(nAttrib);
        }
        // Add contents to the node. Optionally, overwrite it if it's a string.
        void addContents(ConfigNode nContent, bool overwrite=false)
        {
            if(overwrite || contType==NODES)
            {
                auto vec = std::get<vector<ConfigNode>>(this->contents);
                vec.push_back(nContent);
                this->contents=vec;
            }
        }

        // Accessors
        //----------------------------------

        // Get the name of the node
        auto getName()
        {
            return this->name;
        }
        // Get the node's attributes
        auto getAttribs()
        {
            return this->attribs;
        }
        // Get the node's contents, given their data type. String by default.
        template<class r=string>
        r getContents()
        {
            return std::get<r>(this->contents);
        }

        // Print the node's contents. Optionally, define spacing for legibility.
        void print(string spacer="-", string prefix="")
        {
            cout<<prefix<<"Name: "<<name<<" Attribs: ";
            for(int i=0; i<attribs.size(); i++)
            {
                cout<<"("<<attribs[i].first<<","<<attribs[i].second<<") ";
            }
            
            if(contType==STRING)
            {
                cout<<""<<"Contents: ";
                cout<<""<<std::get<string>(contents)<<endl;
            }

            else
            {
                string tmp = prefix.append(spacer);
                for(int i=0; i<std::get<vector<ConfigNode>>(contents).size(); i++)
                {
                    cout<<endl;
                    std::get<vector<ConfigNode>>(contents)[i].print(spacer, tmp);
                }
            }
        }
    
        // Revert the node back to XML
        string unparse()
        {
            string openingTag = "<";
            openingTag+=name;
            if(attribs.size()>0)
            {
                for(int i=0; i<attribs.size(); i++)
                {
                    openingTag+=" ";
                    openingTag+=attribs[i].first;
                    openingTag+="=\"";
                    openingTag+=attribs[i].second;
                    openingTag+="\"";
                }
            }
            openingTag+=">";
            

            string closingTag = "</";
            closingTag+=name;
            closingTag+=">";

            if(contType==STRING)
            {
                openingTag.append(get<string>(contents));
            }
            else
            {
                string innerResults="";
                vector<ConfigNode> children = get<vector<ConfigNode>>(contents);
                for(int i=0; i<children.size(); i++)
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
        vector<ConfigNode> operator[](string rhs)
        {
            if(contType==NODES)
            {
                vector<ConfigNode> toReturn;
                for(int i=0; i<std::get<vector<ConfigNode>>(contents).size(); i++)
                {
                    if(std::get<vector<ConfigNode>>(contents)[i].name==rhs)
                    {
                        toReturn.push_back(std::get<vector<ConfigNode>>(contents)[i]);
                    }
                }
                if(toReturn.size()==0)
                {
                    throw new runtime_error((string("").append("Parameter of ").append(rhs).append(" not found within Config.")));
                }
                
                return toReturn;
            }
            
            return vector<ConfigNode>();
        }
    
        // Destructors
        //----------------------------------

    };
};
#endif


    
