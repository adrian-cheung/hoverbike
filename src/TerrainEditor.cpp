//
// Created by acheung on 10/22/2022.
//

#include <cfloat>
#include "TerrainEditor.h"
#include "Util.h"

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
    float lengthSquared = pow((lineP2 - lineP1).Length(), 2);
    if (lengthSquared == 0.0f) return (point - lineP1).Length();
    float parameterizedT = fmax(0, fmin(1, (point - lineP1).DotProduct(lineP2 - lineP1) / lengthSquared));
    Vec2 projection = lineP1 + (lineP2 - lineP1) * parameterizedT;
    return (point - projection).Length();
}

void TerrainEditor::DebugRender() {
    Vec2 mousePos = Util::MousePosWorld();

    int nearestIndex = FindNearestIndex(mousePos);
    DrawLineEx(points[nearestIndex], points[nearestIndex + 1], 10.0f, RED);
}





