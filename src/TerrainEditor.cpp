//
// Created by acheung on 10/22/2022.
//

#include <cfloat>
#include "TerrainEditor.h"

TerrainEditor::TerrainEditor(Vec2 p1, Vec2 p2) {
    points.push_back(p1);
    points.push_back(p2);
}

void TerrainEditor::AddPoint(Vec2 point) {
    int closestIndex = getClosestIndex(point);
    points.insert(points.begin() + closestIndex + 1, point);
}

int TerrainEditor::getClosestIndex(Vec2 point) {
    int closestIndex = 0;
    float closestDistance = FLT_MAX;
    std::cout << "==============================" << std::endl;
    std::cout << "POINT: " << point.x << ", " << point.y << std::endl;
    for (int i = 0; i < points.size() - 1; i++) {
        float distance = getDistance(point, points[i], points[i+1]);

        std::cout << "distance: " << distance << std::endl;
        if (distance < closestDistance) {
            closestDistance = distance;
            closestIndex = i;
            std::cout << "closest index: " << closestIndex << " - " << points[closestIndex].x << ", " << points[closestIndex].y << std::endl;
        }
    }
    return closestIndex;
}

float TerrainEditor::getDistance(Vec2 point, Vec2 lineP1, Vec2 lineP2) {
    return abs((lineP2.x - lineP1.x) * (lineP1.y - point.y) - (lineP1.x - point.x) * (lineP2.y - lineP1.y))
            / sqrt(pow(lineP2.x - lineP1.x, 2) + pow(lineP2.y - lineP1.y, 2));
}





