//
// Created by Jordanne Stewart on 10/22/22.
//

#ifndef HOVERGAME_PARTICLE_H
#define HOVERGAME_PARTICLE_H

#include "Includes.h"
#include "TerrainSegment.h"

struct Particle {

    Vec2 position;
    Vec2 vel;
    Color color;
    float size;
    float angle;
    bool active;
    //vector<Particle> particles;

    Particle(Vec2 pos, Vec2 vel) : position(pos), vel(vel), size(10){};
    void Render();
    bool Update(float deltaTime);


};



#endif //HOVERGAME_PARTICLE_H
