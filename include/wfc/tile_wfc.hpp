#ifndef WFC_IMPLEMENTATION
#define WFC_IMPLEMENTATION

#include <wfc/eigenstate.hpp>
#include <vector>
#include <map>
#include <random>

#include <iostream>

namespace wfc
{
    class tile_wfc 
    {
        struct States
        {
            std::vector<int> possible_states;
        };

        private:
        protected:
        int e_size;
        std::mt19937 gen;
        std::map<wfc::eigenstate*, int**> eigenstate_map;
        eigenstate* get_eigenstate_at(int i);
        int* get_lowest_possible_grid_element(States** grid, int row, int column);
        void update_possible_grid_element(std::vector<int>* possible_eigenstate_index, eigenstate* neighbour_state, 
        int noise_index,
        int max_allowed_noise);
        void update_neighbours_possible(States** grid, int* results, int row, int column, eigenstate* state, int max_allowed_noise);
        int* calculate_noises(wfc::eigenstate* state1, wfc::eigenstate* state2);
        public:
        tile_wfc(wfc::eigenstate* eigenstates, int e_size);
        int** generate(int row, int column, int max_allowed_noise = 30);
    };
}

#endif