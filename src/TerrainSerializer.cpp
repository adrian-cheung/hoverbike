//
// Created by acheung on 10/23/2022.
//

#include <sstream>
#include "TerrainSerializer.h"

void TerrainSerializer::SaveTerrain(vector<Vec2>& points, std::unordered_set<int>& gapIndices, Vec2& start, Vec2& end) {
    std::string fileContents;
    fileContents += std::to_string(start.x) + " " + std::to_string(start.y) + " " + std::to_string(end.x) + " " + std::to_string(end.y) + "\n";
    for (auto point : points) {
        fileContents += std::to_string(point.x) + " " + std::to_string(point.y) + "\n";
    }
    std::cout << fileContents << std::endl;
    fileContents += "\n";
    for (auto gapIndex : gapIndices) {
        fileContents += std::to_string(gapIndex) + " ";
    }
    raylib::SaveFileText(FILENAME, fileContents);
}

void TerrainSerializer::LoadTerrain(vector<Vec2>& points, std::unordered_set<int>& gapIndices, Vec2& start, Vec2& end) {
    std::stringstream ss(raylib::LoadFileText(FILENAME));
    ss >> start.x >> start.y >> end.x >> end.y;
    std::cout << start.x << " " << start.y << " " << end.x << " " << end.y << std::endl;
    std::string line;
    std::getline(ss, line);
    while (std::getline(ss, line) && !line.empty()) {
        std::stringstream lineStream(line);
        float x, y;
        lineStream >> x >> y;
        points.push_back(Vec2(x, y));
        std::cout << std::to_string(x) << " " << std::to_string(y) << std::endl;
    }
    std::getline(ss, line);
    std::stringstream lineStream(line);
    int gapIndex;
    while (lineStream >> gapIndex) {
        gapIndices.insert(gapIndex);
        std::cout << std::to_string(gapIndex) << std::endl;
    }
    std::cout << "helloge4" << std::endl;
}

void TerrainSerializer::Update(vector<TerrainSegment>& terrainSegments, vector<Vec2>& points, std::unordered_set<int>& gapIndices, Vec2& start, Vec2& end, TerrainEditor& terrainEditor) {
    if (IsKeyPressed(KEY_F1)) {
        SaveTerrain(points, gapIndices, start, end);
    }
    if (IsKeyPressed(KEY_F2)) {
        LoadTerrain(points, gapIndices, start, end);
        terrainEditor.RebuildTerrain(terrainSegments, gapIndices);
    }
}
