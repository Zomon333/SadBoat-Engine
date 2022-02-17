/*
Copyright 2022 Dagan Poulin, Justice Guillory

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
#ifndef REG_H
#define REG_H

#include <unordered_map>
#include <string>
#include <ctime>
#include "idman.h"

using namespace std;

template<class Key, class Content> class Registry{
    private:
        int id;
        string registryName;
        string registryDescription;
        unordered_map<Key, Content, std::hash<Key>> *thisRegistry = new unordered_map<Key, Content, std::hash<Key>>();

    public:
        inline static IDMan* registryIDManager = new IDMan(std::time(nullptr), 5, DEFAULT);
        inline static unordered_map<int, Registry*, std::hash<int>> registrar; //Ideally, I'd want registrar to be a registry of registries but it makes the compilation recursive. Don't do that.

        //Constructors
        //----------------------------------
        Registry()
        {
            Registry("null", "null");
        }

        Registry(string name = "null", string desc = "null")
        {
            id = registryIDManager->generateID();
            registrar[id] = this;
            registryName = name;
            registryDescription=desc;
        }

        Registry(Registry<Key, Content> &copy)
        {
            id = copy.getID();
            registryName = copy.getName();
            registryDescription = copy.getDescription();
            delete thisRegistry;
            thisRegistry = new unordered_map<Key, Content, std::hash<Key>>();
            thisRegistry = copy.getThisRegistry();
        }

        Registry(const Registry&) = default;
        Registry& operator=(const Registry&) = default;

        ~Registry()
        {
            registrar[id] = nullptr;
            registryIDManager->setIDState(id, false);
        }

        //Registry Access
        //----------------------------------
        unordered_map<Key, Content>* getThisRegistry()
        {
            return thisRegistry;
        }
        void setThisRegistry(unordered_map<Key, Content> newBacking)
        {
            *thisRegistry = newBacking;
        }
        void removeItem(Key key)
        {
            thisRegistry->erase(thisRegistry.find(key));
        }
        Content getItem(Key key)
        {
            return (*thisRegistry)[key];
        }
        void setItem(Key key, Content content)
        {
            (*thisRegistry)[key]=content;
        }

        //Registrar Access
        //----------------------------------
        Registry* getRegistry(int index)
        {
            return registrar[index];
        }
        Registry* getRegistry(string name)
        {
            for(auto i = registrar.begin(); i<registrar.end(); i++)
            {
                if(i!=nullptr && i->getName()==name)
                {
                    return i;
                }
            }
        }

        //Attribute Access
        //----------------------------------
        int getID()
        {
            return id;
        }
        string getName()
        {
            return registryName;
        }
        string getDescription()
        {
            return registryDescription;
        }
        void setID(int id) 
        {
            this->id = id; 
        }
        void setName(string nName) 
        {
            this->registryName = nName;
        }
        void setDescription(string nDesc) 
        {
            this->registryDescription = nDesc;
        }



};


#endif