/**
 * @file cmdline_parser.h
 * @brief Command line argument parser for application
 * @author Alsu Khabibulina
 * @date 2025
 */

#pragma once

#include "geometry.h"

#include <filesystem>
#include <string>

namespace cmdline_parser
{

/**
 * @brief Argument name for rectangle points
 *
 * Expected format: --points x1 y1 x2 y2 x3 y3 x4 y4
 */
const std::string POINTS_ARG_NAME = "--points";

/**
 * @brief Argument name for hatch angle
 *
 * Expected format: --angle <degrees>
 */
const std::string ANGLE_ARG_NAME = "--angle";

/**
 * @brief Argument name for hatch step
 *
 * Expected format: --step <distance>
 */
const std::string STEP_ARG_NAME = "--step";

/**
 * @brief Argument name for SVG output file
 *
 * Expected format: --svg <filename>
 */
const std::string SVG_ARG_NAME = "--svg";

/**
 * @struct Config
 * @brief Configuration parameters parsed from command line
 */
struct Config
{
    geometry::Rectangle rect;                    ///< Rectangle defined by four points
    double angle;                                ///< Hatch angle in degrees
    double step;                                 ///< Distance between hatch lines
    std::optional<std::filesystem::path> outSVG; ///< Optional SVG output file path
};

/**
 * @brief Parses command line arguments into configuration
 * @param argc Argument count from main()
 * @param argv Argument values from main()
 * @return Config structure with parsed parameters
 * @throw std::invalid_argument if arguments are missing or invalid
 *
 * Supported arguments:
 * - --points x1 y1 x2 y2 x3 y3 x4 y4 (required)
 * - --angle <degrees> (required)
 * - --step <distance> (required)
 * - --svg <filename> (optional)
 */
Config parse(int argc, char *argv[]);

} // namespace cmdline_parser