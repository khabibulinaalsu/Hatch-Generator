
/**
 * @file main.cpp
 * @brief Main application
 * @mainpage Hatch Generator
 *
 * @section overview Overview
 * This application generates hatch patterns for given rectangle and can output
 * the results both to console and SVG files.
 *
 * @section usage Usage
 * The program accepts command line arguments to define a rectangle, hatch
 * parameters, and optional SVG output:
 * @code
 * ./hatch_generator --points x1 y1 x2 y2 x3 y3 x4 y4 --angle <degrees> --step <distance> [--svg <filename>]
 * @endcode
 * @author Alsu Khabibulina
 * @date 2025
 */

#include <exception>
#include <iostream>

#include "cmdline_parser.h"
#include "geometry.h"
#include "svg_writer.h"

/**
 * @brief Main function
 * @param argc Number of command line arguments
 * @param argv Array of command line argument strings
 * @return Exit status (0 for success, 1 for error)
 *
 * @par Program Flow:
 * 1. Parse command line arguments
 * 2. Generate hatch pattern for the specified rectangle
 * 3. Output hatch segments to console
 * 4. Optionally create SVG file with visualization
 */
int main(int argc, char **argv)
{
    cmdline_parser::Config input;
    try
    {
        input = cmdline_parser::parse(argc, argv);
    }
    catch (const std::exception &e)
    {
        std::cout << e.what() << '\n';
        return 1;
    }

    auto hatch = geometry::generateHatch(input.rect, input.angle, input.step);

    for (auto &segment : hatch)
    {
        std::cout << "Line: " << segment << std::endl;
    }

    if (input.outSVG.has_value())
        try
        {
            svg::SVGWriter writer(input.outSVG.value(), 400, 400);
            writer.drawSegments(hatch, svg::HATCH);
            writer.drawSegments(input.rect.toSegments(), svg::CONTOUR);
        }
        catch (const std::exception &e)
        {
            std::cout << "Failed to write svg file: " << input.outSVG.value() << ' ' << e.what() << '\n';
        }

    return 0;
}