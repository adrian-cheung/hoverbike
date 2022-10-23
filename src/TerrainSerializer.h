//
// Created by acheung on 10/23/2022.
//

#ifndef HOVERGAME_TERRAINSERIALIZER_H
#define HOVERGAME_TERRAINSERIALIZER_H


#include "TerrainEditor.h"

class TerrainSerializer {
public:
    static void Update(vector<TerrainSegment>& terrainSegments, std::unordered_set<int>& gapIndices, TerrainEditor& terrainEditor);

    static void SaveTerrain(const vector<Vec2> &terrainPoints, const std::string &filename);

    static vector<Vec2> LoadTerrain(const std::string &filename);
};


#endif //HOVERGAME_TERRAINSERIALIZER_H
