//
// Created by acheung on 10/23/2022.
//

#include <sstream>
#include "TerrainSerializer.h"

void TerrainSerializer::SaveTerrain(const vector<Vec2>& terrainPoints, const std::string& filename) {
    std::string fileContents;
    for (auto point: terrainPoints) {
        fileContents += std::to_string(point.x) + "," + std::to_string(point.y) + "\n";
    }
    raylib::SaveFileText(filename, fileContents);
}

vector<Vec2> TerrainSerializer::LoadTerrain(const std::string& filename) {
    vector<Vec2> terrainPoints;
    std::stringstream ss(raylib::LoadFileText(filename));
    std::string line;
    while (std::getline(ss, line)) {
        std::stringstream lineStream(line);
        std::string x, y;
        std::getline(lineStream, x, ',');
        std::getline(lineStream, y, ',');
        terrainPoints.push_back(Vec2(std::stof(x), std::stof(y)));
    }
    return terrainPoints;
}

void TerrainSerializer::Update(vector<TerrainSegment>& terrainSegments, std::unordered_set<int>& gapIndices, TerrainEditor& terrainEditor) {
    if (IsKeyPressed(KEY_F1)) {
        SaveTerrain(terrainEditor.points, "terrain.txt");
    }
    if (IsKeyPressed(KEY_F2)) {
        terrainEditor.points = LoadTerrain("terrain.txt");
        terrainEditor.RebuildTerrain(terrainSegments, gapIndices);
    }
}
