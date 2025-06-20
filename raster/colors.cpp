#include <raster/colors.hpp>

#include <ncurses.h>

#include <algorithm>


namespace
{

// Number of color levels
constexpr int NUM_LEVELS = 6;
// Color levels. We do not use equally-spaced levels; we follow a sqrt distribution so that we have more colors with
// high brightness.
constexpr std::array<short, NUM_LEVELS> LEVELS = {0, 447, 632, 775, 894, 1000};

// Offset to avoid overwriting ncurses default colors
constexpr short COLOR_ENCODING_OFFSET = 8;
// Offset to avoid overwriting ncurses default color pair
constexpr short PAIR_ENCODING_OFFSET = 1;

/**
 * Convert color value in [0, 1] to the corresponding level, i.e. index of `LEVELS`.
 */
int color_to_level(float color)
{
    // NOTE: because our color levels follow a sqrt distribution, we need to square first
    return std::clamp(static_cast<int>(std::floor(color * color * NUM_LEVELS)), 0, NUM_LEVELS - 1);
}

/**
 * Convert a single sRGB value to linear.
 */
float srgb_to_linear(float value)
{
    if (value <= 0.04045) {
        return value / 12.92;
    } else {
        return std::pow((value + 0.055) / 1.055, 2.4);
    }
}

/**
 * Convert a single linear value to sRGB.
 */
float linear_to_srgb(float value)
{
    if (value <= 0.0031308) {
        return value * 12.92;
    } else {
        return std::pow(value, 1 / 2.4) * 1.055 - 0.055;
    }
}

}  // namespace


namespace raster
{

void init_colors()
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

short rgb_to_color_pair(const Eigen::Array3f& color)
{
    const int r = color_to_level(color(0));
    const int g = color_to_level(color(1));
    const int b = color_to_level(color(2));
    return (r * NUM_LEVELS + g) * NUM_LEVELS + b + PAIR_ENCODING_OFFSET;
}

Eigen::Array3f srgb_to_linear(const Eigen::Array3f& srgb)
{
    return {::srgb_to_linear(srgb(0)), ::srgb_to_linear(srgb(1)), ::srgb_to_linear(srgb(2))};
}

Eigen::Array3f linear_to_srgb(const Eigen::Array3f& linear)
{
    return {::linear_to_srgb(linear(0)), ::linear_to_srgb(linear(1)), ::linear_to_srgb(linear(2))};
}

}  // namespace raster
