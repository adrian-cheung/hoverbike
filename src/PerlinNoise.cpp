//
// Created by staub on 10/22/2022.
//

#include "PerlinNoise.h"

void PerlinNoise::Render(Vec2 cameraOffset, float screenWidth, float screenHeight, int pixelSize) {
    int parallaxModifierX = 10;
    int parallaxModifierY = 10;
    int perlinStrength = 25;
    float perlinModifier = 9.0f;

    float pixelsF = (float) pixelSize;
    int halfPixel = pixelSize / 2;

    int xOffset = (cameraOffset.x / pixelSize) / parallaxModifierX;
    int yOffset = (cameraOffset.y / pixelSize) / parallaxModifierY;
    int xPixelOffset = std::fmod(cameraOffset.x, pixelSize * parallaxModifierX) / parallaxModifierX;
    int yPixelOffset = std::fmod(cameraOffset.y, pixelSize * parallaxModifierY) / parallaxModifierY;

    int pixelsX = std::ceil(screenWidth / pixelSize);
    int pixelsY = std::ceil(screenHeight / pixelSize);

    for (int x = -1; x < pixelsX + 1; x++) {
        for (int y = -1; y < pixelsY + 1; y++) {
            float modifiedX = (perlinModifier * ((float) (x + xOffset))) / (float) pixelsX;
            float modifiedY = (perlinModifier * ((float) (y + yOffset))) / (float) pixelsY;
            float modifierF = PerlinNoise::perlin(modifiedX, modifiedY);
            int modifierI = (int) (modifierF * perlinStrength);
            modifierI = std::min(modifierI, 45);
            DrawRectangle(pixelSize * x - xPixelOffset,pixelSize * y - yPixelOffset,
                          pixelSize, pixelSize, RayColor(200 + modifierI, 0, 200 + modifierI));
        }
    }
}