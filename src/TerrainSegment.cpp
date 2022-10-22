//
// Created by Tobiathan on 10/21/22.
//

#include "TerrainSegment.h"

void TerrainSegment::Render() const {
    DrawLineEx(p1, p2, 3.0f, RED);
};