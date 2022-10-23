//
// Created by Tobiathan on 10/23/22.
//

#include "RagDoll.h"
#include "Player.h"

void RagDoll::Render() {
    for (RagDollSegment& segment : segments) {

        Vec2 segmentDimens = Vec2(segment.texture->width, segment.texture->height) * scale;
        Vec2 segmentPos = pos + (dimens * (segment.pivotNormBody - Vec2(0.5f, 0.5f))).Rotate(angle);
        segment.texture->Draw(
                RectF {0, 0, (float) segment.texture->width, (float) segment.texture->height},
                {segmentPos.x, segmentPos.y, (float) segmentDimens.x, (float) segmentDimens.y},
                segmentDimens * segment.pivotNorm,
                (angle + segment.angle) * RAD2DEG,
                WHITE
        );
    }

    bodyTexture.Draw(
            RectF {0, 0, (float) bodyTexture.width, (float) bodyTexture.height},
            {pos.x, pos.y, (float) dimens.x, (float) dimens.y},
            {(float) dimens.x / 2.0f, (float) dimens.y / 2.0f},
            angle * RAD2DEG,
            WHITE
    );
}

void RagDoll::Update(float deltaTime, const vector<TerrainSegment>& terrainSegments) {
    accel = Vec2 {};
    angularAccel = 0.0f;

    accel.y += Player::GRAVITY;
    accel.x -= vel.x * 0.7f;
    vel += accel * deltaTime;
    MoveAndRotate(vel * deltaTime, angularVel * deltaTime, terrainSegments);

    for (RagDollSegment& segment : segments) {
        segment.angle += segment.angularVel * deltaTime;
        segment.angularVel *= 0.9f;
//        const float MAX_ANGLE = PI * 0.45f;
//        if (abs(angle) > MAX_ANGLE) {
//            angle = std::signbit(angle) * MAX_ANGLE;
//            angularVel *= -0.5f;
//        }
    }
}
