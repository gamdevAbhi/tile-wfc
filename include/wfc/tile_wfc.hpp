#ifndef WFC_IMPLEMENTATION
#define WFC_IMPLEMENTATION

#include <string>

namespace wfc
{
    class tile_wfc 
    {
        private:
        protected:
        int* eigenstate;
        int e_size;
        unsigned char** values;
        int v_size;
        public:
        tile_wfc(int* eigenstate, int e_size, unsigned char** values, int v_size);
        void generate(int* grid, int grid_size[2]);
    };
}

#endif