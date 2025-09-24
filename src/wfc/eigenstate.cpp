#include <wfc/eigenstate.hpp>

wfc::eigenstate::eigenstate(int* id, unsigned char* data, int width, int height, int channels)
{
    this->id = current_free_id;
    
    *id = this->id;

    current_free_id++;

    // 0  + 4  + 5  +  8
    // 1  + -  + -  +  9
    // 2  + -  + -  + 10
    // 3  + 6  + 7  + 11

    int boundaries_size[2] = {width / 4, height / 4};

    this->b_size = 12;
    
    this->boundaries = new Region[b_size];
    this->colors = new Color[b_size];

    for(int i = 0; i < 4; i++)
    {
        this->boundaries[i] = Region();

        this->boundaries[i].start_row = 0;
        this->boundaries[i].start_col = i * boundaries_size[1];
        this->boundaries[i].end_row = this->boundaries[i].start_row + boundaries_size[0];
        this->boundaries[i].end_col = this->boundaries[i].start_col + boundaries_size[1];
    }

    for(int i = 4; i < 6; i++)
    {
        this->boundaries[i] = Region();

        this->boundaries[i].start_row = (i - 3) * boundaries_size[0];
        this->boundaries[i].start_col = 0;
        this->boundaries[i].end_row = this->boundaries[i].start_row + boundaries_size[0];
        this->boundaries[i].end_col = this->boundaries[i].start_col + boundaries_size[1];
    }

    for(int i = 6; i < 8; i++)
    {
        this->boundaries[i] = Region();

        this->boundaries[i].start_row = (i - 5) * boundaries_size[0];
        this->boundaries[i].start_col = 3 * boundaries_size[1];
        this->boundaries[i].end_row = this->boundaries[i].start_row + boundaries_size[0];
        this->boundaries[i].end_col = this->boundaries[i].start_col + boundaries_size[1];
    }

    for(int i = 8; i < 12; i++)
    {
        this->boundaries[i] = Region();

        this->boundaries[i].start_row = 3 * boundaries_size[0];
        this->boundaries[i].start_col = (i - 8) * boundaries_size[1];
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
}

wfc::Color* wfc::eigenstate::get_color()
{
    return colors;
}
