#include <stb/stb_image.h>
#include <stb/stb_image_write.h>
#include <raylib/raylib.h>
#include <wfc/tile_wfc.hpp>
#include <iostream>

void generate_eigenstate(wfc::eigenstate* state, const char* path)
{
    int id, width, height, channels;

    unsigned char* data = stbi_load(path, &width, &height, &channels, 4);

    *state = wfc::eigenstate(&id, data, width, height, channels);
}

int main()
{
    InitWindow(600, 600, "TILE WFC");

    wfc::eigenstate* states = (wfc::eigenstate*)malloc(sizeof(wfc::eigenstate) * 5);

    generate_eigenstate(states, "../assets/images/1.png");
    generate_eigenstate(states + 1, "../assets/images/2.png");
    generate_eigenstate(states + 2, "../assets/images/3.png");
    generate_eigenstate(states + 3, "../assets/images/4.png");
    generate_eigenstate(states + 4, "../assets/images/5.png");

    wfc::tile_wfc wfc = wfc::tile_wfc(states, 5);
    wfc.generate(4, 4);

    // Image image = { data, width, height, 1, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8};
    
    // Texture2D tex = LoadTextureFromImage(image);

    while(!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        // DrawTexture(tex, 0, 0, WHITE);
        EndDrawing();
    }
    
    CloseWindow();

    return 0;
}