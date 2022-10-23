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
    int closestIndex = FindNearestLineIndex(point);
    points.insert(points.begin() + closestIndex + 1, point);
}



int TerrainEditor::FindNearestPointIndex(Vec2 point) {
    int closestIndex = 0;
    float closestDistance = FLT_MAX;
    for (int i = 0; i < points.size(); i++) {
        float distance = (point - points[i]).Length();
        if (distance < closestDistance) {
            closestDistance = distance;
            closestIndex = i;
        }
    }
    return closestIndex;
}

int TerrainEditor::FindNearestLineIndex(Vec2 point) {
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
    if (!DEV_MODE) {return;}
    Vec2 mousePos = Util::MousePosWorld();

    int nearestIndex = FindNearestLineIndex(mousePos);
    DrawLineEx(points[nearestIndex], points[nearestIndex + 1], 25.0f, RED);
    DrawLineEx(points[nearestIndex], mousePos, 1.0f, ORANGE);
    DrawLineEx(mousePos, points[nearestIndex + 1], 1.0f, ORANGE);

    if (selecting) {
        GimmeRect(selectStart, Util::MousePosWorld()).Draw({255, 255, 255, 60});
    }

    for (int i = 0; i < points.size(); i++) {
        DrawCircleV(points[i], 20, selectedPoints.contains(i) ? GREEN : WHITE);
    }
}

vector<int> TerrainEditor::PointsBetween(Vec2 selectStart, Vec2 selectEnd) {
    vector<int> indices = {};

    RectF rect = GimmeRect(selectStart, selectEnd);
    for (int i = 0; i < points.size(); i++) {
        if (CheckCollisionPointRec(points[i], rect)) {
            indices.push_back(i);
        }
    }

    return indices;
}

void TerrainEditor::Update(vector<TerrainSegment>& terrainSegments, std::unordered_set<int>& gapIndices) {
    bool leftPressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    bool shiftHeld = IsKeyDown(KEY_LEFT_SHIFT);
    Vec2 mousePos = Util::MousePosWorld();

    if ((IsKeyPressed(KEY_DELETE) || IsKeyPressed(KEY_BACKSPACE)) && !selectedPoints.empty()) {
        for (int i = points.size() - 1; i >= 0; i--) {
            if (selectedPoints.contains(i)) {
                points.erase(points.begin() + i);
            }
        }
        selectedPoints.clear();
        grabbing = false;
        RebuildTerrain(terrainSegments, gapIndices);
        return;
    }

    if (IsKeyPressed(KEY_G)) {
        grabbing = true;
        preTransformPoints = points;
        grabStart = mousePos;
    }

    // grab :)
    if (grabbing) {
        Vec2 diff = mousePos - grabStart;
        for (int i = 0; i < points.size(); i++) {
            if (selectedPoints.contains(i)) {
                points[i] = preTransformPoints[i] + diff;
            }
        }
        RebuildTerrain(terrainSegments, gapIndices);

        if (leftPressed || IsKeyPressed(KEY_SPACE)) {
            grabbing = false;
        }
        return;
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
        selectStart = mousePos;
        selecting = true;
    }

    if (selecting && IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) {
        if (!IsKeyDown(KEY_LEFT_SHIFT)) {
            selectedPoints.clear();
        }
        for (int index : PointsBetween(selectStart, mousePos)) {
            selectedPoints.insert(index);
        }
        selecting = false;
        return;
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        int index = FindNearestLineIndex(mousePos);
        AddPoint(mousePos);

        if (!shiftHeld) {
            selectedPoints.clear();
        }
        selectedPoints.insert(index + 1);
        RebuildTerrain(terrainSegments, gapIndices);
    }

    if (IsKeyPressed(KEY_K)) {
        int index = FindNearestLineIndex(mousePos);

        if (gapIndices.contains(index)) {
            gapIndices.erase(index);
        } else {
            gapIndices.insert(index);
        }

        RebuildTerrain(terrainSegments, gapIndices);
    }
}

void TerrainEditor::RebuildTerrain(vector<TerrainSegment> &terrainSegments, std::unordered_set<int>& gapIndices) {
    terrainSegments.clear();
    terrainSegments.reserve(std::max(0, (int) points.size() - 1));

    int batchStart = 0;
    for (int i = 0; i < points.size() - 1; i++) {
        if (gapIndices.contains(i)) {
            if (i != batchStart) {
                terrainSegments.push_back({ points[i], points[batchStart] });
            }

            batchStart = i + 1;
        } else {
            terrainSegments.push_back({ points[i], points[i + 1] });
        }
    }
    terrainSegments.push_back({points[points.size() - 1], points[batchStart]});
}

RectF TerrainEditor::GimmeRect(Vec2 selectStart, Vec2 selectEnd) {
    float minX = std::min(selectStart.x, selectEnd.x);
    float minY = std::min(selectStart.y, selectEnd.y);
    float maxX = std::max(selectStart.x, selectEnd.x);
    float maxY = std::max(selectStart.y, selectEnd.y);
    return { minX, minY, (maxX - minX), (maxY - minY) };
}





