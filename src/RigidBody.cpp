//
// Created by Tobiathan on 10/23/22.
//

#include "RigidBody.h"
#include "Collision.h"

void RigidBody::MoveAndRotate(Vec2 diff, float angleDiff, const vector<TerrainSegment> &terrainSegments) {
    const auto CollidesWithDiff = [&](Vec2 offset){
        return Collision::PolygonTerrain(Polygon(offset), terrainSegments);
    };

    if (!Collision::PolygonTerrain(Polygon(diff, angleDiff), terrainSegments)) {
        pos += diff;
        angle += angleDiff;
        return;
    }

    const float STEP_LEN = 0.5f;

    float diffX = 0;
    for (int i = 0; i < (int) (diff.x / STEP_LEN); i++) {
        diffX += STEP_LEN;
        if (CollidesWithDiff({diffX, 0})) {
            diffX -= STEP_LEN;
            vel.x *= bounceConstant;
            break;
        }
    }

    float diffY = 0;
    for (int i = 0; i < (int) (diff.y / STEP_LEN); i++) {
        diffY += STEP_LEN;
        if (CollidesWithDiff({diffX, diffY})) {
            diffY -= STEP_LEN;
            vel.y *= bounceConstant;
            break;
        }
    }

    const auto CollidesAtRotation = [&](float angleOffset){
        return Collision::PolygonTerrain(Polygon({diffX, diffY}, angleOffset), terrainSegments);
    };

    pos += {diffX, diffY};

    if (!CollidesAtRotation(angleDiff)) {
        angle += angleDiff;
        return;
    }

    float currAngleDiff = 0.0f;
    const float ANGLE_STEP = 0.5f;
    int angleDiffSign = std::signbit(angleDiff);
    for (int i = 0; i < (int) (std::abs(angleDiff) / ANGLE_STEP); i++) {
        currAngleDiff += ANGLE_STEP * (float) angleDiffSign;
        if (CollidesAtRotation(currAngleDiff)) {
            currAngleDiff -= ANGLE_STEP * (float) angleDiffSign;
            angularVel *= bounceConstant;
            break;
        }
    }
    angle += currAngleDiff;
}

vector<Vec2> RigidBody::Polygon(Vec2 offset, float angleOffset) {
    vector<Vec2> unTranslatedPoints = {
            {-dimens.x, -dimens.y},
            {dimens.x, -dimens.y},
            {dimens.x, dimens.y},
            {-dimens.x, dimens.y}
    };

    return unTranslatedPoints | MAP({ return pos + offset + (it * 0.5f).Rotate(angle + angleOffset); }) | to_vector{};
}
