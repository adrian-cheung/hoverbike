#ifndef HOVERGAME_PLAYER_H
#define HOVERGAME_PLAYER_H

#include <Includes.h>
#include "resources/Paths.h"
#include "TerrainSegment.h"
#include "Particle.h"

struct PlayerUpdateInfo {
    float deltaTime;
    vector<TerrainSegment>& terrainSegments;
    vector<Particle>& particles;
};

class Player {
public:
    static constexpr float GRAVITY = 1000.0f;

    bool godModeEnabled = false;

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

    void Update(const PlayerUpdateInfo& params);

    void ApplyForce(Vec2 force, Vec2 point, float deltaTime);

    void Render();

    vector<Vec2> Polygon(Vec2 offset = {0, 0}, float angleOffset = 0);

    void MoveAndRotate(Vec2 diff, float angleDiff, const vector<TerrainSegment>& terrainSegments);

    [[nodiscard]] Vec2 PlayerToWorldPos(Vec2 playerPos) const;

private:
    RayTexture texture = {Paths::Image("bike")};
    float scale = 4;

    void SimulateBoosters(const PlayerUpdateInfo& params);
    [[nodiscard]] optional<float> BoosterRayCastDist(Vec2 origin, float dir, float maxLen, const vector<TerrainSegment>& terrainSegments) const;
};


#endif //HOVERGAME_PLAYER_H
