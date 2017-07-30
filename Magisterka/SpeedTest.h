#pragma once
#include <Windows.h>

class SpeedTester
{
    LARGE_INTEGER mStart;
    LARGE_INTEGER mStop;

public:
    SpeedTester() :
        mStart(),
        mStop()
    {}

    void start() { QueryPerformanceCounter(&mStart); }
    void stop() { QueryPerformanceCounter(&mStop); }

    unsigned long long getCzas() const 
    {
        LARGE_INTEGER czestotliwosc = {};
        QueryPerformanceFrequency(&czestotliwosc);

        return (mStop.QuadPart - mStart.QuadPart)/czestotliwosc.QuadPart; 
    }
};