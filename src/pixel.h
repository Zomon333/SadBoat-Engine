#ifndef P_H
#define P_H

using namespace std;
#include "color.h"

class pixel
{
    private:
        colorClass containedcolorClass;
        float X_POS;
        float Y_POS;

        float X_RES;
        float Y_RES;

        float WIDTH;
        float HEIGHT;
    
    public:
        //Constructors
        pixel()
        {
            WIDTH = 1.0f / 1920.0f;
            HEIGHT = 1.0f / 1080.0f;

            X_POS = 0.0f;
            Y_POS = 0.0f;

            this->containedcolorClass =  colorClass("00000000");
        }

        pixel(pair<float, float> coords, pair<float, float> maxRes, colorClass containedcolorClass)
        {
            WIDTH = 1.0f / maxRes.first;
            HEIGHT = 1.0f / maxRes.second;

            X_POS = coords.first;
            Y_POS = coords.second;

            this->containedcolorClass=  containedcolorClass;
        }

        //colorClassmutators
        colorClass getPixel() { return containedcolorClass; }
        void setPixel(colorClass toSet) { containedcolorClass = toSet; }

        //Position mutators
        void movepixel(pair<float, float> destination) { X_POS = destination.first; Y_POS = destination.second; }
        void offsetPixel(pair<float, float> offset) { X_POS += offset.first; Y_POS += offset.second; }
        pair<float, float> getPosition() { return pair<float, float>(X_POS, Y_POS); }

        //Resolution mutators
        pair<float, float> getRes() { return pair<float, float>(X_RES, Y_RES); }
        void setRes(pair<float, float> newRes) { X_RES = newRes.first; Y_RES = newRes.second; }

        //OpenGL Helper Functions
        pair<float, float> getBounds(int index)
        {
            //float x = (X_POS / ( X_RES / 2.0f)) - 1;
            //float y = (Y_POS / ( Y_RES / 2.0f)) - 1;

            float x = -1 + (WIDTH * X_POS * 2);
            float y = -1 + (HEIGHT * Y_POS * 2);

            if(index==1)
            {
                x+=WIDTH;
            }
            if(index==2)
            {
                x+=WIDTH;
                y+=HEIGHT;
            }
            if(index==3)
            {
                y+=HEIGHT;
            }

            return pair<float, float>(x, y);
        }
};

#endif