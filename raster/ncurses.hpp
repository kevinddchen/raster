#pragma once

#include <ncurses.h>


namespace raster
{

constexpr short COLOR_PAIR_RED = 1;
constexpr short COLOR_PAIR_GREEN = 2;
constexpr short COLOR_PAIR_YELLOW = 3;
constexpr short COLOR_PAIR_BLUE = 4;
constexpr short COLOR_PAIR_MAGENTA = 5;
constexpr short COLOR_PAIR_CYAN = 6;
constexpr short COLOR_PAIR_WHITE = 7;

/**
 * Initialize ncurses.
 */
inline void init_ncurses()
{
    initscr();
    cbreak();
    noecho();

    start_color();
    init_pair(COLOR_PAIR_RED, COLOR_RED, COLOR_BLACK);
    init_pair(COLOR_PAIR_GREEN, COLOR_GREEN, COLOR_BLACK);
    init_pair(COLOR_PAIR_YELLOW, COLOR_YELLOW, COLOR_BLACK);
    init_pair(COLOR_PAIR_BLUE, COLOR_BLUE, COLOR_BLACK);
    init_pair(COLOR_PAIR_MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(COLOR_PAIR_CYAN, COLOR_CYAN, COLOR_BLACK);
    init_pair(COLOR_PAIR_WHITE, COLOR_WHITE, COLOR_BLACK);

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
