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

    Vec2 forwardForce = Vec2 (20, 0).Rotate(angle);
    Vec2 rotateForce = Vec2 (0, -5).Rotate(angle);
    if (IsKeyDown(KEY_SPACE)) {
        Vec2 leftMiddle = PlayerToWorldPos(dimens * 0.5f * Vec2(-1, 0));
        DrawCircleV(leftMiddle, 10, GREEN);
        ApplyForce(forwardForce, leftMiddle, deltaTime);
    }
    if (IsKeyDown(KEY_Z)) {
//        Vec2 bottomLeft = {pos.x - dimens.x / 2.0f * cos(angle) - dimens.y / 2.0f * sin(angle),
//                           pos.y + dimens.y / 2.0f * cos(angle) - dimens.x / 2.0f * sin(angle)};
        Vec2 bottomLeft = PlayerToWorldPos(dimens * 0.5f * Vec2(-1.0f, 1.0f));
        DrawCircleV(bottomLeft, 10, GREEN);
        ApplyForce(rotateForce, bottomLeft, deltaTime);
    }
    if (IsKeyDown(KEY_X)) {
        Vec2 bottomRight = PlayerToWorldPos(dimens * 0.5f * Vec2(1.0f, 1.0f));
        DrawCircleV(bottomRight, 10, GREEN);
        ApplyForce(rotateForce, bottomRight, deltaTime);
    }

    vel += accel * deltaTime;
    angularVel += angularAccel * deltaTime;
    angle += angularVel * deltaTime;
    pos += vel * deltaTime;
    accel = Vec2(0, GRAVITY);
    angularAccel = 0;

}

void Player::ApplyForce(Vec2 force, Vec2 point, float deltaTime) {
    Vec2 r = point - pos;

//    Vec2 torque = r.cross(force);
    float torqueMultiplier = 0.1f;
    float torque = (r.x * force.y - r.y * force.x) * torqueMultiplier;
    float rotInertia = mass * (dimens.x * dimens.x + dimens.y * dimens.y) / 12.0f;

    accel = force / mass;
    angularAccel = torque / rotInertia;
}

vector<Vec2> Player::Polygon() {
    vector<Vec2> unTranslatedPoints = {
            {-dimens.x, -dimens.y},
            {dimens.x, -dimens.y},
            {dimens.x, dimens.y},
            {-dimens.x, dimens.y}
    };

    return unTranslatedPoints | MAP({ return pos + (it * 0.5f).Rotate(angle); }) | to_vector{};
}

void Player::SimulateBoosters(const vector<TerrainSegment>& terrainSegments, float deltaTime) {
    float maxLen = 100.0f;
    float dir = PI / 2.0f;
    optional<float> backBoosterDist = BoosterRayCastDist(dimens * 0.5f, dir, maxLen, terrainSegments);
    optional<float> frontBoosterDist = BoosterRayCastDist(dimens * 0.5f * Vec2(-1, 1), dir, maxLen, terrainSegments);

    if (backBoosterDist) {
        raylib::DrawText(std::to_string(*backBoosterDist), 50, 50, 30, BLUE);

        Vec2 backForce = Vec2 (0, -pow(maxLen - *backBoosterDist, 2) / 50).Rotate(angle + PI/6);
        Vec2 bottomLeft = PlayerToWorldPos(dimens * 0.5f * Vec2(-1.0f, 1.0f));

        DrawLineV(bottomLeft, bottomLeft - backForce, WHITE);
        DrawCircleV(bottomLeft, 10, RED);
        ApplyForce(backForce, bottomLeft, deltaTime);
    }

    if (frontBoosterDist) {
        raylib::DrawText(std::to_string(*frontBoosterDist), 50, 150, 30, BLUE);

        Vec2 frontForce = Vec2 (0, -pow(maxLen - *frontBoosterDist, 2) / 50).Rotate(angle - PI/6);
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
