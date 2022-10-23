//
// Created by Jordanne Stewart on 10/22/22.
//

#include "Particle.h"


void Particle::Render() {
    float size = maxSize * (timeLeft / maxTimeLeft);
    DrawRectanglePro({pos.x, pos.y, size, size},
                     {size / 2, size / 2}, angle, color);
}

bool Particle::Update(float deltaTime) {
    pos += vel * deltaTime;
    timeLeft -= deltaTime;

    return timeLeft <= 0.0f;
}
