#include "Player.h"
#include "Util.h"

void Player::Render() {
	texture.Draw(
            RectF {0, 0, (float) texture.width, (float) texture.height},
            {pos.x, pos.y, (float) texture.width * scale, (float) texture.height * scale},
            {(float) texture.width * scale / 2.0f, (float) texture.height * scale / 2.0f},
            angle,
            WHITE
    );
}

void Player::Update(float deltaTime) {
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

    float tiltSpeed = 5.0f;
    if (IsKeyDown(KEY_LEFT)) { angle -= tiltSpeed; }
    if (IsKeyDown(KEY_RIGHT)) { angle += tiltSpeed; }
}

vector<Vec2> Player::Polygon() {
    Vec2 diff = (Vec2((float) texture.width, (float) texture.height) * scale * 0.5f);

    return Util::ToVector(
        vector<Vec2> {
            {-diff.x, -diff.y},
            {diff.x, -diff.y},
            {diff.x, diff.y},
            {-diff.x, diff.y}
        } | MAP({ return pos + it.Rotate(angle * DEG2RAD); })
    );
}
