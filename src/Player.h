#ifndef HOVERGAME_PLAYER_H
#define HOVERGAME_PLAYER_H

#include <Includes.h>
#include "resources/Paths.h"
#include "TerrainSegment.h"
#include "Particle.h"
#include "RagDoll.h"
#include "RigidBody.h"

struct PlayerUpdateInfo {
    float deltaTime;
    vector<TerrainSegment>& terrainSegments;
    vector<Particle>& particles;
    vector<RagDoll>& ragDolls;
};

class Player : public RigidBody {
public:
    static constexpr float GRAVITY = 1000.0f;

    bool godModeEnabled = false;
    bool isDead = false;

    Vec2 force;
    float mass = 1.0f;


    explicit Player(Vec2 pos) : RigidBody() {
        this->pos = pos;
        dimens = Vec2((float) texture.width, (float) texture.height) * scale;
    }

    void Update(const PlayerUpdateInfo& params);

    void ApplyForce(Vec2 force, Vec2 point, float deltaTime);

    void Render();

    [[nodiscard]] Vec2 PlayerToWorldPos(Vec2 playerPos) const;

    RayTexture& ActiveTexture();

    vector<Vec2> Polygon(Vec2 offset = {0, 0}, float angleOffset = 0) override;

    vector<Vec2> PlayerPolygon(Vec2 offset = {0, 0}, float angleOffset = 0);

    void Die(vector<RagDoll>& ragDolls);

    [[nodiscard]] bool IsCapable() { return !isDead; }

private:
    RayTexture texture = {Paths::Image("bike")};
    RayTexture textureDead = {Paths::Image("bikeonly")};
    float scale = 4;

    void SimulateBoosters(const PlayerUpdateInfo& params);
    [[nodiscard]] optional<float> BoosterRayCastDist(Vec2 origin, float dir, float maxLen, const vector<TerrainSegment>& terrainSegments) const;
};


#endif //HOVERGAME_PLAYER_H
