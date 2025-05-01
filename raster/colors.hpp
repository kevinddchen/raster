#pragma once

#include <ncurses.h>
#include <Eigen/Dense>

#include <algorithm>


namespace
{

// Number of color levels
constexpr short NUM_LEVELS = 6;
// Offset to avoid overwriting ncurses default colors
constexpr short COLOR_OFFSET = 8;
// Offset to avoid overwriting ncurses default color pair
constexpr short PAIR_OFFSET = 1;

}  // namespace


namespace raster
{

/**
 * Initialize ncurses colors.
 *
 * ncurses supports up to 256 colors. We use a simple 6-level palette where each RGB value can have 6 levels.
 * Thus, a color is encoded as an integer < 216 according to the formula: `R * 36 + G * 6 + B`.
 */
inline void init_colors()
{
    start_color();

    // color value increments in equal amounts
    constexpr int INTENSITY_INCREMENT = 1000 / (NUM_LEVELS - 1);

    // define colors
    for (int r = 0; r < NUM_LEVELS; ++r) {
        for (int g = 0; g < NUM_LEVELS; ++g) {
            for (int b = 0; b < NUM_LEVELS; ++b) {
                const short encoding = (r * NUM_LEVELS + g) * NUM_LEVELS + b;
                const short color_name = encoding + COLOR_OFFSET;
                const short pair_name = encoding + PAIR_OFFSET;
                init_color(color_name, r * INTENSITY_INCREMENT, g * INTENSITY_INCREMENT, b * INTENSITY_INCREMENT);
                init_pair(pair_name, color_name, color_name);
            }
        }
    }
}

/**
 * Convert RGB value normalized to [0, 1] to the closest ncurses color pair.
 */
inline short rgb_to_color_pair(const Eigen::Vector3f& color)
{
    const int r = std::clamp(static_cast<int>(std::floor(color(0) * NUM_LEVELS)), 0, NUM_LEVELS - 1);
    const int g = std::clamp(static_cast<int>(std::floor(color(1) * NUM_LEVELS)), 0, NUM_LEVELS - 1);
    const int b = std::clamp(static_cast<int>(std::floor(color(2) * NUM_LEVELS)), 0, NUM_LEVELS - 1);
    return (r * NUM_LEVELS + g) * NUM_LEVELS + b + PAIR_OFFSET;
}

}  // namespace raster
