//
// Created by acheung on 10/22/2022.
//

#ifndef HOVERGAME_TERRAINEDITOR_H
#define HOVERGAME_TERRAINEDITOR_H


#include "Includes.h"

class TerrainEditor {
public:
    vector<Vec2> points;

    TerrainEditor(Vec2 p1, Vec2 p2);
    void AddPoint(Vec2 point);
    void DebugRender(const Camera2D& camera);
private:
    int FindNearestIndex(Vec2 point);
    static float PointLineDistance(Vec2 point, Vec2 lineP1, Vec2 lineP2);
};


#endif //HOVERGAME_TERRAINEDITOR_H
