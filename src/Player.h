#ifndef HOVERGAME_PLAYER_H
#define HOVERGAME_PLAYER_H

#include <Includes.h>
#include "resources/Paths.h"
#include "TerrainSegment.h"

class Player {
public:
    static constexpr float GRAVITY = 500.0f;

    Vec2 pos;
    Vec2 dimens;
    float angle = 0.0f;
//    float deltaTime = 1.0f;

    Vec2 vel;
    Vec2 accel;
    float angularVel;
    float angularAccel;

    Vec2 force;
    float mass = 1.0f;


    explicit Player(Vec2 pos) : pos(pos) {
        dimens = Vec2((float) texture.width, (float) texture.height) * scale;
    }

    void Update(float deltaTime, const vector<TerrainSegment>& terrainSegments);

    void ApplyForce(Vec2 force, Vec2 point, float deltaTime);

    void Render();

    vector<Vec2> Polygon(Vec2 offset = {0, 0}, float angleOffset = 0);

    void MoveAndRotate(Vec2 diff, float angleDiff, const vector<TerrainSegment>& terrainSegments);

    [[nodiscard]] Vec2 PlayerToWorldPos(Vec2 playerPos) const;

private:
    RayTexture texture = {Paths::Image("HoverCraft")};
    float scale = 25.0f;

    void SimulateBoosters(const vector<TerrainSegment>& terrainSegments, float deltaTime);
    [[nodiscard]] optional<float> BoosterRayCastDist(Vec2 playerPoint, float dir, float maxLen, const vector<TerrainSegment>& terrainSegments) const;
};


#endif //HOVERGAME_PLAYER_H
