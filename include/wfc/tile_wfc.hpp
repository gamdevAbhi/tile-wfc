#ifndef WFC_IMPLEMENTATION
#define WFC_IMPLEMENTATION

#include <wfc/eigenstate.hpp>
#include <vector>
#include <map>
#include <random>

#include <iostream>

namespace wfc
{
    struct Neighbour
    {
        wfc::eigenstate* eigenstate;
        int noise;
    };

    class tile_wfc 
    {
        private:
        protected:
        int e_size;
        std::mt19937 gen;
        std::map<wfc::eigenstate*, wfc::Neighbour*> eigenstate_map;

        int calculate_noise(wfc::eigenstate* state1, wfc::eigenstate* state2);
        int get_lowest_superpos_index(std::vector<std::vector<int>> grid);
        void remove_at(std::vector<std::vector<int>>* grid, int index);
        void update_superpos(std::vector<int>* superpos, wfc::eigenstate* neighbour_state, int max_allowed_noise);
        int update_grid_at(std::vector<std::vector<int>>* grid, int index);
        wfc::eigenstate* get_eigenstate_at(int index);
        public:
        tile_wfc(wfc::eigenstate* eigenstates, int e_size);
        wfc::eigenstate* generate(int row, int column, int max_allowed_noise = 30);
    };
}

#endif