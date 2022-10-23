#include "Player.h"
#include "Collision.h"
#include "Util.h"

void Player::Render() {
    ActiveTexture().Draw(
            RectF {0, 0, (float) texture.width, (float) texture.height},
            {pos.x, pos.y, (float) texture.width * scale, (float) texture.height * scale},
            {(float) texture.width * scale / 2.0f, (float) texture.height * scale / 2.0f},
            angle * RAD2DEG,
            WHITE
    );
}

void Player::Update(const PlayerUpdateInfo& params) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vec2 theVec = (Util::MousePosWorld() - pos) / dimens;
        std::cout << theVec.x << ' ' << theVec.y << '\n';
    }

    auto& [deltaTime, terrainSegments, particles, ragDolls] = params;

    accel = Vec2 {};
    angularAccel = 0.0f;

    // gravity :)
    accel.y += GRAVITY;

    //user input

    if (godModeEnabled) {
        float flySpeed = 600.0f;
        if (IsKeyDown(KEY_W)) {pos.y -= flySpeed * deltaTime;}
        if (IsKeyDown(KEY_S)) {pos.y += flySpeed * deltaTime;}
        if (IsKeyDown(KEY_A)) {pos.x -= flySpeed * deltaTime;}
        if (IsKeyDown(KEY_D)) {pos.x += flySpeed * deltaTime;}

        float tiltSpeed = 200.0f * DEG2RAD;
        if (IsKeyDown(KEY_LEFT)) { angle -= tiltSpeed * deltaTime; }
        if (IsKeyDown(KEY_RIGHT)) { angle += tiltSpeed * deltaTime; }
    }

    SimulateBoosters(params);

    Vec2 forwardForce = Vec2 (500, 0).Rotate(angle);
    if (IsCapable() && IsKeyDown(KEY_SPACE)) {
        Vec2 leftMiddle = PlayerToWorldPos(dimens * Vec2(-0.5f, 0.0f));
//        DrawCircleV(leftMiddle, 10, GREEN);
        ApplyForce(forwardForce, leftMiddle, deltaTime);

        for (int i = 0; i < 3; i++) {
            particles.emplace_back(
                    leftMiddle + Vec2(0, 1) * Random::Rand(20.0f),
                    Util::Polar(
                            angle + PI + Random::RandPN(30 * DEG2RAD),
                            Random::Rand(100, 150)
                    ),
                    RayColor(
                            Random::RandI(200, 255),
                            Random::RandI(0, 255),
                            Random::RandI(0, 10),
                            Random::RandI(100, 150)
                    ));
        }
    }

    if (IsCapable() && IsKeyDown(KEY_Z)) {
        angularVel -= 0.5f;
    }
    if (IsCapable() && IsKeyDown(KEY_X)) {
        angularVel += 0.5f;
    }

//    ApplyForce(vel * -0.5f, pos, deltaTime);

    vel += accel * deltaTime;
    angularVel += angularAccel * deltaTime;
    angularVel *= 0.95f;

    if (!godModeEnabled) {
        MoveAndRotate(vel * deltaTime, angularVel * deltaTime, terrainSegments);
    }


    if (!isDead && Collision::PolygonTerrain(PlayerPolygon(), terrainSegments)) {
        Die(ragDolls);
    }

    if (IsKeyPressed(KEY_L)) {
        if (!isDead) {
            Die(ragDolls);
        } else {
            isDead = false;
        }
    }
}

void Player::ApplyForce(Vec2 force, Vec2 point, float deltaTime) {
    Vec2 r = point - pos;

    float torqueMultiplier = 1.0f;
    float torque = (r.x * force.y - r.y * force.x) * torqueMultiplier;
    float rotInertia = mass * (dimens.x * dimens.x + dimens.y * dimens.y) / 12.0f;

    accel += force / mass;
    angularAccel += torque / rotInertia;
}

vector<Vec2> Player::Polygon(Vec2 offset, float angleOffset) {
    vector<Vec2> unTranslatedPoints = {
            {-0.513077, 0.0355813},
            {0.474355, -0.122622},
            {0.469684, 0.341992},
            {-0.510615, 0.382778}
    };

    return unTranslatedPoints | MAP({ return pos + offset + (it * dimens).Rotate(angle + angleOffset); }) | to_vector{};
}

void Player::SimulateBoosters(const PlayerUpdateInfo& params) {
    auto& [deltaTime, terrainSegments, particles, ragDolls] = params;

    float maxLen = 100.0f;
    float dir = PI / 2.0f;
    float angleOffset = 0;

    Vec2 backBoosterPos = PlayerToWorldPos(dimens * Vec2(-0.36f, 0.5f));
    Vec2 frontBoosterPos = PlayerToWorldPos(dimens * Vec2(0.36f, 0.5f));

    optional<float> backBoosterDist = BoosterRayCastDist(backBoosterPos, dir + angleOffset, maxLen, terrainSegments);
    optional<float> frontBoosterDist = BoosterRayCastDist(frontBoosterPos, dir - angleOffset, maxLen, terrainSegments);

    // sigmoid function
    const auto lenToForce = [&](float len){
        return -(GRAVITY * 1.5f) / (1.0f + exp(-(maxLen * 0.5f - len) * 0.1f));
        //        return -(maxLen - len) * 10.0f;
    };

    const auto TryBoost = [&](optional<float> boosterDist, Vec2 boosterPos, float boosterAngle) {
        if (boosterDist) {
            Vec2 boosterForce = Vec2 (0, lenToForce(*boosterDist)).Rotate(boosterAngle);

//            DrawLineV(boosterPos, boosterPos - boosterForce, WHITE);
//            DrawCircleV(boosterPos, 10, RED);
            ApplyForce(boosterForce, boosterPos, deltaTime);
        }
        if (Random::Rand() < 0.7f) {
            float particleAngle = boosterAngle + (PI / 2) + Random::RandPN(30 * DEG2RAD);

            RayColor particleColor = RayColor(
                    Random::RandI(80, 90),
                    Random::RandI(183, 190),
                    Random::RandI(212, 225),
                    125
            );
            particles.emplace_back(
            boosterPos,
            Util::Polar(particleAngle, 250.0f * Random::Rand(1.0f, 1.5f)),
            particleColor);
        }

    };


    TryBoost(backBoosterDist, backBoosterPos, angle + angleOffset);
    TryBoost(frontBoosterDist, frontBoosterPos, angle - angleOffset);
}

optional<float> Player::BoosterRayCastDist(Vec2 origin, float dir, float maxLen, const vector<TerrainSegment>& terrainSegments) const {
    Vec2 rayEnd = origin + Vec2(maxLen, 0).Rotate(angle + dir);

    DrawLineEx(origin, rayEnd, 1.0f, BLUE);

    if (auto collision = Collision::LineTerrainNearest(origin, rayEnd, terrainSegments)) {
        DrawCircleV(*collision, 10.0f, ORANGE);
        return collision->Distance(origin);
    }

    return std::nullopt;
}

Vec2 Player::PlayerToWorldPos(Vec2 playerPoint) const {
    return pos + playerPoint.Rotate(angle);
}

RayTexture &Player::ActiveTexture() {
    return isDead ? textureDead : texture;
}

vector<Vec2> Player::PlayerPolygon(Vec2 offset, float angleOffset) {
    vector<Vec2> unTranslatedPoints = {
            {-0.238239, 0.177102},
            {0.00445822, -0.451804},
            {0.236286, -0.383904},
            {0.11464, 0.189511}
    };

    return unTranslatedPoints | MAP({ return pos + offset + (it * dimens).Rotate(angle + angleOffset); }) | to_vector{};
}

void Player::Die(vector<RagDoll>& ragDolls) {
    isDead = true;
    ragDolls.emplace_back(
            pos + Vec2(0, -30).Rotate(angle), vel, angle, angularVel + Random::RandPN(20), scale
    );
}
