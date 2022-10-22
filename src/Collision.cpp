//
// Created by Tobiathan on 10/22/22.
//

#include "Collision.h"

std::optional<Vec2> Collision::LineLine(Vec2 l1P1, Vec2 l1P2, Vec2 l2P1, Vec2 l2P2) {
    Vec2 output;
    bool collided = CheckCollisionLines(l1P1, l1P2, l2P1, l2P2, &output);
    return collided ? optional<Vec2>{ output } : std::nullopt;
}

std::optional<Vec2> Collision::LineTerrainNearest(Vec2 origin, Vec2 rayEnd, const vector<TerrainSegment> &terrainSegments) {
    std::optional<Vec2> nearest;
    for (const auto & [tP1, tP2]: terrainSegments) {
        auto collision = LineLine(origin, rayEnd, tP1, tP2);
        if (collision.has_value() && (!nearest.has_value() || collision->Distance(origin) < nearest->Distance(origin))) {
            nearest = collision;
        }
    }
    return nearest;
}

bool Collision::PolygonTerrain(const vector<Vec2> &points, const vector<TerrainSegment> &terrainSegments) {
    for (int i = 0; i < points.size(); i++) {
        // inefficient, but doesn't matter :)
        if (auto collision = LineTerrainNearest(points[i], points[(i + 1) % points.size()], terrainSegments)) {
            return true;
        }
    }
    return false;
}
