#include <Includes.h>
#include "src/Player.h"
#include "src/TerrainSegment.h"
#include "src/Collision.h"

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
    camera.offset = (Vec2){ screenWidth/2.0f, screenHeight/2.0f };
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
    float deltaTime = GetFrameTime();
    player->Update(deltaTime);
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

    DrawCircle(screenWidth / 2, screenHeight / 2, 10, RED);

    if (auto underPlayerPoint = Collision::LineTerrainNearest(player->pos, player->pos + Vec2(0, 100), terrainSegments)) {
        DrawCircleV(*underPlayerPoint, 3.0f, BLUE);
    }

    for (const auto& p : player->Polygon()) {
        DrawCircleV(p, 3.0f, PURPLE);
    }

    if (Collision::PolygonTerrain(player->Polygon(), terrainSegments)) {
        DrawText("Colliding >:O", 10, 450, 30, ORANGE);
    }

    raylib::DrawText("gaming time " + std::to_string(player->angle), 190, 200, 20, LIGHTGRAY);

    EndDrawing();
    //----------------------------------------------------------------------------------
}

void UpdatePlayerCamera(int width, int height)
{
    float minSpeed = 2;
    float minEffectLength = 10;
    float speedMultiplier = 0.1f;

    camera.offset = (Vec2){ width/2.0f, height/2.0f };
    Vec2 diff = Vector2Subtract(player->pos, camera.target);
    float diffLength = Vector2Length(diff);

    if (diffLength > minEffectLength)
    {
        float speed = fmaxf(speedMultiplier*diffLength, minSpeed);
        camera.target = Vector2Add(camera.target, Vector2Scale(diff, speed/diffLength));
    }
    
//    camera.offset = (Vec2){ width/2.0f, height/2.0f };
//    camera.target = player->pos;
}