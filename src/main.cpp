#include <stb/stb_image.h>
#include <stb/stb_image_write.h>
#include <raylib/raylib.h>
#include <wfc/tile_wfc.hpp>
#include <iostream>

Texture2D generate_eigenstate(wfc::eigenstate* state, const char* path)
{
    int id, width, height, channels;

    unsigned char* data = stbi_load(path, &width, &height, &channels, 4);

    if(!data)
    {
        std::cout << "image not loaded" << std::endl;
    }

    *state = wfc::eigenstate(&id, data, width, height, channels);

    Image image = { data, width, height, 1, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8};
    
    return LoadTextureFromImage(image);
}

int main()
{
    InitWindow(640, 640, "TILE_WFC");

    int row = 40, column = 40, noise = 47;

    // different noises result
    // 12 will cause only connected path to generate or only white path
    // 47 cause overlapping path to generate or only white path
    // 48 will make mix of white and paths. 

    wfc::eigenstate* states = (wfc::eigenstate*)malloc(sizeof(wfc::eigenstate) * 4);
    Texture2D textures[4];

    textures[0] = generate_eigenstate(states, "../assets/images/1.png");
    textures[1] = generate_eigenstate(states + 1, "../assets/images/2.png");
    textures[2] = generate_eigenstate(states + 2, "../assets/images/3.png");
    textures[3] = generate_eigenstate(states + 3, "../assets/images/4.png");

    std::cout << "generated eigenstate" << std::endl;

    wfc::tile_wfc wfc = wfc::tile_wfc(states, 4);

    std::cout << "wfc generated" << std::endl;

    int** grid = wfc.generate(column, row, noise);

    std::cout << "grid generated" << std::endl;

    while(!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        for(int i = 0; i < row; i++)
        {
            int x_pixel = i * 16;

            for(int j = 0; j < column; j++)
            {
                if(grid[j][i] == -1) continue;

                int y_pixel = j * 16;
                
                DrawTexture(textures[grid[j][i]], x_pixel, y_pixel, WHITE);
            }
        }

        // DrawTexture(tex, 0, 0, WHITE);
        EndDrawing();
    }
    
    CloseWindow();

    return 0;
}