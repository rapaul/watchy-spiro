#ifndef WATCHY_SPIRO_H
#define WATCHY_SPIRO_H

#include <Watchy.h>

class WatchySpiro : public Watchy{
    using Watchy::Watchy;
    public:
        void drawWatchFace();
        void syncTime();
        void drawBackground();
        void drawSpiral(int strokeSize, int maxRadius, int maxRatio);
        void drawTime();
        void drawDate();
        void drawSteps();
        void drawBattery();
};

#endif