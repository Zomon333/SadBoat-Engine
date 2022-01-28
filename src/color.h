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
#ifndef CL_H
#define CL_H

class colorClass
{
private:
    int r;
    int g;
    int b;
    int a;

    int hexToInt(char hex)
    {
        if(hex=='0') return 0;
        if(hex=='1') return 1;
        if(hex=='2') return 2;
        if(hex=='3') return 3;
        if(hex=='4') return 4;
        if(hex=='5') return 5;
        if(hex=='6') return 6;
        if(hex=='7') return 7;
        if(hex=='8') return 8;
        if(hex=='9') return 9;
        if(hex=='A') return 10;
        if(hex=='B') return 11;
        if(hex=='C') return 12;
        if(hex=='D') return 13;
        if(hex=='E') return 14;
        if(hex=='F') return 15;
        return -1;
    }
public:


    colorClass()
    {
        r=0;
        g=0;
        b=0;
        a=0;
    }


    colorClass(const char hexcode[9])
    {
        r=hexToInt(hexcode[0]);
        r*=16;
        r+=hexcode[1];

        g=hexToInt(hexcode[2]);
        g*=16;
        g+=hexcode[3];

        b=hexToInt(hexcode[4]);
        b*=16;
        b+=hexcode[5];

        a=hexToInt(hexcode[6]);
        a*=16;
        a+=hexcode[7];
    }


    colorClass(int r, int g, int b)
    {
        this->r = r;
        this->g = g;
        this->b = b;
        a=0;
    }


    colorClass(int r, int g, int b, int a)
    {
        colorClass(r, g, b);
        this->a=a;
    }

    ~colorClass()
    {

    }

    bool operator==(colorClass rhs)
    {
        if(this->r == rhs.r && this->g == rhs.g && this->b == rhs.b && this->a == rhs.a)
        {
            return true;
        } 
        return false;
    }

    float getR()
    {
        return (r - abs( 255.0f - r )) / 255.0f;  
    }
    float getG()
    {
        return (g - abs( 255.0f - g )) / 255.0f;   
    }
    float getB()
    {
        return (b - abs( 255.0f - b )) / 255.0f;     
    }
    float getA()
    {
        return (a - abs( 255.0f - a )) / 255.0f;         
    }

};
#endif