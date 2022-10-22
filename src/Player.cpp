#include "Player.h"
#include "Collision.h"

void Player::Render() {
	texture.Draw(
            RectF {0, 0, (float) texture.width, (float) texture.height},
            {pos.x, pos.y, (float) texture.width * scale, (float) texture.height * scale},
            {(float) texture.width * scale / 2.0f, (float) texture.height * scale / 2.0f},
            angle * RAD2DEG,
            WHITE
    );
}

void Player::Update(float deltaTime, const vector<TerrainSegment>& terrainSegments) {
    accel = Vec2 {};
    angularAccel = 0.0f;

    // gravity :)
    accel.y += GRAVITY;

    //user input
    float speed = 300.0f;

    if (IsKeyDown(KEY_W)) {
        pos.y -= speed * deltaTime;
    }
    if (IsKeyDown(KEY_S)) {
        pos.y += speed * deltaTime;
    }
    if (IsKeyDown(KEY_A)) {
        pos.x -= speed * deltaTime;
    }
    if (IsKeyDown(KEY_D)) {
        pos.x += speed * deltaTime;
    }

    float tiltSpeed = 200.0f * DEG2RAD;
    if (IsKeyDown(KEY_LEFT)) { angle -= tiltSpeed * deltaTime; }
    if (IsKeyDown(KEY_RIGHT)) { angle += tiltSpeed * deltaTime; }

    SimulateBoosters(terrainSegments, deltaTime);

    Vec2 forwardForce = Vec2 (500, 0).Rotate(angle);
    Vec2 rotateForce = Vec2 (0, -500).Rotate(angle);
    if (IsKeyDown(KEY_SPACE)) {
        Vec2 leftMiddle = PlayerToWorldPos(dimens * 0.5f * Vec2(-1, 0));
        DrawCircleV(leftMiddle, 10, GREEN);
        ApplyForce(forwardForce, leftMiddle, deltaTime);
    }
//    if (IsKeyDown(KEY_Z)) {
//        Vec2 bottomLeft = PlayerToWorldPos(dimens * 0.5f * Vec2(-1.0f, 1.0f));
//        DrawCircleV(bottomLeft, 10, GREEN);
//        ApplyForce(rotateForce, bottomLeft, deltaTime);
//    }
//    if (IsKeyDown(KEY_X)) {
//        Vec2 bottomRight = PlayerToWorldPos(dimens * 0.5f * Vec2(1.0f, 1.0f));
//        DrawCircleV(bottomRight, 10, GREEN);
//        ApplyForce(rotateForce, bottomRight, deltaTime);
//    }

    if (IsKeyDown(KEY_Z)) {
        angularVel -= 0.5f;
//        angularAccel -= 0.1f;
    }
    if (IsKeyDown(KEY_X)) {
        angularVel += 0.5f;
//        angularAccel += 0.1f;
    }

    ApplyForce(vel * -0.5f, pos, deltaTime);

    vel += accel * deltaTime;
    angularVel += angularAccel * deltaTime;
    angularVel *= 0.95f;
    MoveAndRotate(vel * deltaTime, angularVel * deltaTime, terrainSegments);
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

void Player::SimulateBoosters(const vector<TerrainSegment>& terrainSegments, float deltaTime) {
    float maxLen = 100.0f;
    float dir = PI / 2.0f;
    float angleOffset = 0;

    optional<float> backBoosterDist = BoosterRayCastDist(dimens * 0.5f * Vec2(-1, 1), dir + angleOffset, maxLen, terrainSegments);
    optional<float> frontBoosterDist = BoosterRayCastDist(dimens * 0.5f, dir - angleOffset, maxLen, terrainSegments);

    // sigmoid function
    const auto lenToForce = [&](float len){
        return -(GRAVITY * 1.5f) / (1.0f + exp(-(maxLen * 0.5f - len) * 0.1f));
        //        return -(maxLen - len) * 10.0f;
    };

    if (backBoosterDist) {
        raylib::DrawText(std::to_string(*backBoosterDist), 50, 50, 30, BLUE);

        Vec2 backForce = Vec2 (0, lenToForce(*backBoosterDist)).Rotate(angle + angleOffset);
        Vec2 bottomLeft = PlayerToWorldPos(dimens * 0.5f * Vec2(-1.0f, 1.0f));

        DrawLineV(bottomLeft, bottomLeft - backForce, WHITE);
        DrawCircleV(bottomLeft, 10, RED);
        ApplyForce(backForce, bottomLeft, deltaTime);
    }

    if (frontBoosterDist) {
        raylib::DrawText(std::to_string(*frontBoosterDist), 50, 150, 30, BLUE);

        Vec2 frontForce = Vec2 (0, lenToForce(*frontBoosterDist)).Rotate(angle - angleOffset);
        Vec2 bottomRight = PlayerToWorldPos(dimens * 0.5f * Vec2(1.0f, 1.0f));

        DrawLineV(bottomRight, bottomRight - frontForce, WHITE);
        DrawCircleV(bottomRight, 10, RED);
        ApplyForce(frontForce, bottomRight, deltaTime);
    }
}

optional<float> Player::BoosterRayCastDist(Vec2 playerPoint, float dir, float maxLen, const vector<TerrainSegment>& terrainSegments) const {
    Vec2 origin = PlayerToWorldPos(playerPoint);
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
