//
// Created by Tobiathan on 10/22/22.
//

#ifndef HOVERGAME_UTIL_H
#define HOVERGAME_UTIL_H


#include "Includes.h"

class Util {
public:
    static Vec2 Polar(float radAngle, float notSoRadMagnitude) {
        return Vec2(cos(radAngle), sin(radAngle)) * notSoRadMagnitude;
    }
};


#endif //HOVERGAME_UTIL_H
