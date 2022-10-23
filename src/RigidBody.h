//
// Created by Tobiathan on 10/23/22.
//

#ifndef HOVERGAME_RIGIDBODY_H
#define HOVERGAME_RIGIDBODY_H


#include "Includes.h"
#include "TerrainSegment.h"

class RigidBody {
public:
    Vec2 pos = {};
    Vec2 dimens;
    float angle = 0.0f;

    Vec2 vel = {};
    Vec2 accel = {};
    float angularVel = 0.0f;
    float angularAccel = 0.0f;
    float bounceConstant = -0.1f;

    virtual vector<Vec2> Polygon(Vec2 offset = {0, 0}, float angleOffset = 0);

    virtual void MoveAndRotate(Vec2 diff, float angleDiff, const vector<TerrainSegment>& terrainSegments);
};


#endif //HOVERGAME_RIGIDBODY_H
