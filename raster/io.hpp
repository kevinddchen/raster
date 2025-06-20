#pragma once

#include <string>
#include <vector>


namespace raster::io
{

/**
 * Read lines from a text file.
 */
std::vector<std::string> read_lines(const char* filename);

/**
 * Split the string `str` using the separator `sep`.
 */
std::vector<std::string> split(const std::string& str, const std::string& sep);

}  // namespace raster::io
