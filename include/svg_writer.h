/**
 * @file svg_writer.h
 * @brief SVG graphics file generator for geometry objects
 * @author Alsu Khabibulina
 * @date 2025
 */

#pragma once

#include <filesystem>
#include <fstream>
#include <unordered_map>
#include <vector>

#include "geometry.h"

namespace svg
{

/**
 * @enum LineFormat
 * @brief Defines different line styles for SVG rendering
 */
enum LineFormat
{
    CONTOUR, ///< Contour lines (thicker, for boundaries)
    HATCH    ///< Hatch lines (thinner, for filling)
};

/**
 * @class SVGWriter
 * @brief Generates SVG files from geometric segments
 *
 * This class creates SVG images containing geometric segments with
 * different formatting options. It handles coordinate scaling and
 * applies different visual styles based on line format.
 */
class SVGWriter
{
  public:
    /**
     * @brief Constructs an SVG writer and creates the output file
     * @param outFile Path to the output SVG file
     * @param w Width of the SVG canvas in pixels
     * @param h Height of the SVG canvas in pixels
     */
    SVGWriter(const std::filesystem::path &outFile, double w, double h);

    /**
     * @brief Destructor - completes SVG file and closes it
     */
    ~SVGWriter();

    /**
     * @brief Adds segments to be drawn with specified format
     * @param segments Vector of segments to draw
     * @param lf Line format to apply to these segments
     */
    void drawSegments(std::vector<geometry::Segment> segments, LineFormat lf) noexcept;

  private:
    std::ofstream outFile;                                                         ///< Output file stream
    std::unordered_map<LineFormat, std::vector<geometry::Segment>> formatSegments; ///< Segments grouped by format
    double width;                                                                  ///< SVG canvas width
    double height;                                                                 ///< SVG canvas height

    /**
     * @brief Renders all segments to the SVG file
     *
     * Calculates bounding box, applies scaling, and writes SVG line elements
     * with appropriate formatting based on line type.
     */
    void draw();
};

} // namespace svg