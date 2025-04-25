#include <raster/app.hpp>
#include <raster/ncurses.hpp>

#include <cstdlib>


int main()
{
    raster::init_ncurses();

    raster::App app;
    app.run();

    raster::end_ncurses();
    return EXIT_SUCCESS;
}
