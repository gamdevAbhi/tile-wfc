#include <stb/stb_image.h>
#include <stb/stb_image_write.h>
#include <raylib/raylib.h>
#include <wfc/eigenstate.hpp>
#include <iostream>

int main()
{
    InitWindow(600, 600, "TILE WFC");

    int id, width, height, channels;

    unsigned char* data = stbi_load("../assets/images/Wallpaper 3.jpg", &width, &height, &channels, 4);

    if(!data)
    {
        return -1;
    }

    wfc::eigenstate state = wfc::eigenstate(&id, data, width, height, channels);

    Image image = { data, width, height, 1, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8};
    
    Texture2D tex = LoadTextureFromImage(image);

    while(!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTexture(tex, 0, 0, WHITE);
        EndDrawing();
    }
    
    CloseWindow();

    return 0;
}