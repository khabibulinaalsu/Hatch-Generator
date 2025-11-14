/**
 * @file cmdline_parser.cpp
 * @brief Implementation of command line argument parser
 * @author Alsu Khabibulina
 * @date 2025
 */

#include "cmdline_parser.h"

#include <filesystem>
#include <optional>
#include <stdexcept>
#include <string>

#include "geometry.h"

namespace cmdline_parser
{

Config parse(int argc, char *argv[])
{
    std::optional<std::array<geometry::Point, 4>> rect;
    std::optional<double> angle;
    std::optional<double> step;
    std::optional<std::filesystem::path> svg;

    // Parse all command line arguments
    for (size_t i = 1; i < argc; ++i)
    {
        std::string currentArg(argv[i]);

        // Handle --points argument
        if (currentArg == POINTS_ARG_NAME)
        {
            if (rect.has_value())
            {
                throw std::invalid_argument(POINTS_ARG_NAME + " argument gets more then once");
            }
            if (i + 8 >= argc)
            {
                throw std::invalid_argument("Expected <double> x 8 after " + POINTS_ARG_NAME);
            }
            rect.emplace();
            for (int j = 0; j < 4; ++j)
            {
                rect.value()[j] = {std::stod(argv[i + j * 2 + 1]), std::stod(argv[i + j * 2 + 2])};
            }
            i += 8;
        }
        // Handle --angle argument
        else if (currentArg == ANGLE_ARG_NAME)
        {
            if (angle.has_value())
            {
                throw std::invalid_argument(ANGLE_ARG_NAME + "argument gets more then once");
            }
            if (i + 1 >= argc)
            {
                throw std::invalid_argument("Expected <double> after " + ANGLE_ARG_NAME);
            }
            std::string nextArg(argv[i + 1]);
            angle.emplace(stod(nextArg));
            i += 1;
        }
        // Handle --step argument
        else if (currentArg == STEP_ARG_NAME)
        {
            if (step.has_value())
            {
                throw std::invalid_argument(STEP_ARG_NAME + " argument gets more then once");
            }
            if (i + 1 >= argc)
            {
                throw std::invalid_argument("Expected <double> after " + STEP_ARG_NAME);
            }
            std::string nextArg(argv[i + 1]);
            step.emplace(stod(nextArg));
            i += 1;
        }
        // Handle --svg argument
        else if (currentArg == SVG_ARG_NAME)
        {
            if (svg.has_value())
            {
                throw std::invalid_argument(SVG_ARG_NAME + " argument gets more then once");
            }
            if (i + 1 >= argc)
            {
                throw std::invalid_argument("Expected <path> after " + SVG_ARG_NAME);
            }
            std::string nextArg(argv[i + 1]);
            svg.emplace(nextArg);
            i += 1;
        }
        // Unknown argument
        else
        {
            throw std::invalid_argument("Unexpected argument: " + currentArg);
        }
    }

    // Validate that required arguments are present
    if (!rect.has_value() || !angle.has_value() || !step.has_value())
    {
        throw std::invalid_argument("Required arg missing");
    }

    return {.rect = {rect.value()}, .angle = angle.value(), .step = step.value(), .outSVG = svg};
}

} // namespace cmdline_parser
