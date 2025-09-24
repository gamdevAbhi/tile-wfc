#ifndef EIGENSTATE_IMPLEMENTATION
#define EIGENSTATE_IMPLEMENTATION

#include <raylib/raylib.h>
#include <cmath>

namespace wfc
{
    struct Region
    {
        int start_row, start_col, end_row, end_col;
    };

    struct Color
    {
        int r, g, b;
    };
    
    class eigenstate
    {
        private:
        inline static int current_free_id = 0;
        protected:
        unsigned char* data;
        Region* boundaries;
        Color* colors;
        Region region;
        int id, width, height, channels, b_size; 
        public:
        eigenstate(int* id, unsigned char* data, int width, int height, int channels);
        Color* get_color();
    };
}

#endif