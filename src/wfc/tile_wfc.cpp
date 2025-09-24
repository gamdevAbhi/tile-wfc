#include <wfc/tile_wfc.hpp>

wfc::tile_wfc::tile_wfc(wfc::eigenstate *eigenstates, int e_size)
{
    std::random_device rd;
    this->gen = std::mt19937(rd());

    this->e_size = e_size;

    for(int i = 0; i < e_size; i++)
    {
        eigenstate_map.insert({eigenstates + i, new int[e_size]});

        for(int j = 0; j < e_size; j++)
        {
            eigenstate_map[eigenstates + i][j] = calculate_noise(eigenstates + i, eigenstates + j);

            // std::cout << "state : " << eigenstates + i << " neighbour : " << eigenstates + j << 
            // " noise : " << eigenstate_map[eigenstates + i][j] << std::endl;
        }
    }
}

wfc::eigenstate *wfc::tile_wfc::get_eigenstate_at(int i)
{
    if(i >= eigenstate_map.size()) return nullptr;
    
    auto it = eigenstate_map.begin();

    std::advance(it, i);

    return it->first;
}

int* wfc::tile_wfc::get_lowest_possible_grid_element(States **grid, int row, int column)
{
    int lowest_possible_size = INT_MAX;
    int* lowest_possible_results = new int[3]{-1, -1, 0};

    for(int i = 0; i < row; i++)
    {
        for(int j = 0; j < column; j++)
        {
            int size = grid[i][j].possible_states.size();

            if(size == 0 || size > lowest_possible_size) continue;

            lowest_possible_size = size;
            lowest_possible_results[0] = i;
            lowest_possible_results[1] = j;
        }
    }

    lowest_possible_results[2] = lowest_possible_size;

    return lowest_possible_results;
}

void wfc::tile_wfc::update_possible_grid_element(std::vector<int>* possible_eigenstate_index, eigenstate *neighbour_state,
    int max_allowed_noise)
{
    int i = 0;

    while(i < possible_eigenstate_index->size())
    {
        int index = (*possible_eigenstate_index)[i];

        if(eigenstate_map[neighbour_state][index] > max_allowed_noise)
        {
            auto it = possible_eigenstate_index->begin();

            std::advance(it, i);

            possible_eigenstate_index->erase(it);
        }
        else i++;
    }

    // std::cout << "possible_eigenstate_size : " << possible_eigenstate_index->size() << std::endl;
}

void wfc::tile_wfc::update_neighbours_possible(States** grid, int* results, int row, int column, eigenstate* state, int max_allowed_noise)
{
    if(results[0] + 1 < row) update_possible_grid_element(&(grid[results[0] + 1][results[1]].possible_states), state, max_allowed_noise);
    if(results[1] + 1 < column) update_possible_grid_element(&(grid[results[0]][results[1] + 1].possible_states), state, max_allowed_noise);
    if(results[0] - 1 >= 0) update_possible_grid_element(&(grid[results[0] - 1][results[1]].possible_states), state, max_allowed_noise);
    if(results[1] - 1 >= 0) update_possible_grid_element(&(grid[results[0]][results[1] - 1].possible_states), state, max_allowed_noise);
}

int wfc::tile_wfc::calculate_noise(wfc::eigenstate *state1, wfc::eigenstate *state2)
{
    wfc::Color* color1 = state1->get_color();
    wfc::Color* color2 = state2->get_color();

    int noise = 0;

    for(int i = 0; i < 12; i++)
    {
        // std::cout << "color 1 : " << color1[i].r << " , " << color1[i].g << " , " << color1[i].b << std::endl;
        // std::cout << "color 2 : " << color2[i].r << " , " << color2[i].g << " , " << color2[i].b << std::endl;
        
        noise += abs(color1[i].r - color2[i].r) + abs(color1[i].g - color2[i].g) + abs(color1[i].b - color2[i].b);
    }

    return noise / 12;
}

int** wfc::tile_wfc::generate(int row, int column, int max_allowed_noise)
{
    int grid_size = row * column;

    States** grid = new States*[row];
    int** final_grid = (int**)malloc(row * sizeof(int*));

    for(int i = 0; i < row; i++)
    {
        grid[i] = new States[column];
        final_grid[i] = (int*)malloc(column * sizeof(int));

        for(int j = 0; j < column; j++)
        {
            for(int x = 0; x < e_size; x++) grid[i][j].possible_states.push_back(x);
        }
    }

    // std::cout << "grid and final grid init" << std::endl;
    
    {
        int* results = new int[3];

        results[0] = gen() % row;
        results[1] = gen() % column;
        results[2] = grid[results[0]][results[1]].possible_states.size();
        
        int eigenstate_index = grid[results[0]][results[1]].possible_states[gen() % results[2]];

        eigenstate* state = get_eigenstate_at(eigenstate_index);

        // std::cout << "state address : " << state << std::endl;

        final_grid[results[0]][results[1]] = eigenstate_index;
        update_neighbours_possible(grid, results, row, column, state, max_allowed_noise);

        grid[results[0]][results[1]].possible_states.clear();

        delete[] results;
    }

    for(int i = 1; i < grid_size; i++)
    {
        // std::cout << "loop : " << i << std::endl;

        int* results = get_lowest_possible_grid_element(grid, row, column);

        // std::cout << "results : [" << results[0] << ", " << results[1] << "], size : " << results[2] << std::endl;

        int eigenstate_index = grid[results[0]][results[1]].possible_states[gen() % results[2]];

        // std::cout << "eigenstate index : " << eigenstate_index << std::endl;

        eigenstate* state = get_eigenstate_at(eigenstate_index);

        // std::cout << "state address : " << state << std::endl;

        final_grid[results[0]][results[1]] = eigenstate_index;

        // std::cout << "value : " << final_grid[results[0]][results[1]] << std::endl;

        grid[results[0]][results[1]].possible_states.clear();

        update_neighbours_possible(grid, results, row, column, state, max_allowed_noise);

        delete[] results;
    }

    for(int i = 0; i < row; i++)
    {
        delete[] grid[i];
    }

    delete[] grid;

    return final_grid;
}
