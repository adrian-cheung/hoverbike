//
// Created by Tobiathan on 10/21/22.
//

#ifndef HOVERGAME_TERRAINSEGMENT_H
#define HOVERGAME_TERRAINSEGMENT_H


#include "Includes.h"

struct TerrainSegment {
    Vec2 p1;
    Vec2 p2;

    void Render() const;
};


#endif //HOVERGAME_TERRAINSEGMENT_H
