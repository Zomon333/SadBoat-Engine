using namespace std;
#include <algorithm>


class framebuffer
{
    private:
        vector<pair<int, frame>> frameLayers;

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
        framebuffer(int X_RES, int Y_RES)
        {
            frame blackground(X_RES, Y_RES);
            for(int y = 0; y<Y_RES; y++)
            {
                for(int x = 0; x<X_RES; x++)
                {
                    blackground.setPixel(pair<int, int>(x, y), pixel("00000000"));
                }
            }

            frameLayers.push_back(pair<int, frame>(-INFINITY, blackground));
        }

        void sortFrames()
        {
            insertionSort(frameLayers, 0, frameLayers.size());
        }

        void pushFrame(frame toAdd);
        void addFrameAt(frame toAdd, int index);
        
        frame popFrame();
        frame removeFrameAt(frame toAdd, int index);

        frame getFrame(int index);
        void setFrame(frame toSet, int index);

        frame renderFinal();



};