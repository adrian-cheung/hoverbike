#include <Includes.h>
#include "src/Player.h"
#include "src/TerrainSegment.h"
#include "src/Collision.h"
#include "src/PerlinNoise.h"
#include "src/CityScape.h"
#include "src/Particle.h"
#include "src/TerrainEditor.h"
#include "src/Util.h"
#include "src/RagDoll.h"

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
const int screenWidth = 1920 / 2;
const int screenHeight = 1080 / 2;
const float screenWidthF = (float) screenWidth;
const float screenHeightF = (float) screenHeight;

const int virtualRatio = 4;
const int virtualScreenWidth = screenWidth / virtualRatio;
const int virtualScreenHeight = screenHeight / virtualRatio;

shared_ptr<Player> player;
Camera2D camera = { 0 };
Camera2D theOtherCamera = { 0 };
vector<Vec2> terrainPoints;
vector<TerrainSegment> terrainSegments;
TerrainEditor terrainEditor = TerrainEditor({screenWidthF * -1.0f, screenHeightF * 0.75f}, {screenWidthF * 10.0f, screenHeightF * 0.75f});
CityScape cityscape1 = CityScape(2, 5, 0, RayColor(0, 0, 0, 128));
CityScape cityscape2 = CityScape(1, 3, 150, RayColor(0, 0, 0, 170));
vector<Particle> particles;
vector<RagDoll> ragDolls;
raylib::RenderTexture target;

RectF destRect;
RectF srcRect;

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
void UpdateDrawFrame();     // Update and Draw one frame
void UpdatePlayerCamera(int width, int height);
void UpdateTerrain();
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
    // Add points with terrainEditor

//    terrainEditor.AddPoint(Vec2 {screenWidthF * 1, screenHeightF * 0.9f});
//    terrainEditor.AddPoint(Vec2 {screenWidthF * 4, screenHeightF * -1.0f});
//    terrainEditor.AddPoint(Vec2 {screenWidthF * 2, screenHeightF * 0.4f});
//    terrainEditor.AddPoint(Vec2 {screenWidthF * 3, screenHeightF * 0.7f});
    // print points
    for (auto& point : terrainEditor.points) {
        std::cout << point.x << ", " << point.y << std::endl;
    }

    terrainSegments.reserve(terrainEditor.points.size() - 1);
    for (int i = 0; i < terrainEditor.points.size() - 1; i++) {
        terrainSegments.push_back({ terrainEditor.points[i], terrainEditor.points[i + 1] });
    }


//    vector<Vec2> terrainPoints = {
//            {screenWidthF * -1, 0.0f},
//            {0, screenHeightF * 0.7f},
//            {screenWidthF / 2.0f, screenHeightF * 0.75f},
//            {screenWidthF, screenHeightF * 0.7f},
//            {screenWidthF * 2, screenHeightF * 0.2f},
//            {screenWidthF * 20, screenHeightF * 0.7f},
//    };
//    terrainSegments.reserve(terrainPoints.size() - 1);
//    for (int i = 0; i < terrainPoints.size() - 1; i++) {
//        terrainSegments.push_back({ terrainPoints[i], terrainPoints[i + 1] });
//    }

    SetTargetFPS(60);   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    camera.target = player->pos;
    camera.offset = (Vec2){ screenWidthF / 2.0f, screenHeightF / 2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    Util::Setup(camera);

    theOtherCamera.zoom = 1.0f;
    theOtherCamera.rotation = 0.0f;

    target = raylib::RenderTexture(virtualScreenWidth, virtualScreenHeight);

    // The target's height is flipped (in the source Rectangle), due to OpenGL reasons
    srcRect = RectF { 0.0f, 0.0f, (float)target.texture.width, -(float)target.texture.height };
    destRect = RectF { 0, 0, screenWidth, screenHeight };


    // Main game loop
    while (!window.ShouldClose())    // Detect window close button or ESC key
    {
        UpdatePlayerCamera(screenWidth, screenHeight);
        UpdateTerrain();
        UpdateDrawFrame();
    }

    return 0;
}

// update method with WASD key movement
void Update() {
    if (IsKeyPressed(KEY_G)) {
        player->godModeEnabled ^= true;
        player->vel = Vec2 {};
        player->angularVel = 0.0f;
    }

    float deltaTime = GetFrameTime();
    player->Update({deltaTime, terrainSegments, particles, ragDolls});
}

//----------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------
void UpdateDrawFrame()
{
    BeginDrawing();
    ClearBackground(RAYWHITE);

    PerlinNoise::Render(Vector2Subtract(camera.target, camera.offset), screenWidthF, screenHeightF, 15);
    cityscape1.Render(Vector2Subtract(camera.target, camera.offset), screenWidthF, screenHeightF, 60);
    cityscape2.Render(Vector2Subtract(camera.target, camera.offset), screenWidthF, screenHeightF, 60);

    BeginMode2D(camera);

    Update();

    // render terrain points
    for (const Vec2& point : terrainEditor.points) {
        DrawCircleV(point, 20, WHITE);
    }
    for (const TerrainSegment& terrainSegment : terrainSegments) {
        terrainSegment.Render();
    }

    for (Vec2 p : player->Polygon()) {
        DrawCircleV(p, 5, PURPLE);
    }

    terrainEditor.DebugRender();

    for(int i = 0; i < particles.size(); i++) {
        particles[i].Render();

        if (particles[i].Update(GetFrameTime())) {
            particles.erase(particles.begin() + i);
        }
    }

    EndMode2D();

    BeginTextureMode(target);
    ClearBackground({0, 0, 0, 0});
    BeginMode2D(theOtherCamera);
    // BEGIN PIXELATED ==========================
    player->Render();

    for (int i = 0; i < ragDolls.size(); i++) {
        RagDoll& ragDoll = ragDolls[i];
//        if () {
        // TODO: delete
//            i--;
//        }
        ragDoll.Update(GetFrameTime(), terrainSegments);
        ragDoll.Render();
    }
    //   END PIXELATED ==========================
    EndMode2D();
    EndTextureMode();

    // Draw
    //----------------------------------------------------------------------------------
    target.GetTexture().Draw(srcRect, destRect, {0, 0}, 0.0f, WHITE);

    EndDrawing();
    //----------------------------------------------------------------------------------
}

void UpdatePlayerCamera(int width, int height)
{
    // zoom based on mouse wheel
    float wheel = GetMouseWheelMove();
    if (wheel != 0)
    {
        camera.target = GetScreenToWorld2D(GetMousePosition(), camera);

        camera.zoom += wheel * 0.05f;
        if (camera.zoom < 0.05f) {
            camera.zoom = 0.05f;
        } else if (camera.zoom > 2.0f) {
            camera.zoom = 2.0f;
        }
    }

    // smooth camera movement
    float minEffectLength = 10;

    camera.offset = (Vec2){ width/2.0f, height/2.0f };
    Vec2 diff = Vector2Subtract(player->pos, camera.target);
    float diffLength = Vector2Length(diff);

    if (diffLength > minEffectLength)
    {
        camera.target = player->pos - ((diff / diffLength) * minEffectLength);
    }

    // pixelated camera
    theOtherCamera.offset = Vec2(virtualScreenWidth, virtualScreenHeight) / 2.0f;
    theOtherCamera.target = Vec2 {camera.target};
    theOtherCamera.zoom = camera.zoom / virtualRatio;
}

// update terrain method
void UpdateTerrain() {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        terrainEditor.AddPoint(GetScreenToWorld2D(GetMousePosition(), camera));
    }
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
        terrainEditor.points.pop_back();
    }
    if (IsKeyPressed(KEY_BACKSPACE)) {
        terrainEditor.points.clear();
        terrainSegments.clear();
    }
    if (IsKeyPressed(KEY_ENTER)) {
        terrainSegments.clear();
        terrainSegments.reserve(terrainEditor.points.size() - 1);
        for (int i = 0; i < terrainEditor.points.size() - 1; i++) {
            terrainSegments.push_back({ terrainEditor.points[i], terrainEditor.points[i + 1] });
        }
    }
}