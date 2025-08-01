#include <stdio.h>
#include <raylib.h>

#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "../nob.h"

#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 900

#define TILE_COUNT 16
#define TILE_DIM SCREEN_WIDTH * 0.10f
#define TILE_PADDING 10

#define BOARD_DIM_IN_TILES 4
#define SELECTED_COUNT_MAX 4

#define WORD_FONT_SIZE TILE_DIM * 0.2f

typedef struct {
  const char *word;
  Rectangle rect;
  bool selected;
} Tile;

typedef struct {
  Tile *items;
  size_t capacity;
  size_t count;
} Tiles;

typedef struct {
  const char *label;
  Rectangle rect;
  Color color;
  Color hovered_color;
} Button;

int main() {

  const char *words[TILE_COUNT] = { "one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen" };
  static_assert(TILE_COUNT == ARRAY_LEN(words), "words length and TILE_COUNT do not match"); 
 
  size_t total_tile_dim = (TILE_COUNT / BOARD_DIM_IN_TILES) * (TILE_DIM + TILE_PADDING);
  size_t original_x = (SCREEN_WIDTH/2) - (total_tile_dim/2);
  size_t x = original_x;
  size_t y = (SCREEN_HEIGHT/2) - (total_tile_dim/2);

  Tiles tiles = {0};
  for (size_t i = 0; i < TILE_COUNT; ++i) {
    Tile t = {0};
    t.word = strdup(words[i]);
    Rectangle r = { .x = x, .y = y, .width = TILE_DIM, .height = TILE_DIM };
    t.rect = r;
    t.selected = false;
    da_append(&tiles, t);
    x += TILE_DIM + TILE_PADDING;
    if (i > 0 && (i+1) % BOARD_DIM_IN_TILES == 0) {
        y += TILE_DIM + TILE_PADDING;
        x = original_x;
    }
  }

  size_t button_row_y = y + TILE_PADDING;

  size_t last_frame_selected_count = 0;

  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Connections");
  
  int sw = MeasureText("Submit", WORD_FONT_SIZE)*1.4;
  Rectangle submit_btn = { .x = SCREEN_WIDTH / 4, .y = button_row_y, .width = sw, .height = WORD_FONT_SIZE * 1.4 };
  Button submit = { .label = "Submit", .rect = submit_btn, .color = DARKGREEN, .hovered_color = LIME };

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(GetColor(0x181818FF));

    Vector2 m = GetMousePosition();
    bool clicked = IsMouseButtonReleased(MOUSE_BUTTON_LEFT);
    size_t selected_count = 0;

    for (size_t i = 0; i < tiles.count; ++i) {
        Tile *t = &tiles.items[i];
        bool hovered = CheckCollisionPointRec(m, t->rect);
        if (hovered) {
          if (clicked) {
            t->selected = !t->selected;
            if (t->selected && last_frame_selected_count == SELECTED_COUNT_MAX)
                t->selected = false;
          }
        }
        if (t->selected) selected_count++;
        Color tile_color = (false || t->selected) ? SKYBLUE : RAYWHITE;
        DrawRectangleRec(t->rect, tile_color);
        int tw = MeasureText(t->word, WORD_FONT_SIZE);
        DrawText(t->word, t->rect.x + TILE_DIM/2 - tw/2, t->rect.y + TILE_DIM/2 - WORD_FONT_SIZE/2, WORD_FONT_SIZE, BLACK);
    }

    last_frame_selected_count = selected_count;
    
    bool submit_hovered = CheckCollisionPointRec(m, submit.rect);  
    Color submit_color = submit_hovered ? submit.hovered_color : submit.color;
    DrawRectangleRec(submit.rect, submit_color);
    int tw = MeasureText(submit.label, WORD_FONT_SIZE);
    DrawText(submit.label, submit.rect.x+submit.rect.width/2-(tw/2), submit.rect.y+submit.rect.height/2-WORD_FONT_SIZE/2, WORD_FONT_SIZE, WHITE);
    if (submit_hovered && clicked) {
      
    }
 
    EndDrawing();
  }


  return 0;
}
