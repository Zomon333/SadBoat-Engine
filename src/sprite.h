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