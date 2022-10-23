//
// Created by Jordanne Stewart on 10/22/22.
//

#ifndef HOVERGAME_PARTICLE_H
#define HOVERGAME_PARTICLE_H

#include "Includes.h"
#include "TerrainSegment.h"
#include "Random.h"
#include <random>
#include <chrono>

struct Particle {

    Vec2 pos;
    Vec2 vel;
    RayColor color;
    float maxSize;
    float timeLeft;
    float maxTimeLeft;
    float angle = 0.0f;

    Particle(Vec2 pos, Vec2 vel, Color color) : pos(pos), vel(vel), color(color)
    {
        maxTimeLeft = Random::Rand(0.5f, 1.0f);
        timeLeft = maxTimeLeft;
        maxSize = Random::Rand(10, 20);
        angle = Random::Rand(360.0f);
    };
    void Render();
    bool Update(float deltaTime);


};



#endif //HOVERGAME_PARTICLE_H
