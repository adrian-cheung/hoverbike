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


    static void Render();
    void Update(float deltaTime, const vector<TerrainSegment>& terrainSegments);

};



#endif //HOVERGAME_PARTICLE_H
