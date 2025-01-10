#include "include/raylib.h"
#include "include/raymath.h"
#include "utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ::CONSTANTS
#define COLOR_PLAYER       CLITERAL(Color){0, 158, 47, 255}  // RAYLIB LIME
#define COLOR_BULLET       CLITERAL(Color){0, 228, 48, 255}  // Green
#define COLOR_ENEMY        CLITERAL(Color){190, 33, 55, 255} // Maroon
#define PLAYER_RADIUS      16
#define PLAYER_SPEED       100
#define PLAYER_INIT_HEALTH 100
#define ENEMY_RADIUS       24
#define ENEMY_SPEED        25
#define ENEMY_INIT_HEALTH  100
#define NUM_ENEMIES        3

// ::STRUCT DECLARATIONS
typedef struct Player {
    Vector2 position;
    bool    isValid;
} Player;

typedef struct Enemy {
    Vector2 position;
    bool    isValid;
    int     health;
} Enemy;

typedef struct World {
    Vector2 cameraTarget;
    Player  player;
    Enemy   enemies[NUM_ENEMIES];
} World;

// ::GLOBALS
World *world = 0;

// ::HELPER FUNCTIONS
void InitGame() {
    world = (World *)calloc(1, sizeof(World));

    world->player = (Player){.position = (Vector2){0, 0}, .isValid = true};

    const float ENEMY_SPAWN_RADIUS = 300.0f;
    const float ANGLE_STEP         = (2 * PI) / NUM_ENEMIES;
    for (int iEnemy = 0; iEnemy < NUM_ENEMIES; iEnemy++) {
        float   angle = iEnemy * ANGLE_STEP;
        Vector2 spawnPoint;
        spawnPoint.x =
            world->player.position.x + ENEMY_SPAWN_RADIUS * cos(angle);
        spawnPoint.y =
            world->player.position.y + ENEMY_SPAWN_RADIUS * sin(angle);

        world->enemies[iEnemy] = (Enemy){.position = spawnPoint,
                                         .isValid  = true,
                                         .health   = ENEMY_INIT_HEALTH};
    }
}

// ::MAIN
int main(void) {
    const int screenWidth  = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "DotHack Game Workshop");
    SetTargetFPS(0);
    SetExitKey(KEY_NULL);
    bool exitWindowRequested = false;
    bool exitWindow          = false;

    // ::INITIALISATION
    Texture2D logo = LoadTexture("./resources/Raylib_logo.png");

    InitGame();

    Camera2D camera = {0};
    camera.target   = world->cameraTarget;
    camera.offset   = (Vector2){screenWidth / 2.0f, screenHeight / 2.0f};
    camera.rotation = 0;
    camera.zoom     = 1.0;

    // ::GAME LOOP
    while (!exitWindow) {
        float deltaTime = GetFrameTime();

        { // ::LOOP INPUT

            if (exitWindowRequested) {
                if (IsKeyPressed(KEY_Y) || IsKeyPressed(KEY_ENTER))
                    exitWindow = true;
                else if (IsKeyPressed(KEY_N) || IsKeyPressed(KEY_ESCAPE))
                    exitWindowRequested = false;
            } else if (WindowShouldClose() || IsKeyPressed(KEY_ESCAPE))
                exitWindowRequested = true;

            if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
                world->player.position.x += PLAYER_SPEED * deltaTime;
            if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
                world->player.position.x -= PLAYER_SPEED * deltaTime;
            if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))
                world->player.position.y -= PLAYER_SPEED * deltaTime;
            if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))
                world->player.position.y += PLAYER_SPEED * deltaTime;
        }

        {     // ::LOOP UPDATE LOGIC
            { // Camera
                Vector2 target_pos = world->player.position;
                animate_v2_to_target(&(camera.target), target_pos, deltaTime,
                                     3.0f);
            }

            { // ENEMY UPDATE
                Vector2 target_pos = world->player.position;
                for (int iEnemy = 0; iEnemy < NUM_ENEMIES; iEnemy++) {
                    Enemy *curEnemy = &world->enemies[iEnemy];
                    if (!curEnemy->isValid)
                        continue;

                    // NOTE: METHOD 1: "Rubber band" effect
                    // animate_v2_to_target(&(curEnemy->position), target_pos,
                    // deltaTime, 0.25f);

                    // NOTE: METHOD 2: Uniform speed
                    Vector2 displacement =
                        Vector2Subtract(target_pos, curEnemy->position);
                    displacement = Vector2Scale(
                        displacement, (ENEMY_SPEED * deltaTime)
                                          / Vector2Length(displacement));
                    curEnemy->position =
                        Vector2Add(curEnemy->position, displacement);
                }
            }
        }

        { // ::LOOP RENDER
            BeginDrawing();
            {
                ClearBackground(RAYWHITE);

                { // ::RENDER CAMERA SPACE
                    BeginMode2D(camera);
                    render_tile_background(camera.target);

                    DrawTexture(logo, -logo.width / 2.0f, -logo.height / 2.0f,
                                (Color){255, 255, 255, 32});

                    // ::RENDER PLAYER
                    if (world->player.isValid)
                        DrawCircleV(world->player.position, PLAYER_RADIUS,
                                    COLOR_PLAYER);

                    // ::RENDER ENEMY
                    for (int iEnemy = 0; iEnemy < NUM_ENEMIES; iEnemy++) {
                        if (!world->enemies[iEnemy].isValid)
                            continue;

                        DrawCircleV(world->enemies[iEnemy].position,
                                    ENEMY_RADIUS, COLOR_ENEMY);
                    }

                    EndMode2D();
                }

                { // ::RENDER SCREEN SPACE

#if DEBUG
                    DrawText("This is a debug build", 10, 10, 20, LIGHTGRAY);
#elif !DEBUG
                    DrawText("This is a release build", 10, 10, 20, LIGHTGRAY);
#endif
                }
            }

            // EXIT WINDOW
            if (exitWindowRequested) {
                DrawRectangle(0, 0, screenWidth, screenHeight,
                              (Color){0, 0, 0, 128});
                DrawRectangle(0, 100, screenWidth, 200, RAYWHITE);
                DrawText("Are you sure you want to exit program? [Y/N]", 40,
                         180, 30, BLACK);
            }
            EndDrawing();
        }
    }

    free(world);
    UnloadTexture(logo);
    CloseWindow();
    return 0;
}
