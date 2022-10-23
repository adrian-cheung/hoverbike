//
// Created by Tobiathan on 10/22/22.
//

#ifndef HOVERGAME_UTIL_H
#define HOVERGAME_UTIL_H

#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include <cstring>

#include "Includes.h"

class Util {
public:
    static void Setup(Camera2D& camera) {
        susCameraPtr = &camera;
    }

    static Vec2 Polar(float radAngle, float notSoRadMagnitude) {
        return Vec2(cos(radAngle), sin(radAngle)) * notSoRadMagnitude;
    }

    static Vec2 MousePosWorld() {
        return GetScreenToWorld2D(GetMousePosition(), *susCameraPtr);
    }

    static std::string Fmt(const char* fmt, ...)
    {
        char buf[1024];
        sprintf(buf, "");

        va_list ap;
        va_start(ap, fmt);
        vsprintf(buf + strlen(buf), fmt, ap);
        return buf;
    }

private:
    inline static Camera2D* susCameraPtr = nullptr;
};


#endif //HOVERGAME_UTIL_H
