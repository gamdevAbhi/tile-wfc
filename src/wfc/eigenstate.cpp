#include <wfc/eigenstate.hpp>

wfc::eigenstate::eigenstate(int* id, unsigned char* data, int width, int height, int channels)
{
    this->id = current_free_id;
    
    *id = this->id;

    current_free_id++;

    // 0  + 1  + 2  +  3
    // 4  + -  + -  +  6
    // 5  + -  + -  +  7
    // 8  + 9  + 10 +  11

    int boundaries_size[2] = {width / 4, height / 4};

    this->b_size = 12;
    
    this->boundaries = new Region[b_size];
    this->colors = new Color[b_size];

    for(int i = 0; i < 4; i++)
    {
        this->boundaries[i] = Region();

        this->boundaries[i].start_row = i * boundaries_size[0];
        this->boundaries[i].start_col = 0;
        this->boundaries[i].end_row = this->boundaries[i].start_row + boundaries_size[0];
        this->boundaries[i].end_col = this->boundaries[i].start_col + boundaries_size[1];
    }

    for(int i = 4; i < 6; i++)
    {
        this->boundaries[i] = Region();

        this->boundaries[i].start_row = 0;
        this->boundaries[i].start_col = (i - 3) * boundaries_size[1];
        this->boundaries[i].end_row = this->boundaries[i].start_row + boundaries_size[0];
        this->boundaries[i].end_col = this->boundaries[i].start_col + boundaries_size[1];
    }

    for(int i = 6; i < 8; i++)
    {
        this->boundaries[i] = Region();

        this->boundaries[i].start_row = 3 * boundaries_size[0];
        this->boundaries[i].start_col = (i - 5) * boundaries_size[1];
        this->boundaries[i].end_row = this->boundaries[i].start_row + boundaries_size[0];
        this->boundaries[i].end_col = this->boundaries[i].start_col + boundaries_size[1];
    }

    for(int i = 8; i < 12; i++)
    {
        this->boundaries[i] = Region();

        this->boundaries[i].start_row = (i - 8) * boundaries_size[0];
        this->boundaries[i].start_col = 3 * boundaries_size[1];
        this->boundaries[i].end_row = this->boundaries[i].start_row + boundaries_size[0];
        this->boundaries[i].end_col = this->boundaries[i].start_col + boundaries_size[1];
    }

    int boundaries_pixel_size = boundaries_size[0] * boundaries_size[1];

    for(int i = 0; i < this->b_size; i++)
    {
        long long sumR = 0, sumG = 0, sumB = 0;

        for(int row = boundaries[i].start_row; row < boundaries[i].end_row; row++)
        {
            for(int column = boundaries[i].start_col; column < boundaries[i].end_col; column++)
            {
                int index = (row + (width * column)) * (channels + 1);

                sumR += data[index + 0];  // R
                sumG += data[index + 1];  // G
                sumB += data[index + 2];  // B

                // std::cout << (int)data[index + 0] << " " << (int)data[index + 1] << " " << (int)data[index + 2] << std::endl;
            }
        }

        int avgR = (int)round((double)sumR / boundaries_pixel_size);
        int avgG = (int)round((double)sumG / boundaries_pixel_size);
        int avgB = (int)round((double)sumB / boundaries_pixel_size);

        colors[i] = Color();

        colors[i].r = avgR;
        colors[i].g = avgG;
        colors[i].b = avgB;
    }

    // up color
    up_color.r = (colors[0].r + colors[1].r + colors[2].r + colors[3].r) / 4;
    up_color.g = (colors[0].g + colors[1].g + colors[2].g + colors[3].g) /4;
    up_color.b = (colors[0].b + colors[1].b + colors[2].b + colors[3].b) / 4;
    // left color
    left_color.r = (colors[0].r + colors[4].r + colors[5].r + colors[8].r) / 4;
    left_color.g = (colors[0].g + colors[4].g + colors[5].g + colors[8].g) / 4;
    left_color.b = (colors[0].b + colors[4].b + colors[5].b + colors[8].b) / 4;
    // right color
    right_color.r = (colors[3].r + colors[6].r + colors[7].r + colors[11].r) / 4;
    right_color.g = (colors[3].g + colors[6].g + colors[7].g + colors[11].g) / 4;
    right_color.b = (colors[3].b + colors[6].b + colors[7].b + colors[11].b) / 4;
    // down color
    down_color.r = (colors[8].r + colors[9].r + colors[10].r + colors[11].r) / 4;
    down_color.g = (colors[8].g + colors[9].g + colors[10].g + colors[11].g) / 4;
    down_color.b = (colors[8].b + colors[9].b + colors[10].b + colors[11].b) / 4;
}

wfc::Color wfc::eigenstate::get_color(int index)
{
    return colors[index];
}

wfc::Color wfc::eigenstate::get_up_color()
{
    return up_color;
}

wfc::Color wfc::eigenstate::get_left_color()
{
    return left_color;
}

wfc::Color wfc::eigenstate::get_right_color()
{
    return right_color;
}

wfc::Color wfc::eigenstate::get_down_color()
{
    return down_color;
}