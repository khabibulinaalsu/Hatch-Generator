/**
 * @file geometry.h
 * @brief Geometry lib for 2D computational geometry operations
 * @author Alsu Khabibulina
 * @date 2025
 */

#pragma once

#include <array>
#include <ostream>
#include <vector>

namespace geometry
{

/// Epsilon value for floating point comparisons
constexpr double EPS = 1e-7;

struct Vector;

/**
 * @struct Point
 * @brief Represents a 2D point with x and y coordinates
 */
struct Point
{
    double x; ///< X coordinate
    double y; ///< Y coordinate

    /**
     * @brief Adds a vector to this point
     * @param v Vector to add
     * @return New point resulting from the addition
     */
    Point operator+(const Vector &v) const noexcept;
};

/**
 * @struct Vector
 * @brief Represents a 2D vector
 */
struct Vector
{
    double x; ///< X component of the vector
    double y; ///< Y component of the vector

    /**
     * @brief Constructs a vector from components
     * @param x X component
     * @param y Y component
     */
    Vector(double x, double y) noexcept : x(x), y(y)
    {
    }

    /**
     * @brief Constructs a vector from two points
     * @param p1 Starting point
     * @param p2 Ending point
     *
     * Creates vector from p1 to p2 (p2 - p1).
     */
    Vector(const Point &p1, const Point &p2) noexcept : x(p2.x - p1.x), y(p2.y - p1.y)
    {
    }

    /**
     * @brief Multiplies vector by a scalar
     * @param c Scalar multiplier
     * @return Scaled vector
     */
    Vector operator*(double c) const noexcept;
};

/**
 * @struct Line
 * @brief Represents a line in general form: ax + by + c = 0
 */
struct Line
{
    double a; ///< Coefficient for x
    double b; ///< Coefficient for y
    double c; ///< Constant term

    /**
     * @brief Constructs a line through two points
     * @param p1 First point on the line
     * @param p2 Second point on the line
     */
    Line(const Point &p1, const Point &p2) noexcept : a(p1.y - p2.y), b(p2.x - p1.x), c(p1.x * p2.y - p2.x * p1.y)
    {
    }

    /**
     * @brief Constructs a line from a normal vector and a point
     * @param norm Normal vector to the line
     * @param p Point on the line
     */
    Line(const Vector &norm, const Point &p) noexcept : a(norm.x), b(norm.y), c(-norm.x * p.x - norm.y * p.y)
    {
    }
};

/**
 * @struct Segment
 * @brief Represents a line segment between two points
 */
struct Segment
{
    Point a;   ///< First endpoint of the segment
    Point b;   ///< Second endpoint of the segment
    Line line; ///< Line containing this segment

    /**
     * @brief Constructs a segment from two points
     * @param p1 First endpoint
     * @param p2 Second endpoint
     */
    Segment(const Point &p1, const Point &p2) noexcept : a(p1), b(p2), line(p1, p2)
    {
    }
};

/**
 * @struct Rectangle
 * @brief Represents a rectangle defined by four corner points
 *
 * Points should be ordered (e.g., clockwise or counter-clockwise).
 */
struct Rectangle
{
    std::array<Point, 4> points; ///< Corner points of the rectangle

    /**
     * @brief Converts rectangle to its four boundary segments
     * @return Vector containing the four segments of the rectangle
     */
    std::vector<Segment> toSegments() const noexcept;
};

/**
 * @brief Computes cross product of two vectors
 * @param v1 First vector
 * @param v2 Second vector
 * @return Cross product value (v1 x v2)
 */
double crossProduct(const Vector &v1, const Vector &v2) noexcept;

/**
 * @brief Computes dot product of two vectors
 * @param v1 First vector
 * @param v2 Second vector
 * @return Dot product value (v1 * v2)
 */
double dotProduct(const Vector &v1, const Vector &v2) noexcept;

/**
 * @brief Computes squared distance between two points
 * @param p1 First point
 * @param p2 Second point
 * @return Squared Euclidean distance between points
 *
 * More efficient than computing actual distance when only
 * comparisons are needed.
 */
double distance2(const Point &p1, const Point &p2) noexcept;

/**
 * @brief Gets the normal vector of a line
 * @param l Input line
 * @return Normal vector to the line
 *
 * For line ax + by + c = 0, the normal vector is (a, b).
 */
Vector normOf(const Line &l) noexcept;

/**
 * @brief Checks if two lines are the same or parallel
 * @param l1 First line
 * @param l2 Second line
 * @return true if lines are same or parallel, false otherwise
 */
bool isLinesSameOrParallel(const Line &l1, const Line &l2) noexcept;

/**
 * @brief Finds intersection point of two lines
 * @param l1 First line
 * @param l2 Second line
 * @return Intersection point
 *
 * Don't use parallel or same lines
 */
Point linesIntersection(const Line &l1, const Line &l2) noexcept;

/**
 * @brief Checks if a point lies on a segment
 * @param p Point to check
 * @param s Segment to check against
 * @return true if point lies on the segment, false otherwise
 *
 * Uses epsilon comparison for floating point precision.
 */
bool isInSegment(const Point &p, const Segment &s) noexcept;

/**
 * @brief Generates hatch lines for a rectangle
 * @param rect Rectangle to fill with hatch
 * @param angle Angle of hatch lines in degrees
 * @param step Distance between hatch lines
 * @return Vector of hatch segments
 *
 * Creates a series of parallel lines at given angle that intersect
 * with the rectangle, commonly used for shading or cross-hatching.
 */
std::vector<Segment> generateHatch(const Rectangle &rect, double angle, double step) noexcept;

/**
 * @brief Output stream operator for Point
 * @param out Output stream
 * @param p Point to output
 * @return Reference to the output stream
 *
 * Output format: (x y)
 */
std::ostream &operator<<(std::ostream &out, const Point &p) noexcept;

/**
 * @brief Output stream operator for Segment
 * @param out Output stream
 * @param s Segment to output
 * @return Reference to the output stream
 *
 * Output format: (x1 y1) -> (x2 y2)
 */
std::ostream &operator<<(std::ostream &out, const Segment &s) noexcept;

} // namespace geometry
