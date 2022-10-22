//
// Created by Jordanne Stewart on 10/22/22.
//

#ifndef HOVERGAME_PARTICLE_H
#define HOVERGAME_PARTICLE_H

#include "Includes.h"
#include "TerrainSegment.h"

struct Particle {

    Vec2 position;
    Color color;
    float size;
    float angle;
    bool active;
    //vector<Particle> particles;

    Particle(Vec2 pos) : position(pos), size(10){};
    void Render();
    bool Update();


};



#endif //HOVERGAME_PARTICLE_H
