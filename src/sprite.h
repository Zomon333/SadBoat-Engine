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

#ifndef S_H
#define S_H

class sprite
{
    private:
        int width;
        int height;
        vector<vector<pixel>> image;

    public:
        sprite()
        {
            width = 1;
            height = 1;
            
            image.resize(width);
            for(int i = 0; i < width; i++)
            {
                image[i].resize(height);
            }

            for(int x = 0; x<width; x++)
            {
                for(int y = 0; y<height; y++)
                {
                    image[x][y] = pixel("00000000");
                }
            }

        }
        
        sprite(char* fileName)
        {
            sprite();
            //Load the file
        }

        sprite(string spriteName)
        {
            //Get the sprite from the globals
            if(textures.find(spriteName) != textures.end())
            {
                image = textures[spriteName];
                width = image.size();
                height = image[0].size();
            }
            else
            {
                sprite();
            }
        }


};

#endif