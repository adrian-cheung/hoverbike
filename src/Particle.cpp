//
// Created by Jordanne Stewart on 10/22/22.
//

#include "Particle.h"

#define MAX_PARTICLES 200


void Particle::Render() {

    DrawCircle(position.x , position.y , size, RED);


}

bool Particle::Update(float deltaTime) {
    position = position + vel * deltaTime;
    size -= 1;

    return size <= 0;

}
