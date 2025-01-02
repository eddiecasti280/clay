#define CLAY_IMPLEMENTATION
#include "clay.h"
#include "renderers/raylib/clay_renderer_raylib.c"

void HandleClayErrors(Clay_ErrorData errorData) {
    printf("%s", errorData.errorText.chars);
}

int main(void) {
    // Clay_Raylib_Initialize(FLAG_WINDOW_RESIZABLE);
    Clay_Raylib_Initialize(800, 600, "Clay UI Demo", FLAG_WINDOW_RESIZABLE);

    uint64_t clayRequiredMemory = Clay_MinMemorySize();
    Clay_Arena clayMemory = (Clay_Arena) {
        .memory = malloc(clayRequiredMemory),
        .capacity = clayRequiredMemory
    };
    Clay_Initialize(clayMemory, (Clay_Dimensions) {
        .width = GetScreenWidth(),
        .height = GetScreenHeight(),
    }, (Clay_ErrorHandler) { HandleClayErrors });

    while (!WindowShouldClose()) {
        // Run once per frame
        Clay_SetLayoutDimensions((Clay_Dimensions) {
            .width = GetScreenWidth(),
            .height = GetScreenHeight()
        });

        Clay_Sizing layoutExpand = {
            .width = CLAY_SIZING_GROW(),
            .height = CLAY_SIZING_GROW()
        };

        Clay_BeginLayout();
        // Build UI here
        CLAY(
            CLAY_RECTANGLE({ .color = {43, 41, 51, 255 } }),
            CLAY_LAYOUT({
                .layoutDirection = CLAY_TOP_TO_BOTTOM,
                .sizing = layoutExpand,
                .padding = { 16, 16 }
            })
        ) {
            // Child elements go inside braces
            CLAY(
                CLAY_ID("Header Bar"),
                CLAY_RECTANGLE({
                    .color = { 90, 90, 90, 255 },
                    .cornerRadius = 8
                }),
                CLAY_LAYOUT({
                    .sizing = layoutExpand
                })
            ) {}
            CLAY(
                CLAY_ID("Lower Content"),
                CLAY_LAYOUT({
                    .sizing = layoutExpand
                })
            ) {}
        }

        Clay_RenderCommandArray renderCommands = Clay_EndLayout();

        BeginDrawing();
        ClearBackground(BLACK);
        Clay_Raylib_Render(renderCommands);
        EndDrawing();
    }
}