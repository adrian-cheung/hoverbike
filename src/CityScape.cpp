//
// Created by staub on 10/22/2022.
//

#include "CityScape.h"

void CityScape::Render(Vec2 cameraOffset, float screenWidth, float screenHeight, int buildingWidth) {
    int xOffset = (int) (cameraOffset.x / buildingWidth) / xParralax;
    int xPixelOffset = std::fmod(cameraOffset.x, buildingWidth * xParralax) / xParralax;
    int yPixelOffset = (cameraOffset.y) / yParallax;

    int numBuildings = std::ceil(screenWidth / buildingWidth);
    int halfBuilding = buildingWidth / 2;
    for (int x = -1; x < numBuildings + 1; x++) {
        int height = CityScape::GetHeight(x + xOffset);
        int topY = (screenHeight / 2) + height;
        DrawRectangle(x * buildingWidth - xPixelOffset, screenHeight - topY - yPixelOffset, buildingWidth, topY + yPixelOffset, RayColor(0, 0, 0, 128));
    }
}