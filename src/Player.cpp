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

    float tiltSpeed = 5.0f;
    if (IsKeyDown(KEY_LEFT)) { angle -= tiltSpeed * deltaTime; }
    if (IsKeyDown(KEY_RIGHT)) { angle += tiltSpeed * deltaTime; }

    if (IsKeyDown(KEY_Z)) {
        Vec2 bottomLeft = {pos.x - dimens.x / 2.0f * cos(angle) - dimens.y / 2.0f * sin(angle),
                           pos.y + dimens.y / 2.0f * cos(angle) - dimens.x / 2.0f * sin(angle)};
        ApplyForce({20, -1}, bottomLeft, deltaTime);
    }
    if (IsKeyDown(KEY_X)) {
        ApplyForce({20, -1}, {pos.x + dimens.x / 2.0f, pos.y}, deltaTime);
    }

}

void Player::ApplyForce(Vec2 force, Vec2 point, float deltaTime) {
    Vec2 r = pos - point;

//    Vec2 torque = r.cross(force);
    float torque = r.x * force.y - r.y * force.x;
    float rotInertia = mass * (dimens.x * dimens.x + dimens.y * dimens.y) / 12.0f;

    accel = force / mass;
    vel += accel * deltaTime;
    angularAccel += torque / rotInertia;
    angularVel += angularAccel * deltaTime;
    angle += angularVel * deltaTime;

    pos += vel * deltaTime;
}

void Player::ApplyForceCenter(Vec2 force) {
    accel = force / mass;
}
