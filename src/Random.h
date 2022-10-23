//
// Created by Tobiathan on 10/22/22.
//

#ifndef HOVERGAME_RANDOM_H
#define HOVERGAME_RANDOM_H

#include "Includes.h"
#include <cstdlib>

class Random {
public:

    static float RandAngle() {
        return Rand(PI * 2);
    }

    static float Rand(float max = 1.0f) {
        return (float) dist(gen) * max;
    }

    static float Rand(float min, float max) {
        return min + Rand(max - min);
    }

    static float RandPN(float maxPN) {
        return Rand(-maxPN, maxPN);
    }

    static int RandI(int min, int max) {
        return (int) Rand(min, max + 1);
    }

    static int RandI(int max) {
        return RandI(0, max);
    }
private:
    static inline std::mt19937 gen;
    static inline std::uniform_real_distribution<> dist = std::uniform_real_distribution<>(0, 1);
};


#endif //HOVERGAME_RANDOM_H
