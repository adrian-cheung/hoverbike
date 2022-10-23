//
// Created by acheung on 10/22/2022.
//

#include <cfloat>
#include "TerrainEditor.h"

TerrainEditor::TerrainEditor(Vec2 p1, Vec2 p2) {
    points = { p1, p2 };
}

void TerrainEditor::AddPoint(Vec2 point) {
    int closestIndex = FindNearestIndex(point);
    points.insert(points.begin() + closestIndex + 1, point);
}

int TerrainEditor::FindNearestIndex(Vec2 point) {
    int closestIndex = 0;
    float closestDistance = FLT_MAX;
    for (int i = 0; i < points.size() - 1; i++) {
        float distance = PointLineDistance(point, points[i], points[i + 1]);

        if (distance < closestDistance) {
            closestDistance = distance;
            closestIndex = i;
        }
    }
    return closestIndex;
}

float TerrainEditor::PointLineDistance(Vec2 point, Vec2 lineP1, Vec2 lineP2) {
    return (float) abs((lineP2.x - lineP1.x) * (lineP1.y - point.y) - (lineP1.x - point.x) * (lineP2.y - lineP1.y))
            / ((float) sqrt(pow(lineP2.x - lineP1.x, 2) + pow(lineP2.y - lineP1.y, 2)));
}

void TerrainEditor::DebugRender(const Camera2D& camera) {
    Vec2 mousePos = GetScreenToWorld2D(GetMousePosition(), camera);

    int nearestIndex = FindNearestIndex(mousePos);
    DrawLineEx(points[nearestIndex], points[nearestIndex + 1], 10.0f, RED);
}





