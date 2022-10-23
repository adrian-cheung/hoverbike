//
// Created by staub on 10/22/2022.
//

#ifndef HOVERGAME_CITYSCAPE_H
#define HOVERGAME_CITYSCAPE_H

#include "Includes.h"

class CityScape {
public:
    int xParralax;
    int yParallax;
    int yOffset;
    RayColor color;
    CityScape(int xParralax, int yParralax, int yOffset, RayColor color)
          : xParralax(xParralax), yParallax(yParralax), yOffset(yOffset), color(color) {
        seed = rand();
    }
    void Render(Vec2 cameraOffset, float screenWidth, float screenHeight, int buildingWidth);

private:
    int seed = 0;
    int GetHeight(int x) {
        srand(seed);
        for (int i = 0; i < abs(x * xParralax); i++) {
            rand();
        }
        return (rand() % 150) - yOffset;
    }
};


#endif //HOVERGAME_CITYSCAPE_H
