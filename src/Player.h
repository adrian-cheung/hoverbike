#ifndef HOVERGAME_PLAYER_H
#define HOVERGAME_PLAYER_H

#include <Includes.h>
#include "resources/Paths.h"
#include "TerrainSegment.h"

class Player {
public:
    Vec2 pos;
    Vec2 dimens;
    float angle = 0.0f;
//    float deltaTime = 1.0f;

    Vec2 vel;
    Vec2 accel;
    float angularVel;
    float angularAccel;

    Vec2 force;
    Vec2 forceLeft;
    Vec2 forceRight;
    float mass = 1.0f;


    explicit Player(Vec2 pos) : pos(pos) {
        dimens = Vec2((float) texture.width, (float) texture.height) * scale;
    }

    void Update(float deltaTime, const vector<TerrainSegment>& terrainSegments);

    void ApplyForce(Vec2 force, Vec2 point, float deltaTime);

    void ApplyForceCenter(Vec2 force);

    void Render();

    vector<Vec2> Polygon();

private:
    RayTexture texture = {Paths::Image("HoverCraft")};
    float scale = 25.0f;

    void SimulateBoosters(const vector<TerrainSegment>& terrainSegments);
    [[nodiscard]] optional<float> BoosterRayCastDist(Vec2 playerPoint, float dir, float maxLen, const vector<TerrainSegment>& terrainSegments) const;
    Vec2 PlayerToWorldPos(Vec2 playerPos) const;
};


#endif //HOVERGAME_PLAYER_H
