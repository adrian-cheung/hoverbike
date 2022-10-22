#include "Player.h"

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
