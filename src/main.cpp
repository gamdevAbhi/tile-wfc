#include <stb/stb_image.h>
#include <stb/stb_image_write.h>
#include <raylib/raylib.h>
#include <iostream>

int main()
{
    InitWindow(600, 600, "TILE WFC");
    
    while(!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        EndDrawing();
    }
    
    CloseWindow();

    return 0;
}