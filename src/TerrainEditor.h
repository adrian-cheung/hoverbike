//
// Created by acheung on 10/22/2022.
//

#ifndef HOVERGAME_TERRAINEDITOR_H
#define HOVERGAME_TERRAINEDITOR_H


#include "Includes.h"
#include "TerrainSegment.h"
#include <unordered_set>

class TerrainEditor {
public:
    vector<Vec2> points;
    std::unordered_set<int> selectedPoints;
    vector<Vec2> preTransformPoints;
    Vec2 selectStart;
    bool grabbing = false;
    Vec2 grabStart;
    bool selecting = false;

    TerrainEditor(Vec2 p1, Vec2 p2);

    void Update(vector<TerrainSegment>& terrainSegments);
    void AddPoint(Vec2 point);
    void MovePoint();
    void DebugRender();
private:
    int FindNearestPointIndex(Vec2 point);
    int FindNearestLineIndex(Vec2 point);
    static float PointLineDistance(Vec2 point, Vec2 lineP1, Vec2 lineP2);
    vector<int> PointsBetween(Vec2 selectStart, Vec2 selectEnd);
    void RebuildTerrain(vector<TerrainSegment>& terrainSegments);

    static RectF GimmeRect(Vec2 selectStart, Vec2 selectEnd);
};


#endif //HOVERGAME_TERRAINEDITOR_H
