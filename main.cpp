#include <Includes.h>
#include "src/Player.h"

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
int screenWidth = 1920 / 2;
int screenHeight = 1080 / 2;
std::shared_ptr<Player> player;

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

    [](){std::cout << "helloge testing" << std::endl; }();
    std::vector<int> v = {1, 2, 3};
    for (int n : v | reverse | take(2)) {
        std::cout << n << '\n';
    }

    // Initialization
    //--------------------------------------------------------------------------------------
    raylib::Window window(screenWidth, screenHeight, "raylib-cpp [core] example - basic window");
    player = std::make_shared<Player>();

    SetTargetFPS(60);   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

//    std::vector<int> v = {1, 2, 3, 4, 5};
//    for (int n : v | std::views::take(3)) {
//        printf("%i\n", n);
//    }

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
    //user input
    float speed = 5.0f;

    if (IsKeyDown(KEY_W)) {
        player->pos.y -= speed;
    }
    if (IsKeyDown(KEY_S)) {
        player->pos.y += speed;
    }
    if (IsKeyDown(KEY_A)) {
        player->pos.x -= speed;
    }
    if (IsKeyDown(KEY_D)) {
        player->pos.x += speed;
    }
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

    player->Render();

    DrawText("helloge business", 190, 200, 20, LIGHTGRAY);

    EndDrawing();
    //----------------------------------------------------------------------------------
}