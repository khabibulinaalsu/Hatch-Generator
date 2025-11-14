/**
 * @file svg_writer.cpp
 * @brief Implementation of SVG file generator
 * @author Alsu Khabibulina
 * @date 2025
 */

#include "svg_writer.h"
#include "geometry.h"

#include <limits>
#include <ostream>
#include <stdexcept>

namespace
{

/**
 * @brief Writes SVG file header with specified dimensions
 * @param out Output stream to write to
 * @param w Width of SVG canvas
 * @param h Height of SVG canvas
 * @return Reference to the output stream
 */
std::ostream &writeSVGHeader(std::ostream &out, size_t w, size_t h)
{
    out << "<svg version=\"1.1\"\n";
    out << "    width=\"" << w << "\" height=\"" << h << "\"\n";
    out << "    xmlns=\"http://www.w3.org/2000/svg\">\n";
    out << '\n';
    return out;
}

/**
 * @brief Writes SVG file closing tag
 * @param out Output stream to write to
 * @return Reference to the output stream
 */
std::ostream &writeSVGTail(std::ostream &out)
{
    out << "\n</svg>\n";
    return out;
}

} // namespace

namespace svg
{

SVGWriter::SVGWriter(const std::filesystem::path &outFilePath, double w, double h)
    : outFile(outFilePath), width(w), height(h)
{
    if (!outFile)
    {
        throw std::runtime_error("Failed to open file: " + outFilePath.string());
    }

    // Create output file and write SVG header
    writeSVGHeader(outFile, w, h);
}

SVGWriter::~SVGWriter()
{
    // Render all segments and close SVG file
    draw();
    writeSVGTail(outFile);
}

void SVGWriter::drawSegments(std::vector<geometry::Segment> segments, LineFormat lf) noexcept
{
    formatSegments[lf].insert(formatSegments[lf].end(), segments.begin(), segments.end());
}

void SVGWriter::draw()
{
    double minX = std::numeric_limits<double>::max();
    double minY = std::numeric_limits<double>::max();
    double maxX = std::numeric_limits<double>::min();
    double maxY = std::numeric_limits<double>::min();

    // Calculate overall bounding box from all segments
    for (const auto &pair : formatSegments)
    {
        for (const auto &segment : pair.second)
        {
            minX = std::min(minX, segment.a.x);
            minX = std::min(minX, segment.b.x);
            minY = std::min(minY, segment.a.y);
            minY = std::min(minY, segment.b.y);

            maxX = std::max(maxX, segment.a.x);
            maxX = std::max(maxX, segment.b.x);
            maxY = std::max(maxY, segment.a.y);
            maxY = std::max(maxY, segment.b.y);
        }
    }

    double w = maxX - minX;
    double h = maxY - minY;

    // Scale to fit canvas while maintaining aspect ratio
    double scale = std::min(width / w, height / h);

    for (const auto &pair : formatSegments)
    {
        double strokeWidth = 1.5;
        // Apply different stroke widths based on line format
        switch (pair.first)
        {
        case CONTOUR:
            strokeWidth = 2;
            break;
        case HATCH:
            strokeWidth = 1;
        }

        // Convert each segment to SVG line element
        for (const auto &segment : pair.second)
        {
            // Scale and translate X coordinates
            double ax = scale * (segment.a.x - minX);
            double bx = scale * (segment.b.x - minX);

            // Scale, translate, and invert Y axis (SVG has inverted Y)
            double ay = scale * (-segment.a.y + maxY);
            double by = scale * (-segment.b.y + maxY);

            outFile << "<line x1=\"" << ax << "\" y1=\"" << ay << "\" x2=\"" << bx << "\" y2=\"" << by
                    << "\" stroke=\"black\" stroke-width=\"" << strokeWidth << "\" />\n";
        }
    }
}

} // namespace svg
