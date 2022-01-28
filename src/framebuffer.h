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
#ifndef FB_H
#define FB_H

using namespace std;

class framebuffer
{
    private:
        vector<pair<int, frame>> frameLayers;

        //I know this insertion sort code works, I just don't know if it works in this context.
        void insertionSort(vector<pair<int, frame>> array, const int start, const int end) {

            if(start>end)
            {
                return;
            }

            int length = (end - start)+1;
            if(length>1)
            {
                int leastKey=start;
                for(int i=end;i>(start-1);i--)
                {
                   
                    if(array.at(i).first < array.at(leastKey).first)
                    {
                        leastKey= i;
                    }
                }
                swap(array[leastKey],array[start]);
                insertionSort(array, (start+1), end);
            }
        }

    public:
        framebuffer()
        {
            framebuffer(1920, 1080);
        }

        framebuffer(int X_RES, int Y_RES)
        {
            frame blackground(X_RES, Y_RES);
            for(int y = 0; y<Y_RES; y++)
            {
                for(int x = 0; x<X_RES; x++)
                {
                    blackground.setcolorClass(pair<int, int>(x, y), colorClass("00000000"));
                }
            }

            frameLayers.push_back(pair<int, frame>(-15000, blackground));
        }

        void sortFrames()
        {
            insertionSort(frameLayers, 0, frameLayers.size());
        }

        void addSprite(sprite toAdd, pair<int, int> coords);

        void pushFrame(frame toAdd) { sortFrames(); frameLayers.push_back(pair<int, frame>(frameLayers[0].first-1, toAdd)); }
        void addFrameAt(frame toAdd, int index) { frameLayers.push_back(pair<int, frame>(index, toAdd)); sortFrames(); }
        
        frame popFrame() { sortFrames(); auto toReturn = frameLayers.back(); frameLayers.pop_back(); return toReturn.second; }
        void removeFrameAt(int index) { sortFrames(); frameLayers.erase(frameLayers.begin()+index); }

        frame getFrame(int index) { sortFrames(); return frameLayers.at(index).second;}
        void setFrame(frame toSet, int index) { sortFrames(); frameLayers[index] = pair<int, frame>(index, toSet); }

        frame getFinal()
        {
            return frame();
        }



};

#endif