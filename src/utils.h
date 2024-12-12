#ifndef UTILS_H
#define UTILS_H
#include "include/raylib.h"
#include <math.h>
#include <stdbool.h>

static inline bool almost_equals(float value, float target, float tolerance) {
  return fabs(value - target) < tolerance;
}

static bool animate_float_to_target(float* value, float target, float delta_t, float rate) {
  *value += (target - *value) * (1.0 - pow(2.0f, -rate * delta_t));
  if (almost_equals(*value, target, 0.001f)) {
    *value = target;
    return true;
  }
  return false;
}

static void animate_v2_to_target(Vector2* value, Vector2 target, float delta_t, float rate) {
  animate_float_to_target(&(value->x), target.x, delta_t, rate);
  animate_float_to_target(&(value->y), target.y, delta_t, rate);
}

// TILE RENDERING FUNCTIONALITY
#define TILE_WIDTH 32
#define HALF_TILE_WIDTH TILE_WIDTH / 2.0
#define COLOR_TILE \
  CLITERAL(Color) { 0, 158, 47, 25 }

static inline int   worldPosToTilePos(float worldPos) { return roundf(worldPos / (float)TILE_WIDTH); }
static inline float tilePosToWorldPos(int tilePos) { return ((float)tilePos * (float)TILE_WIDTH); }

static void render_tile_background(Vector2 referencePoint) {
  int playerTileX = worldPosToTilePos(referencePoint.x);
  int playerTileY = worldPosToTilePos(referencePoint.y);
  int tileRenderRadiusX = 40;
  int tileRenderRadiusY = 30;
  for (int x = playerTileX - tileRenderRadiusX;
       x < playerTileX + tileRenderRadiusX;
       x++) {
    for (int y = playerTileY - tileRenderRadiusY;
         y < playerTileY + tileRenderRadiusY;
         y++) {
      if ((x + (y % 2 == 0)) % 2 == 0) {
        float posX = tilePosToWorldPos(x) - HALF_TILE_WIDTH;
        float posY = tilePosToWorldPos(y) - HALF_TILE_WIDTH;
        DrawRectangle(posX, posY, TILE_WIDTH, TILE_WIDTH, COLOR_TILE);
      }
    }
  }
}

#endif // !UTILS_H
