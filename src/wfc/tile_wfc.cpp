#include <wfc/tile_wfc.hpp>

wfc::tile_wfc::tile_wfc(int* eigenstate, int e_size, unsigned char** values, int v_size)
{
    this->eigenstate = eigenstate;
    this->values = values;
    this->e_size = e_size;
    this->v_size = v_size;
}

void wfc::tile_wfc::generate(int* grid, int grid_size[2])
{
    if(grid_size[0] <= 0 || grid_size[1] <= 0) return;

    grid = new int[grid_size[0] * grid_size[1]];

    for(int i = 0; i < grid_size[0]; i++)
    {
        
    }
}
