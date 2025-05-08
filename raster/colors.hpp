#pragma once

#include <ncurses.h>
#include <Eigen/Dense>

#include <algorithm>


namespace
{

// Number of color levels
constexpr int NUM_LEVELS = 6;
// Color levels
constexpr std::array<short, NUM_LEVELS> LEVELS = {0, 200, 400, 600, 800, 1000};

// Offset to avoid overwriting ncurses default colors
constexpr short COLOR_ENCODING_OFFSET = 8;
// Offset to avoid overwriting ncurses default color pair
constexpr short PAIR_ENCODING_OFFSET = 1;

}  // namespace


namespace raster
{

/**
 * Initialize ncurses colors.
 *
 * ncurses supports up to 256 colors. We use a 6-level palette where each RGB value can have 6 levels, for a total of
 * 216 colors. Thus, a color is encoded as an integer according to the formula: `R * 36 + G * 6 + B`. The specific
 * palette is given in `LEVELS`.
 */
inline void init_colors()
{
    start_color();

    // define colors
    for (int r = 0; r < NUM_LEVELS; ++r) {
        for (int g = 0; g < NUM_LEVELS; ++g) {
            for (int b = 0; b < NUM_LEVELS; ++b) {
                const short encoding = (r * NUM_LEVELS + g) * NUM_LEVELS + b;
                const short color_encoding = encoding + COLOR_ENCODING_OFFSET;
                const short pair_encoding = encoding + PAIR_ENCODING_OFFSET;
                init_color(color_encoding, LEVELS[r], LEVELS[g], LEVELS[b]);
                init_pair(pair_encoding, color_encoding, color_encoding);
            }
        }
    }
}

/**
 * Convert color value in [0, 1] to the corresponding level, i.e. index in `LEVELS`.
 */
inline int color_to_level(float color)
{
    return std::clamp(static_cast<int>(std::floor(color * NUM_LEVELS)), 0, NUM_LEVELS - 1);
}


/**
 * Convert RGB value normalized to [0, 1] to the closest ncurses color pair.
 */
inline short rgb_to_color_pair(const Eigen::Vector3f& color)
{
    const int r = color_to_level(color(0));
    const int g = color_to_level(color(1));
    const int b = color_to_level(color(2));
    return (r * NUM_LEVELS + g) * NUM_LEVELS + b + PAIR_ENCODING_OFFSET;
}

}  // namespace raster
