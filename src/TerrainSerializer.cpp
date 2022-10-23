//
// Created by acheung on 10/23/2022.
//

#include <sstream>
#include "TerrainSerializer.h"

void TerrainSerializer::SaveTerrain() {
    std::string fileContents;
    fileContents += std::to_string(start.x) + " " + std::to_string(start.y) + " " + std::to_string(end.x) + " " + std::to_string(end.y);
    for (auto point : points) {
        fileContents += std::to_string(point.x) + " " + std::to_string(point.y) + "\n";
    }
    fileContents += "\n";
    for (auto gapIndex : gapIndices) {
        fileContents += std::to_string(gapIndex) + " ";
    }
    raylib::SaveFileText(FILENAME, fileContents);
}

void TerrainSerializer::LoadTerrain() {
    vector<Vec2> terrainPoints;
    std::stringstream ss(raylib::LoadFileText(FILENAME));
    ss >> start.x >> start.y >> end.x >> end.y;
    std::string line;
    while (!line.empty() && std::getline(ss, line)) {
        std::stringstream lineStream(line);
        float x, y;
        lineStream >> x >> y;
        terrainPoints.push_back(Vec2(x, y));
    }
    std::getline(ss, line);
    std::stringstream lineStream(line);
    int gapIndex;
    while (lineStream >> gapIndex) {
        gapIndices.insert(gapIndex);
    }
}

void TerrainSerializer::Update(vector<TerrainSegment>& terrainSegments, std::unordered_set<int>& gapIndices, TerrainEditor& terrainEditor) {
    if (IsKeyPressed(KEY_F1)) {
        SaveTerrain();
    }
    if (IsKeyPressed(KEY_F2)) {
        LoadTerrain();
        terrainEditor.RebuildTerrain(terrainSegments, gapIndices);
    }
}
