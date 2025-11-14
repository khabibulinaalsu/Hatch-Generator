/**
 * @file geometry.cpp
 * @brief Implementation of geometry library functions
 * @author Alsu Khabibulina
 * @date 2025
 */

#include "geometry.h"
#include <cmath>
#include <cstddef>
#include <vector>

namespace geometry
{

Point Point::operator+(const Vector &v) const noexcept
{
    return Point(x + v.x, y + v.y);
}

Vector Vector::operator*(double c) const noexcept
{
    return Vector(x * c, y * c);
}

std::vector<Segment> Rectangle::toSegments() const noexcept
{
    std::vector<Segment> res;
    size_t size = points.size(); // 4

    // Create segment from last point to first point to close the rectangle
    res.emplace_back(points[0], points[size - 1]);

    // Create segments between consecutive points
    for (size_t i = 0; i < size - 1; i++)
    {
        res.emplace_back(points[i], points[i + 1]);
    }
    return res;
}

double crossProduct(const Vector &v1, const Vector &v2) noexcept
{
    return v1.x * v2.y - v1.y * v2.x;
}

double dotProduct(const Vector &v1, const Vector &v2) noexcept
{
    return v1.x * v2.x + v1.y * v2.y;
}

double distance2(const Point &p1, const Point &p2) noexcept
{
    return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}

Vector normOf(const Line &l) noexcept
{
    return Vector(l.a, l.b);
}

bool isLinesSameOrParallel(const Line &l1, const Line &l2) noexcept
{
    Vector norm1 = normOf(l1);
    Vector norm2 = normOf(l2);

    // Lines are parallel if their normal vectors are parallel
    return std::abs(crossProduct(norm1, norm2)) < EPS;
}

Point linesIntersection(const Line &l1, const Line &l2) noexcept
{
    // Calculate determinant
    double d = l1.a * l2.b - l2.a * l1.b;

    // If determinant is zero, lines are parallel (should be checked before calling)
    double dx = l1.b * l2.c - l2.b * l1.c;
    double dy = l1.c * l2.a - l2.c * l1.a;

    return Point(dx / d, dy / d);
}

bool isInSegment(const Point &p, const Segment &s) noexcept
{
    Vector AB(s.a, s.b);
    Vector AP(s.a, p);
    Vector PB(p, s.b);

    // Point is on segment if:
    // 1. Cross product is zero (point lies on the line)
    // 2. Dot products are positive (point is between endpoints)
    return std::abs(crossProduct(AB, AP)) < EPS && dotProduct(AB, AP) > 0 && dotProduct(AB, PB) > 0;
}

std::vector<Segment> generateHatch(const Rectangle &rect, double angle, double step) noexcept
{
    // Convert angle from degrees to radians
    double rad = angle * M_PI / 180.0;

    // Create hatch direction vector
    Vector hatchNorm = Vector(std::sin(rad), std::cos(rad)) * step;
    std::vector<Segment> rectSegments = rect.toSegments();
    Point point = rect.points.front();

    std::vector<Segment> res;

    bool forward = true, isContinue = false, firstIter = true;

    while (forward || isContinue)
    {
        Line hatchLine(hatchNorm, point);
        isContinue = false;

        std::vector<Point> intersections;

        // Find intersections with all rectangle lines
        for (auto &segment : rectSegments)
        {
            if (!isLinesSameOrParallel(segment.line, hatchLine))
            {
                intersections.emplace_back(linesIntersection(segment.line, hatchLine));
            }
        }

        // If hatch line is not parallel to any rectangle segments
        // Otherwise there are 2 intersections
        if (intersections.size() == 4)
        {
            size_t farthest1, farthest2;
            double maxDistance2 = 0;

            for (size_t i = 0; i < intersections.size(); i++)
            {
                for (size_t j = i + 1; j < intersections.size(); j++)
                {
                    double d = distance2(intersections[i], intersections[j]);

                    if (maxDistance2 < d)
                    {
                        maxDistance2 = d;
                        farthest1 = i;
                        farthest2 = j;
                    }
                }
            }

            // Keep 2 middle points
            intersections.erase(intersections.begin() + farthest2);
            intersections.erase(intersections.begin() + farthest1);
        }

        // Check if intersection points are valid (lie on segments)
        for (auto &segment : rectSegments)
        {
            if (isInSegment(intersections.front(), segment))
            {
                isContinue = true;

                res.emplace_back(intersections[0], intersections[1]);
                break;
            }
        }

        // Switch direction when reaching the boundary
        if (!isContinue && forward && !firstIter)
        {
            forward = false;
            hatchNorm = hatchNorm * (-1);
            point = rect.points.front();
            isContinue = true;
        }

        firstIter = false;
        point = point + hatchNorm;
    }

    return res;
}

std::ostream &operator<<(std::ostream &out, const Point &p) noexcept
{
    out << "(" << p.x << ' ' << p.y << ")";
    return out;
}

std::ostream &operator<<(std::ostream &out, const Segment &s) noexcept
{
    out << s.a << " -> " << s.b;
    return out;
}

} // namespace geometry
