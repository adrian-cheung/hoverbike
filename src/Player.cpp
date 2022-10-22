#include "Player.h"
#include "Util.h"
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
    float speed = 5.0f;

    if (IsKeyDown(KEY_W)) {
        pos.y -= speed;
    }
    if (IsKeyDown(KEY_S)) {
        pos.y += speed;
    }
    if (IsKeyDown(KEY_A)) {
        pos.x -= speed;
    }
    if (IsKeyDown(KEY_D)) {
        pos.x += speed;
    }

    float tiltSpeed = 5.0f * DEG2RAD;
    if (IsKeyDown(KEY_LEFT)) { angle -= tiltSpeed; }
    if (IsKeyDown(KEY_RIGHT)) { angle += tiltSpeed; }

    SimulateBoosters(terrainSegments);
}

vector<Vec2> Player::Polygon() {
    return Util::ToVector(
        vector<Vec2> {
            {-dimens.x, -dimens.y},
            {dimens.x, -dimens.y},
            {dimens.x, dimens.y},
            {-dimens.x, dimens.y}
        } | MAP({ return pos + (it * 0.5f).Rotate(angle); })
    );
}

void Player::SimulateBoosters(const vector<TerrainSegment>& terrainSegments) {
    float maxLen = 100.0f;
    float dir = PI / 2.0f;
    optional<float> backBoosterDist = BoosterRayCastDist(dimens * 0.5f, dir, maxLen, terrainSegments);
    optional<float> frontBoosterDist = BoosterRayCastDist(dimens * 0.5f * Vec2(-1, 1), dir, maxLen, terrainSegments);

    if (backBoosterDist) {
        raylib::DrawText(std::to_string(*backBoosterDist), 50, 50, 30, BLUE);
    }

    if (frontBoosterDist) {
        raylib::DrawText(std::to_string(*frontBoosterDist), 50, 150, 30, BLUE);
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
