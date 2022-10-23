//
// Created by Tobiathan on 10/22/22.
//

#ifndef HOVERGAME_UTIL_H
#define HOVERGAME_UTIL_H


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

private:
    inline static Camera2D* susCameraPtr = nullptr;
};


#endif //HOVERGAME_UTIL_H
