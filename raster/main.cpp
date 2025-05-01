#include <raster/app.hpp>

#include <ncurses.h>


int main()
{
    // init ncurses
    initscr();
    cbreak();
    noecho();

    raster::App app(100, 100);
    app.run();

    // end ncurses
    endwin();

    return EXIT_SUCCESS;
}
