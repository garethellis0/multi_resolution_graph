#ifndef THUNDERBOTS_NAVIGATOR_GRAPHFACTORY_H
#define THUNDERBOTS_NAVIGATOR_GRAPHFACTORY_H

// C++ STD Includes
#include <cmath>

// Thunderbots Includes
#include <GraphNode.h>
#include <RealNode.h>

template <typename T>
class GraphFactory {
public:

    /**
     * Creates a GraphFactory with default values
     */
    GraphFactory() :
            size(1)
    {};

    // TODO: Maybe better comment here?
    /**
     * Sets the minimum resolution for the generated graph at a given point
     * @param coordinates the point at which to set the given resolution
     * @param radius the area around the given point where the given resolution should be applied
     * (Note: If this is 0, then the resolution will still be enforced, just at the given point)
     * @param resolution the minimum resolution this point must be
     */
    void setMinResolutionAtPoint(Coordinates coordinates, double radius, double resolution);

    /**
     * Sets the overall length/width of the graph
     * (ie. this will set the graph to be of `size x size`)
     * @param size the length and width of the graph
     */
    void setGraphSize(double size);

    /**
     * Creates a graph with the currently set parameters
     * @return a new graph with the currently set parameters
     */
    GraphNode<T> createGraph();

private:

    using Radius = double;
    using Resolution = double;

    // TODO: We may want to replace these with Thunderbots Geometry classes at some point
    class Area {
    public:
        /**
         * Checks if a given point is in this area
         * @param point the point we're checking if is in this area
         * @return true if the given point is in this area, false otherwise
         */
        virtual bool pointInArea(Coordinates point) = 0;
    };

    class Circle : Area {
    public:
        // Delete the default constructor
        Circle() = delete;
        Circle(Radius radius, Coordinates center):
                radius(radius),
                center(center)
        {}
        bool pointInArea(Coordinates point) override {
            double distance_to_center = std::sqrt(
                    std::pow(point.x - center.x, 2) + std::pow(point.x - center.x, 2)
            );
            return distance_to_center < radius;
        }
    private:
        Radius radius;
        Coordinates center;
    };

    class Rectangle : Area {
    public:
        // Delete the default constructor
        Rectangle() = delete;
        Rectangle(double width, double height, Coordinates bottom_left_coordinates):
                width(width),
                height(height),
                bottom_left_coordinates(bottom_left_coordinates)
        {}
        bool pointInArea(Coordinates point) override {
            double min_x = bottom_left_coordinates.x;
            double max_x = bottom_left_coordinates.x + width;
            double min_y = bottom_left_coordinates.y;
            double max_y = bottom_left_coordinates.y + height;
            return point.x > min_x && point.x < max_x
                    && point.y > min_y && point.y < max_y;
        }
    private:
        double width, height;
        Coordinates bottom_left_coordinates;
    };

    /**
     * Sets all nodes within the given area on the given graph to the given resolution
     * @param graph_node the graph in which we're setting the min. resolution of an area
     * @param area the area over which the minimum resolution is to be set
     * @param min_resolution the minimum resolution every node in the given area must be
     */
    void changeGraphResolutionForArea(GraphNode<T>& graph_node,
                                      Area area,
                                      Resolution min_resolution);

    // TODO: Better name for this variable?
    // A list of areas with a given minimum resolution
    std::vector<std::pair<Area, Resolution>> min_resolution_areas;

    // The length/width of the top level graph
    double size;
};

#include <GraphFactory.tpp>

#endif //THUNDERBOTS_NAVIGATOR_GRAPHFACTORY_H
