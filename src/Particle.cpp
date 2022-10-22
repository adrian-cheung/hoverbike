//
// Created by Jordanne Stewart on 10/22/22.
//

#include "Particle.h"

#define MAX_PARTICLES 200


void Particle::Render() {
    DrawRectanglePro({position.x, position.y, size, size},
                     {size / 2, size / 2}, angle, RayColor(0, 0, 255, 128));
}

bool Particle::Update(float deltaTime, int frames) {
    position = position + vel * deltaTime;
    if (frames % 15 == 0) {
        size -= 1;
    }

    return size <= 0;

}
