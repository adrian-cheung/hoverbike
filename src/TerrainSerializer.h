//
// Created by acheung on 10/23/2022.
//

#ifndef HOVERGAME_TERRAINSERIALIZER_H
#define HOVERGAME_TERRAINSERIALIZER_H


#include "TerrainEditor.h"

class TerrainSerializer {

public:
    const std::string FILENAME = "terrain.txt";
    vector<Vec2> points;
    std::unordered_set<int> gapIndices;
    Vec2 start;
    Vec2 end;

    void Update(vector<TerrainSegment>& terrainSegments, TerrainEditor& terrainEditor);

    void SaveTerrain();

    void LoadTerrain();
};


#endif //HOVERGAME_TERRAINSERIALIZER_H
