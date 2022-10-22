//
// Created by staub on 10/22/2022.
//

#include "PerlinNoise.h"

void PerlinNoise::Render(Vec2 cameraOffset, float screenWidth, float screenHeight, int pixelSize) {
    int parallaxModifierX = 2;
    int parallaxModifierY = 10;
    int perlinStrength = 25;
    float perlinModifier = 9.0f;

    float pixelsF = (float) pixelSize;
    int halfPixel = pixelSize / 2;

    int xOffset = (cameraOffset.x / pixelSize) / parallaxModifierX;
    int yOffset = (cameraOffset.y / pixelSize) / parallaxModifierY;
    int xPixelOffset = (((int) cameraOffset.x / pixelSize) % parallaxModifierX) * ((float) pixelSize / parallaxModifierX);
    int yPixelOffset = (((int) cameraOffset.y / pixelSize) % parallaxModifierY) * ((float) pixelSize / parallaxModifierY);

    int pixelsX = std::ceil(screenWidth / pixelSize);
    int pixelsY = std::ceil(screenHeight / pixelSize);

    for (int x = 0; x < pixelsX; x++) {
        for (int y = 0; y < pixelsY; y++) {
            float modifiedX = (perlinModifier * ((float) (x + xOffset))) / (float) pixelsX;
            float modifiedY = (perlinModifier * ((float) (y + yOffset))) / (float) pixelsY;
            float modifierF = PerlinNoise::perlin(modifiedX, modifiedY);
            int modifierI = (int) (modifierF * perlinStrength);
            modifierI = std::min(modifierI, 45);
            DrawRectangle(pixelSize * x - (3 * halfPixel) - xPixelOffset,pixelSize * y - (3 * halfPixel) - yPixelOffset,
                          pixelSize + 100, pixelSize + 100, RayColor(200 + modifierI, 0, 200 + modifierI));
        }
    }
}