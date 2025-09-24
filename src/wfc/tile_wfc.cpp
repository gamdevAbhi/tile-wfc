#include <wfc/tile_wfc.hpp>

wfc::tile_wfc::tile_wfc(wfc::eigenstate *eigenstates, int e_size)
{
    std::random_device rd;
    this->gen = std::mt19937(rd());

    this->e_size = e_size;

    for(int i = 0; i < e_size; i++)
    {
        eigenstate_map.insert({eigenstates + i, new wfc::Neighbour[e_size]});

        for(int j = 0; j < e_size; j++)
        {
            eigenstate_map[eigenstates + i][j] = {eigenstates + j, calculate_noise(eigenstates + i, eigenstates + j)};
        }
    }
}

int wfc::tile_wfc::calculate_noise(wfc::eigenstate *state1, wfc::eigenstate *state2)
{
    wfc::Color* color1 = state1->get_color();
    wfc::Color* color2 = state2->get_color();

    int noise = 0;

    for(int i = 0; i < 12; i++)
    {
        noise += abs(color1[i].r - color2[i].r) + abs(color1[i].g - color2[i].g) + abs(color1[i].b - color2[i].b);
    }

    return noise;
}

int wfc::tile_wfc::get_lowest_superpos_index(std::vector<std::vector<int>> grid)
{
    int lowest_superpos_index = -1;
    int lowest_superpos;

    for(int i = 0; i < grid.size(); i++)
    {
        if(lowest_superpos_index < 0 || (lowest_superpos > grid[i].size() && grid[i].size() > 1))
        {
            lowest_superpos_index = i;
            lowest_superpos = grid[i].size();
        }
    }

    return lowest_superpos_index;
}

void wfc::tile_wfc::remove_at(std::vector<std::vector<int>>* grid, int index)
{
    auto it = grid->begin();
    
    std::advance(it, index);

    grid->erase(it);
}

void wfc::tile_wfc::update_superpos(std::vector<int> *superpos, wfc::eigenstate *neighbour_state, int max_allowed_noise)
{
    int i = 0;
    
    while(i < superpos->size())
    {
        if(eigenstate_map[neighbour_state][0].noise > max_allowed_noise)
        {
            auto it = superpos->begin();

            std::advance(it, i);

            superpos->erase(it);
        }
        else i++;
    }

    std::cout << "superpos size :" << superpos->size() << std::endl;
}

int wfc::tile_wfc::update_grid_at(std::vector<std::vector<int>>* grid, int index)
{
    std::cout << "gen : " << (*grid)[index].size() << std::endl;
    int superpos_index = gen() % grid[index].size();

    return superpos_index;

    // int eigenstate_index = grid[index][superpos_index];

    // std::cout << "eigenstate_index:" << eigenstate_index << std::endl;

    // auto it = eigenstate_map.begin();
    
    // std::advance(it, eigenstate_index);

    // return it->first;
}

wfc::eigenstate *wfc::tile_wfc::get_eigenstate_at(int index)
{
    auto it = eigenstate_map.begin();
    
    std::advance(it, index);

    return it->first;
}

wfc::eigenstate* wfc::tile_wfc::generate(int row, int column, int max_allowed_noise)
{
    int grid_size = row * column;
    std::vector<std::vector<int>> grid;
    wfc::eigenstate** final_grid = (wfc::eigenstate**) malloc(sizeof(wfc::eigenstate*) * grid_size);

    
    for(int i = 0; i < grid_size; i++)
    {
        std::vector<int> indexes;
        
        for(int x = 0; x < e_size; x++)
        {
            indexes.push_back(x);
        }
        
        grid.push_back(indexes);
    }

    int index = gen() % grid_size;
    int eigenstate_index = update_grid_at(&grid, index);

    grid[index] = std::vector<int>();
    grid[index].push_back(eigenstate_index);

    // remove_at(&grid, index);
    
    for(int i = 0; i < grid_size; i++)
    {
        index = get_lowest_superpos_index(grid);

        std::cout << "index : " << index << std::endl;

        if(index < 0) continue;

        eigenstate_index = update_grid_at(&grid, index);

        std::cout << "eigenstate index : " << eigenstate_index << std::endl;

        // int ind = index + (column * row)

        // index = current_row + (current_column * row)
        // current_row = index - (current_column * row)

        // current_row = 3 - (3 * current_column)
        // current_row = 1 - current_column
        // current_column = (current_row / 3) - 1

        if(index - row >= 0) update_superpos(&grid[index - row], get_eigenstate_at(grid[index][0]), max_allowed_noise);
        if(index + row < grid.size()) update_superpos(&(grid[index + row]), get_eigenstate_at(grid[index][0]), max_allowed_noise);
        if(index - 1 >= 0) update_superpos(&grid[index - 1], get_eigenstate_at(grid[index][0]), max_allowed_noise);
        if(index + 1 < grid.size()) update_superpos(&grid[index + 1], get_eigenstate_at(grid[index][0]), max_allowed_noise);

        // int eigenstate_index = get_eigenstate_at(grid, index);

        grid[index] = std::vector<int>();
        grid[index].push_back(eigenstate_index);

        // std::cout << index << " " << grid.size() << std::endl;
    }

    return *final_grid;
}
