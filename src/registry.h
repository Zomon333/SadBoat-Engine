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
        inline static unordered_map<int, Registry*, std::hash<int>> registrar;

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