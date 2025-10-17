#include "Engine/Utils/Clock.h"

double Clock::GetTimeSeconds() {
    LARGE_INTEGER frequency, currentTime;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&currentTime);
    return (double)(currentTime.QuadPart) / (double)(frequency.QuadPart);
}

double Clock::GetTimeMilliseconds() {
    return GetTimeSeconds() * 1000;
}