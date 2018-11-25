#pragma once

#include "Area.h"

namespace multi_resolution_graph {
    template<typename T>
    class Polygon : public Area<T> {
    public:
        // Delete the default constructor
        Polygon() = delete;

        // TODO: We should probably throw an exception if we get less than 3 points
        /**
         * Construct a Polygon with given values
         * @param boundary_points A list of points that make up the boundary of
         * the polygon, where the first and last points are assumed to be
         * connected
         */
        Polygon(std::vector<Coordinates> boundary_points);

        bool overlapsNode(Node<T> &node) override;

        std::shared_ptr<Area<T>> clone() const {
            return std::make_shared<Polygon<T>>(*this);
        };

    private:

        /**
         * Gets the edges of the this polygon
         * @return the edges of this polygon as a list of
         * consecutive point pairs
         */
        std::vector<std::pair<Coordinates, Coordinates>> getEdges();

        /**
         * Checks if two given line segments intersect (including endpoints)
         *
         * @param line1 a line segment, defined by two points
         * @param line2 a line segment, defined by two points
         * @return if line1 and line2 intersect
         */
        static bool lineSegmentsIntersect(std::pair<Coordinates, Coordinates> line1, std::pair<Coordinates, Coordinates> line2);


        /**
         * Checks if a given value is within given bounds
         *
         * It does not matter what order the bounds are in
         *
         * @param val
         * @param bound1
         * @param bound2
         *
         * @return if `val` is between `bound1` and `bound2`
         */
        static bool valueInRange(double val, double bound1, double bound2){
            double min = std::min(bound1, bound2);
            double max = std::max(bound1, bound2);
            return (val >= min) && (val <= max);
        };

        /**
         * Checks if a given point lies within this polygon
         *
         * @param point
         * @return if `point` lies within this polygon
         */
        bool containsPoint(Coordinates& point);

        // A list of points that make up the boundary of the polynomial. The
        // first and last points are assumed to be connected
        std::vector<Coordinates> boundary_points;
    };
}

#include "Polygon.tpp"
