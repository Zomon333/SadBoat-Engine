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