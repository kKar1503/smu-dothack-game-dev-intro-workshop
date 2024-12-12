#include "include/raylib.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/raymath.h"
#include "utils.h"

// ::CONSTANTS
#define COLOR_PLAYER \
  CLITERAL(Color) { 0, 158, 47, 255 } // RAYLIB LIME
#define COLOR_BULLET \
  CLITERAL(Color) { 0, 228, 48, 255 } // Green
#define COLOR_ENEMY \
  CLITERAL(Color) { 190, 33, 55, 255 } // Maroon

// ::STRUCT DECLARATIONS

typedef struct World {
  Vector2 cameraTarget;
} World;

// ::GLOBALS
World* world = 0;

// ::HELPER FUNCTIONS
void InitGame() {
  world = (World*)calloc(1, sizeof(World));
}

// ::MAIN
int main(void) {
  const int screenWidth = 800;
  const int screenHeight = 450;
  InitWindow(screenWidth, screenHeight, "DotHack Game Workshop");
  SetTargetFPS(60);
  SetExitKey(KEY_NULL);
  bool exitWindowRequested = false;
  bool exitWindow = false;

  // ::INITIALISATION
  Texture2D logo = LoadTexture("./resources/Raylib_logo.png");

  InitGame();

  Camera2D camera = {0};
  camera.target = world->cameraTarget;
  camera.offset = (Vector2){screenWidth / 2.0f, screenHeight / 2.0f};
  camera.rotation = 0;
  camera.zoom = 1.0;

  // ::GAME LOOP
  while (!exitWindow) {
    float deltaTime = GetFrameTime();

    { // ::LOOP INPUT

      if (exitWindowRequested) {
        if (IsKeyPressed(KEY_Y) || IsKeyPressed(KEY_ENTER)) exitWindow = true;
        else if (IsKeyPressed(KEY_N) || IsKeyPressed(KEY_ESCAPE)) exitWindowRequested = false;
      } else if (WindowShouldClose() || IsKeyPressed(KEY_ESCAPE)) exitWindowRequested = true;
    }

    { // ::LOOP UPDATE LOGIC
    }

    { // ::LOOP RENDER
      BeginDrawing();
      {
        ClearBackground(RAYWHITE);

        { // ::RENDER CAMERA SPACE
          BeginMode2D(camera);
          render_tile_background(camera.target);

          DrawTexture(logo, -logo.width / 2.0f, -logo.height / 2.0f, (Color){255, 255, 255, 32});

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
        DrawRectangle(0, 0, screenWidth, screenHeight, (Color){0, 0, 0, 128});
        DrawRectangle(0, 100, screenWidth, 200, RAYWHITE);
        DrawText("Are you sure you want to exit program? [Y/N]", 40, 180, 30, BLACK);
      }
      EndDrawing();
    }
  }

  free(world);
  UnloadTexture(logo);
  CloseWindow();
  return 0;
}
