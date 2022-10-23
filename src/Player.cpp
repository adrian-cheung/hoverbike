#include "Player.h"
#include "Collision.h"
#include "Util.h"

void Player::Render() {
	texture.Draw(
            RectF {0, 0, (float) texture.width, (float) texture.height},
            {pos.x, pos.y, (float) texture.width * scale, (float) texture.height * scale},
            {(float) texture.width * scale / 2.0f, (float) texture.height * scale / 2.0f},
            angle * RAD2DEG,
            WHITE
    );
}

void Player::Update(const PlayerUpdateInfo& params) {
    auto& [deltaTime, terrainSegments, particles] = params;

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
    if (IsKeyDown(KEY_SPACE)) {
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

    if (IsKeyDown(KEY_Z)) {
        angularVel -= 0.5f;
    }
    if (IsKeyDown(KEY_X)) {
        angularVel += 0.5f;
    }

    ApplyForce(vel * -0.5f, pos, deltaTime);

    vel += accel * deltaTime;
    angularVel += angularAccel * deltaTime;
    angularVel *= 0.95f;

    if (!godModeEnabled) {
        MoveAndRotate(vel * deltaTime, angularVel * deltaTime, terrainSegments);
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
            {-dimens.x, -dimens.y},
            {dimens.x, -dimens.y},
            {dimens.x, dimens.y},
            {-dimens.x, dimens.y}
    };

    return unTranslatedPoints | MAP({ return pos + offset + (it * 0.5f).Rotate(angle + angleOffset); }) | to_vector{};
}

void Player::SimulateBoosters(const PlayerUpdateInfo& params) {
    auto& [deltaTime, terrainSegments, particles] = params;

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

void Player::MoveAndRotate(Vec2 diff, float angleDiff, const vector<TerrainSegment> &terrainSegments) {
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
            vel.x = -vel.x * 0.9f;
            break;
        }
    }

    float diffY = 0;
    for (int i = 0; i < (int) (diff.y / STEP_LEN); i++) {
        diffY += STEP_LEN;
        if (CollidesWithDiff({diffX, diffY})) {
            diffY -= STEP_LEN;
            vel.y = -vel.y * 0.9f;
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
            angularVel = -angularVel * 0.9f;
            break;
        }
    }
    angle += currAngleDiff;
}
