#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include "pixel.h"
#include "frame.h"
#include "framebuffer.h"

using namespace std;

int main()
{
    cout<<"Hello world!";
    const int X_RES = 1920;
    const int Y_RES = 1080;
    const int RESCOUNT = X_RES * Y_RES;

    frame* testFrame;
    testFrame = new frame(X_RES, Y_RES);





    cout<<"Goodbye world!";
    return 0;
}