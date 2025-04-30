#include <raster/app.hpp>

#include <ncurses.h>


int main()
{
    // init ncurses
    initscr();
    cbreak();
    noecho();

    raster::App app;
    app.run();

    // end ncurses
    endwin();

    return EXIT_SUCCESS;
}
