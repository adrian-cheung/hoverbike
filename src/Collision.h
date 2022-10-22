//
// Created by Tobiathan on 10/22/22.
//

#ifndef HOVERGAME_COLLISION_H
#define HOVERGAME_COLLISION_H

#include <Includes.h>
#include "TerrainSegment.h"

class Collision {
public:
    static std::optional<Vec2> LineLine(Vec2 l1P1, Vec2 l1P2, Vec2 l2P1, Vec2 l2P2);
    static std::optional<Vec2> LineTerrainNearest(Vec2 p1, Vec2 p2, const vector<TerrainSegment>& terrainSegments);
};


#endif //HOVERGAME_COLLISION_H
