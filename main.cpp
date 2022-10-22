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
Camera2D camera = { 0 };
vector<Vec2> terrainPoints = {
        {0, 600},
        {(float) screenWidth / 2.0f, 700},
        {(float) screenWidth, 600},
};
vector<TerrainSegment> terrainSegments;

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
void UpdateDrawFrame();     // Update and Draw one frame
void UpdatePlayerCamera(int width, int height);
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

//    std::vector<int> v = {1, 2, 3, 4, 5};
//    for (int n : v | std::views::take(3)) {
//        printf("%i\n", n);
//    }

    camera.target = player->pos;
    camera.offset = (Vector2){ screenWidth/2.0f, screenHeight/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    // Main game loop
    while (!window.ShouldClose())    // Detect window close button or ESC key
    {

        Update();
        UpdatePlayerCamera(screenWidth, screenHeight);
        UpdateDrawFrame();

//        UpdatePlayerCamera();
//        UpdateCamera();
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

    BeginMode2D(camera);

    for (const TerrainSegment& terrainSegment : terrainSegments) {
        terrainSegment.Render();
    }

    player->Render();

    DrawText("gaming time", 190, 200, 20, LIGHTGRAY);

    EndDrawing();
    //----------------------------------------------------------------------------------
}

void UpdatePlayerCamera(int width, int height)
{
//    // Camera
//    //----------------------------------------------------------------------------------
//    static raylib::Camera2D camera({(float) screenWidth / 2.0f, (float) screenHeight / 2.0f}, {0, 0}, 0.0f, 1.0f);
//    camera.target = player->pos;
//    camera.offset = {0, 0};
//    camera.zoom = 1.0f;
//    BeginMode2D(camera);
////    raylib::DrawPoly(terrainPoints.data(), terrainPoints.size(), GREEN);
//    EndMode2D();
//    //----------------------------------------------------------------------------------
//    static Vector2 bbox = { 0.2f, 0.2f };
//
//    Vector2 bboxWorldMin = GetScreenToWorld2D((Vector2){ (1 - bbox.x)*0.5f*width, (1 - bbox.y)*0.5f*height }, camera);
//    Vector2 bboxWorldMax = GetScreenToWorld2D((Vector2){ (1 + bbox.x)*0.5f*width, (1 + bbox.y)*0.5f*height }, camera);
//    camera.offset = (Vector2){ (1 - bbox.x)*0.5f * width, (1 - bbox.y)*0.5f*height };
//
//    if (player->pos.x < bboxWorldMin.x) camera.target.x = player->pos.x;
//    if (player->pos.y < bboxWorldMin.y) camera.target.y = player->pos.y;
//    if (player->pos.x > bboxWorldMax.x) camera.target.x = bboxWorldMin.x + (player->pos.x - bboxWorldMax.x);
//    if (player->pos.y > bboxWorldMax.y) camera.target.y = bboxWorldMin.y + (player->pos.y - bboxWorldMax.y);

    camera.offset = (Vector2){ width/2.0f, height/2.0f };
    camera.target = player->pos;
}