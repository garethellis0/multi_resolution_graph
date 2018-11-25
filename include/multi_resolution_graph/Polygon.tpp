#pragma once

#include "Node.h"

namespace multi_resolution_graph {

template <typename T>
Polygon<T>::Polygon(std::vector<Coordinates> boundary_points) :
boundary_points(boundary_points){};

template <typename T>
bool Polygon<T>::overlapsNode(Node<T>& node) {
    // Get the 4 corner points of this node
    Coordinates p1 = node.getCoordinates();
    Coordinates p2 = {
            p1.x + node.getScale(),
            p1.y
    };
    Coordinates p3 = {
            p1.x + node.getScale(),
            p1.y + node.getScale()
    };
    Coordinates p4 = {
            p1.x,
            p1.y + node.getScale()
    };
    std::vector<Coordinates> node_points = {p1, p2, p3, p4};
    std::vector<std::pair<Coordinates, Coordinates>> node_edges = {
            {p1,p2},
            {p2,p3},
            {p3,p4},
            {p4,p1},
    };

    std::vector<std::pair<Coordinates, Coordinates>> polygon_edges = this->getEdges();

    // This polygon overlaps the node if:
    // 1) An edge of the polygon overlaps an edge of the node
    // OR
    // 2) A vertex of this polygon lies within the node (to catch the case
    // where the entire polygon is within the node)
    // OR
    // 3) A vertex of the node lies within this polygon (to catch the case
    // where the entire node is within the polygon)

    // Check if any edge of this polygon intersects any edge of the node
    for (const auto& node_edge : node_edges) {
        for (const auto& polygon_edge : polygon_edges) {
            if (lineSegmentsIntersect(node_edge, polygon_edge)) {
                // If any polygon edge intersects a node edge, we know they
                // overlap, so we can stop here
                return true;
            }
        }
    }

    double max_x = std::max_element(begin(node_points), end(node_points),
            [&](auto p1, auto p2){return p1.x < p2.x;})->x;
    double min_x = std::min_element(begin(node_points), end(node_points),
            [&](auto p1, auto p2){return p1.x < p2.x;})->x;
    double max_y = std::max_element(begin(node_points), end(node_points),
            [&](auto p1, auto p2){return p1.y < p2.y;})->y;
    double min_y = std::min_element(begin(node_points), end(node_points),
            [&](auto p1, auto p2){return p1.y < p2.y;})->y;

    // Check if any vertex of this polygon lies within the node
    for (const auto& point : boundary_points) {
        bool in_x_bounds = point.x >= min_x && point.x <= max_x;
        bool in_y_bounds = point.y >= min_y && point.y <= max_y;
        if (in_x_bounds && in_y_bounds) {
            // If any vertex of this polygon lies within this node, we know
            // they intersect, so we can just stop here
            return true;
        }
    }

    // Check if any vertex of the node lies within this polygon
    for (auto point : node_points) {
        if(this->containsPoint(point)) {
            return true;
        }
    }

    // If we got here, then there is no intersection
    return false;
}

template<typename T>
bool
Polygon<T>::lineSegmentsIntersect(std::pair<Coordinates, Coordinates> line1,
                                  std::pair<Coordinates, Coordinates> line2) {
    // Check for vertical lines
    bool line1_vertical = line1.second.x - line1.first.x == 0;
    bool line2_vertical = line2.second.x - line2.first.x == 0;

    bool lines_intersect;
    if (line1_vertical && line2_vertical) {
        bool same_x_value = line1.first.x == line2.first.x;
        bool y_values_overlap =
                valueInRange(line2.first.y, line1.first.y, line1.second.y) ||
                valueInRange(line2.second.y, line1.first.y, line1.second.y) ||
                valueInRange(line1.first.y, line2.first.y, line2.second.y) ||
                valueInRange(line1.second.y, line2.first.y, line2.second.y);
        lines_intersect = same_x_value && y_values_overlap;
    } else if (line1_vertical) {
        // Compute the potential x and y intersects
        double slope = (line2.second.y - line2.first.y) /
                       (line2.second.x - line2.first.x);
        double y_intersect = line2.first.y +
                             slope * (line1.first.x - line2.first.x);
        double x_intersect = line1.first.x;
        lines_intersect =
                valueInRange(y_intersect, line1.first.y, line1.second.y) &&
                valueInRange(x_intersect, line2.first.x, line2.second.x);
    } else if (line2_vertical) {
        // Compute the potential x and y intersects
        double slope = (line1.second.y - line1.first.y) /
                       (line1.second.x - line1.first.x);
        double y_intersect = line1.first.y +
                             slope * (line2.first.x - line1.first.x);
        double x_intersect = line2.first.x;
        lines_intersect =
                valueInRange(y_intersect, line2.first.y, line2.second.y) &&
                valueInRange(x_intersect, line1.first.x, line1.second.x);
    } else  {
        // Neither line is vertical
        // Compute the intersection of the two lines
        double slope1 = (line1.second.y - line1.first.y) /
                        (line1.second.x - line1.first.x);
        double slope2 = (line2.second.y - line2.first.y) /
                        (line2.second.x - line2.first.x);
        double intercept1 = line1.first.y - line1.first.x * slope1;
        double intercept2 = line2.first.y - line2.first.x * slope2;
        double x_intersection = (intercept2 - intercept1) / (slope1 - slope2);
        bool x_intersection_on_line1 = valueInRange(x_intersection, line1.first.x, line1.second.x);
        bool x_intersection_on_line2 = valueInRange(x_intersection, line2.first.x, line2.second.x);
        lines_intersect = x_intersection_on_line1 && x_intersection_on_line2;
    }

    return lines_intersect;
}

    template<typename T>
std::vector<std::pair<Coordinates, Coordinates>> Polygon<T>::getEdges() {
    std::vector<std::pair<Coordinates, Coordinates>> edges;

    for (uint i = 0; i < boundary_points.size()-1; i++) {
        edges.emplace_back(std::make_pair(boundary_points[i], boundary_points[i+1]));
    }

    // The first and last points are assumed to be connected
    if (edges.size() >= 2) {
        edges.emplace_back(
                std::make_pair(
                        boundary_points[boundary_points.size()-1],
                        boundary_points[0]));
    }

    return edges;
}

template <typename T>
bool Polygon<T>::containsPoint(multi_resolution_graph::Coordinates &point) {
    /*
     * Our strategy for this is to draw a from the point to infinity in the x
     * direction, then check how many edges that line intersects
     *
     * Odd # of edges: point is within polygon
     * Even # of edges: point is not within polygon
     *
     * (https://www.geeksforgeeks.org/how-to-check-if-a-given-point-lies-inside-a-polygon/)
     */
    auto edges = getEdges();

    int num_overlapping_edges = 0;
    for (auto const& edge : edges) {
        // Check if the point overlaps the edge in the y
        if (valueInRange(point.y, edge.first.y, edge.second.y)){
            // Find where our infinite line overlaps the edge in x
            bool edge_is_vertical = edge.first.x - edge.second.x == 0;
            double x_intersect = 0;
            if (edge_is_vertical) {
                x_intersect = edge.first.x;
            } else { // Edge is not vertical
                double slope = (edge.second.y - edge.first.y) /
                        (edge.second.x - edge.first.x);
                x_intersect = edge.first.x + (point.y - edge.first.y) / slope;
            }

            if (x_intersect > point.x) {
                num_overlapping_edges++;
            }
        }
    }

    bool odd_num_of_overlapping_edges = num_overlapping_edges % 2 != 0;
    return odd_num_of_overlapping_edges;
}

}

