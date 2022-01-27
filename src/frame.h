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
#ifndef F_H
#define F_H

using namespace std;

class frame
{
    private:
        int X_RES;
        int Y_RES;
        int RESCOUNT;

        pixel* frameBuffer;

    public:
        frame()
        {
            X_RES = 1920;
            Y_RES = 1080;
            RESCOUNT = X_RES * Y_RES;
            frameBuffer = new pixel[RESCOUNT];
        }

        frame(int X_RES, int Y_RES)
        {
            this->X_RES = X_RES;
            this->Y_RES = Y_RES;
            RESCOUNT = X_RES * Y_RES;
            frameBuffer = new pixel[RESCOUNT];
        }

        
        pixel getPixel(int x, int y)
        {
            int offset = X_RES * (y-1);
            return frameBuffer[(x+offset)];
        }

        pixel getPixel(pair<int, int> coord)
        {
            return getPixel(coord.first, coord.second);
        }

        void setPixel(int x, int y, pixel data)
        {
            int offset = X_RES * (y-1);
            frameBuffer[(x+offset)] = data;
        }

        void setPixel(pair<int, int> coord, pixel data)
        {
            setPixel(coord.first, coord.second, data);
        }

        
};

#endif