#pragma once

#include <ncurses.h>


namespace raster
{

/**
 * Initialize ncurses.
 */
inline void init_ncurses()
{
    initscr();
    cbreak();
    noecho();

    curs_set(0);  // hide cursor
}

/**
 * End ncurses.
 */
inline void end_ncurses()
{
    endwin();
}

}  // namespace raster
