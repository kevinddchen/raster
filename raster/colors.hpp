#pragma once

#include <ncurses.h>


namespace raster
{

/**
 * Initialize ncurses colors.
 */
inline void init_colors()
{
    // offset to avoid overwriting ncurses default colors
    constexpr short CUSTOM_COLOR_OFFSET = 8;

    start_color();

    // define colors
    for (int r = 0; r < 6; ++r) {
        for (int g = 0; g < 6; ++g) {
            for (int b = 0; b < 6; ++b) {
                const short pair_name = r * 36 + g * 6 + b;
                const short color_name = pair_name + CUSTOM_COLOR_OFFSET;
                init_color(color_name, r * 200, g * 200, b * 200);
                init_pair(pair_name, color_name, color_name);
            }
        }
    }
}

}  // namespace raster
