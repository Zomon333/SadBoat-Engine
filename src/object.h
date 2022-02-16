#ifndef OBJ_H
#define OBJ_H

//#include <idman.h>
#include <unordered_map>

using namespace std;

class Object {
    private:
        float x;
        float y;

        float dX;
        float dY;

        //Needs hitbox array
        //

    public:

        int id;

        static IDMan objectIDManager;
        static std::unordered_map<int, Object*> objectsRegistry;

        Object()
        {
            id = objectIDManager.generateID();
            objectsRegistry[id]=this;

            x=0;
            y=0;
            dX=0;
            dY=0;
        }

        Object(float x, float y, float dX=0, float dY=0)
        {
            Object();
            this->x = x;
            this->y = y;
            this->dX = dX;
            this->dY = dY;
        }

        ~Object()
        {
            objectIDManager.setIDState(id, false);
            objectsRegistry[id] = nullptr;
        }


};

#endif