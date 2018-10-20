#pragma once

#include "Node.h"

template <typename T>
Circle<T>::Circle(Radius radius, Coordinates center):
    radius(radius),
    center(center)
    {}

// TODO: YOU ARE HERE - Looks like this implementation is buggy! With the new optimiz. circles don't work
template <typename T>
bool Circle<T>::overlapsNode(Node<T>& node) {
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
    std::vector<Coordinates> points = {p1, p2, p3, p4};

    // This circle intersects this node if:
    // - the center of the circle lies within the node
    // - any of the corners of the node are contained in the circle
    // - the edge of the circle intersects the edge of this node

    // Check if center of circle is within the node
    double min_x = node.getCoordinates().x;
    double min_y = node.getCoordinates().y;
    double max_x = node.getCoordinates().x + node.getScale();
    double max_y = node.getCoordinates().y + node.getScale();
    if (center.x > min_x && center.x < max_x &&
        center.y > min_y && center.y < max_y) {
        return true;
    }

    // Check if any of the node corners are within the circle
    for (const Coordinates& point : points){
        double dx = center.x - point.x;
        double dy = center.y - point.y;
        double distance = std::sqrt(std::pow(dx,2) + std::pow(dy,2));
        if (distance < radius){
            return true;
        }
    }

    // Check if the edge of this circle intersects any of the edges
    // of the node
    // Construct a line between every possible pairing of points
    // (ie. all the edges of the node)
    std::vector<std::pair<Coordinates, Coordinates>> node_edges = {
            {p1,p2},
            {p2,p3},
            {p3,p4},
            {p4,p1},
    };
    for (std::pair<Coordinates, Coordinates> edge: node_edges){
        // Check that the line formed by the two points
        // does not intersect the circle
        // Math from here:
        // http://mathworld.wolfram.com/Circle-LineIntersection.html

        // Find the two points that will make up the line
        // (circle is presumed to be at (0,0), so offset the line from the circle)
        Coordinates p1 = edge.first;
        Coordinates p2 = edge.second;
        double x1 = p1.x - center.x;
        double y1 = p1.y - center.y;
        double x2 = p2.x - center.x;
        double y2 = p2.y - center.y;

        double dx = x2 - x1;
        double dy = y2 - y1;
        double dr = std::sqrt(std::pow(dx,2) + std::pow(dy,2));
        double D = x1*y2 - x2*y1;
        double intersection_discriminant =
                std::pow(radius,2)*std::pow(dr,2) - std::pow(D,2);

        // Get at least one of the intersection points
        auto sgn = [&](double x) {
            if (x < 0) { return -1; }
            else return 1;
        };
        Coordinates intersection;
        intersection.x = (D*dy + sgn(dy)*dx * std::sqrt(intersection_discriminant))
                         / std::pow(dr,2);
        intersection.y = (-D*dx + std::abs(dy)*std::sqrt(intersection_discriminant))
                         / std::pow(dr,2);


        // TODO: DRY here
        // Check if either intersection is on the line segment
        // that actually makes up the node edge
        if (intersection.x >= std::min(x1,x2) && intersection.x <= std::max(x1,x2) &&
            intersection.y >= std::min(y1,y2) && intersection.y <= std::max(y1,y2)){
            return true;
        }

    }

    // If none of the above checks passed, then this circle
    // does not overlap the given node
    return false;
}

