#include <Includes.h>
#include "src/Player.h"
#include "src/TerrainSegment.h"

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
int screenWidth = 1920 / 2;
int screenHeight = 1080 / 2;
float screenWidthF = (float) screenWidth;
float screenHeightF = (float) screenHeight;
shared_ptr<Player> player;
vector<TerrainSegment> terrainSegments;

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
void UpdateDrawFrame();     // Update and Draw one frame
void Update();

//----------------------------------------------------------------------------------
// Main Entry Point
//----------------------------------------------------------------------------------
int main()
{


    // Initialization
    //--------------------------------------------------------------------------------------
    raylib::Window window(screenWidth, screenHeight, "raylib-cpp [core] example - basic window");
    player = std::make_shared<Player>(Vec2 {screenWidthF / 2, screenHeightF / 2});

    vector<Vec2> terrainPoints = {
            {0, screenHeightF * 0.7f},
            {screenWidthF / 2.0f, screenHeightF * 0.75f},
            {screenWidthF, screenHeightF * 0.7f},
    };
    terrainSegments.reserve(terrainPoints.size() - 1);
    for (int i = 0; i < terrainPoints.size() - 1; i++) {
        terrainSegments.push_back({ terrainPoints[i], terrainPoints[i + 1] });
    }

    SetTargetFPS(60);   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!window.ShouldClose())    // Detect window close button or ESC key
    {
        Update();
        UpdateDrawFrame();
    }

    return 0;
}

// update method with WASD key movement
void Update() {
    player->Update();
}

//----------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------
void UpdateDrawFrame()
{
    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

    ClearBackground(RAYWHITE);

    for (const TerrainSegment& terrainSegment : terrainSegments) {
        terrainSegment.Render();
    }

    player->Render();

    DrawText("gaming time", 190, 200, 20, LIGHTGRAY);

    EndDrawing();
    //----------------------------------------------------------------------------------
}