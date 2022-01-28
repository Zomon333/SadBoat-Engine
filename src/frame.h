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

        //pixel* frameBuffer;
        vector<vector<pixel>> frameBuffer;

    public:
        frame()
        {
            X_RES = 1920;
            Y_RES = 1080;
            RESCOUNT = X_RES * Y_RES;
            
            frameBuffer.resize(X_RES);
            for(int x = 0; x<X_RES; x++)
            {
                frameBuffer[x].resize(Y_RES);
                for( int y = 0; y<Y_RES; y++)
                {
                    frameBuffer[x][y] = pixel(pair<float, float>( ((float)(x)), ((float)(y)) ), pair<float, float>( ((float)(X_RES)), ((float)(Y_RES))), colorClass());
                }
            }

        }

        frame(int X_RES, int Y_RES)
        {
            this->X_RES = X_RES;
            this->Y_RES = Y_RES;
            RESCOUNT = X_RES * Y_RES;
            
            frameBuffer.resize(X_RES);
            for(int x = 0; x<X_RES; x++)
            {
                frameBuffer[x].resize(Y_RES);
                for( int y = 0; y<Y_RES; y++)
                {
                    frameBuffer[x][y] = pixel(pair<float, float>( ((float)(x)), ((float)(y)) ), pair<float, float>( ((float)(X_RES)), ((float)(Y_RES))), colorClass());
                }
            }

        }

        
        pixel getPixel(int x, int y)
        {
            x = x % X_RES;
            y = y % Y_RES;
            
            return frameBuffer[x][y];
        }

        pixel getPixel(pair<int, int> coord)
        {
            return getPixel(coord.first, coord.second);
        }

        void setcolorClass(int x, int y, colorClass data)
        {
            frameBuffer[x][y] = pixel(pair<float, float>((float)(x), (float)(y)), pair<float, float>( (((float)(X_RES))), (((float)(Y_RES))) ), data);
        }

        void setcolorClass(pair<int, int> coord, colorClass data)
        {
            setcolorClass(coord.first, coord.second, data);
        }

        
};

#endif