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

        int id;

    public:
        static IDMan spriteIDs;
        static std::unordered_map<int, sprite*> spriteRegistry;

        sprite()
        {
            id = spriteIDs.generateID();
            spriteRegistry[id] = this;

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
                    image[x][y] = pixel(pair<float, float>( ((float)(x)), ((float)(y))  ), pair<float, float>( ((float)(stoi(config["X_RES"].second.second))), ((float)(stoi(config["Y_RES"].second.second))) ), colorClass("00000000")); //pixel("00000000");
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
            /*
            TODO:
                Implement this once graphics are working.

            */

            //Get the sprite from the globals
            /*if(textures.find(spriteName) != textures.end())
            {
                image = textures[spriteName];
                width = image.size();
                height = image[0].size();
            }
            else
            {
                sprite();
            }*/
            sprite();
        }

        ~sprite()
        {
            spriteIDs.setIDState(id, false);
            spriteRegistry[id] = nullptr;
        }

};

#endif