#include <Includes.h>
#include "src/Player.h"
#include "src/TerrainSegment.h"
#include "src/Collision.h"
#include "src/PerlinNoise.h"
#include "src/CityScape.h"
#include "src/Particle.h"

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
const int screenWidth = 1920 / 2;
const int screenHeight = 1080 / 2;
const float screenWidthF = (float) screenWidth;
const float screenHeightF = (float) screenHeight;

const int virtualScreenWidth = screenWidth / 8;
const int virtualScreenHeight = screenHeight / 8;
const float virtualRatio = (float)screenWidth/(float)virtualScreenWidth;

Rectangle sourceRec;
Rectangle destRec;

shared_ptr<Player> player;
Camera2D camera = { 0 };
Camera2D pixelCamera = { 0 };
RenderTexture2D target;

vector<TerrainSegment> terrainSegments;
CityScape cityscape1 = CityScape(2, 5, 0, RayColor(0, 0, 0, 128));
CityScape cityscape2 = CityScape(1, 3, 150, RayColor(0, 0, 0, 170));
vector<Particle> particle;

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
            {screenWidthF * -1, 0.0f},
            {0, screenHeightF * 0.7f},
            {screenWidthF / 2.0f, screenHeightF * 0.75f},
            {screenWidthF, screenHeightF * 0.7f},
            {screenWidthF * 3, screenHeightF * 0.73f},
            {screenWidthF * 20, screenHeightF * 0.7f},
    };
    terrainSegments.reserve(terrainPoints.size() - 1);
    for (int i = 0; i < terrainPoints.size() - 1; i++) {
        terrainSegments.push_back({ terrainPoints[i], terrainPoints[i + 1] });
    }

    SetTargetFPS(60);   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    camera.target = player->pos;
    camera.offset = (Vec2){ screenWidthF / 2.0f, screenHeightF / 2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    target = LoadRenderTexture(virtualScreenWidth, virtualScreenHeight);
    sourceRec = { 0.0f, 0.0f, (float)target.texture.width, -(float)target.texture.height };
    destRec = { -virtualRatio, -virtualRatio, screenWidth + (virtualRatio*2), screenHeight + (virtualRatio*2) };

    // Main game loop
    while (!window.ShouldClose())    // Detect window close button or ESC key
    {
//        Update();
        UpdatePlayerCamera(screenWidth, screenHeight);
        UpdateDrawFrame();
    }
//    UnloadRenderTexture(target);    // Unload render texture
//
//    CloseWindow();

    return 0;
}

// update method with WASD key movement
void Update() {
    float deltaTime = GetFrameTime();
    player->Update(deltaTime, terrainSegments);
}

//----------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------
void UpdateDrawFrame()
{
    BeginTextureMode(target);

    // Draw
    //----------------------------------------------------------------------------------
//    BeginDrawing();

    ClearBackground(RAYWHITE);

    PerlinNoise::Render(Vector2Subtract(camera.target, camera.offset), screenWidthF, screenHeightF, 15);
    cityscape1.Render(Vector2Subtract(camera.target, camera.offset), screenWidthF, screenHeightF, 60);
    cityscape2.Render(Vector2Subtract(camera.target, camera.offset), screenWidthF, screenHeightF, 60);

    BeginMode2D(camera);

    Update();

    for (const TerrainSegment& terrainSegment : terrainSegments) {
        terrainSegment.Render();
    }

    player->Render();
    Particle::Render();

    DrawCircle(screenWidth / 2, screenHeight / 2, 10, BLACK);

    if (auto underPlayerPoint = Collision::LineTerrainNearest(player->pos, player->pos + Vec2(0, 100), terrainSegments)) {
        DrawCircleV(*underPlayerPoint, 3.0f, BLUE);
    }

    for (const auto& p : player->Polygon()) {
        DrawCircleV(p, 3.0f, PURPLE);
    }

    if (Collision::PolygonTerrain(player->Polygon(), terrainSegments)) {
        DrawText("Colliding >:O", 10, 450, 30, ORANGE);
    }

    raylib::DrawText("gaming time", 190, 200, 20, LIGHTGRAY);
    raylib::DrawText("player angle: " + std::to_string(player->angle), 190, 250, 20, LIGHTGRAY);

//    EndDrawing();

    EndTextureMode();

    BeginDrawing();
    BeginMode2D(pixelCamera);
    DrawTexturePro(target.texture, sourceRec, destRec, { 0, 0 }, 0.0f, WHITE);
    EndMode2D();
    EndDrawing();

    //----------------------------------------------------------------------------------
}

void UpdatePlayerCamera(int width, int height)
{
    pixelCamera.target = camera.target;

    camera.target.x = (int)pixelCamera.target.x;
    pixelCamera.target.x -= camera.target.x;
    pixelCamera.target.x *= virtualRatio;

    camera.target.y = (int)pixelCamera.target.y;
    pixelCamera.target.y -= camera.target.y;
    pixelCamera.target.y *= virtualRatio;
    
    float minEffectLength = 10;

    camera.offset = (Vec2){ width/2.0f, height/2.0f };
    Vec2 diff = Vector2Subtract(player->pos, camera.target);
    float diffLength = Vector2Length(diff);

    if (diffLength > minEffectLength)
    {
        camera.target = player->pos - ((diff / diffLength) * minEffectLength);
    }
}