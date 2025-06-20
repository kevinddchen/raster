#pragma once

#include <Eigen/Dense>


namespace raster
{

/**
 * Initialize ncurses colors.
 *
 * ncurses supports up to 256 colors. We use a 6-level palette where each RGB value can have 6 levels, for a total of
 * 216 colors. Thus, a color is encoded as an integer according to the formula: `R * 36 + G * 6 + B`. The specific
 * palette is given in `LEVELS`.
 */
void init_colors();

/**
 * Convert RGB value normalized to [0, 1] to the closest ncurses color pair.
 */
short rgb_to_color_pair(const Eigen::Array3f& color);

/**
 * Convert from sRGB color space to linear color space.
 */
Eigen::Array3f srgb_to_linear(const Eigen::Array3f& srgb);

/**
 * Convert from linear color space to sRGB color space.
 */
Eigen::Array3f linear_to_srgb(const Eigen::Array3f& linear);

}  // namespace raster
