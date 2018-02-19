#ifndef THUNDERBOTS_NAVIGATOR_GRAPHFACTORY_H
#define THUNDERBOTS_NAVIGATOR_GRAPHFACTORY_H

// C++ STD Includes
#include <cmath>
#include <functional>

// Boost Includes
#include <boost/optional.hpp>

// Thunderbots Includes
#include <GraphNode.h>
#include <RealNode.h>

template <typename T>
class GraphFactory {
public:

    using Radius = double;
    using Resolution = double;
    using Scale = double;

    // TODO: We may want to replace these with Thunderbots Geometry classes at some point
    class Area {
    public:
        /**
         * Checks if a given point is in this area
         * @param point the point we're checking if is in this area
         * @return true if the given point is in this area, false otherwise
         */
        virtual bool pointInArea(Coordinates point) = 0;

        // TODO: Comment these functions
        virtual Area* clone() const = 0;
    };

    class Circle : public Area {
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
        Circle* clone() const { return new Circle(*this); };
    private:
        Radius radius;
        Coordinates center;
    };

    class Rectangle : public Area {
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
        Rectangle* clone() const { return new Rectangle(*this); };
    private:
        double width, height;
        Coordinates bottom_left_coordinates;
    };


    /**
     * Creates a GraphFactory with default values
     */
    GraphFactory() :
            top_level_graph_scale(1)
    {};

    /**
     * Sets the max scale for the generated graph at a given point
     * @param coordinates the point at which to set the given resolution
     * @param max_scale the max scale this point must be
     */
    void setMaxScaleAtPoint(Coordinates coordinates, double max_scale);

    /**
     * Sets the max scale for the generated graph for a given area
     * @param area the area in which to set the given resolution
     * @param max_scale the max scale for the area
     */
    void setMaxScaleInArea(Area &area, double max_scale);

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

    /**
     * Sets all nodes within the given area on the given graph to the given resolution
     * @param graph_node the graph in which we're setting the min. resolution of an area
     * @param area the area over which the max scale is to be set
     * @param max_scale the maximum scale every node in the given area must be
     */
    void setMinGraphResolutionForArea(GraphNode<T> &graph_node,
                                      Area &area,
                                      Scale max_scale);

    /**
     * Sets the max scale for the node closest to the given point
     * @param graph_node the graph in which we're setting the min. resolution of a point
     * @param coordinates the point we want to set the scale of
     * @param max_scale the maximum scale the node closest to the given point must be
     */
    void setMinGraphResolutionForPoint(GraphNode<T> &graph_node,
                                       Coordinates coordinates,
                                       Scale max_scale);

    // TODO: Better name for this variable?
    // A list of areas with a given max scale
    std::vector<std::pair<Area*, Scale>> min_resolution_areas;

    // TODO: Better name for this variable?
    // A list of points with a given max scale
    std::vector<std::pair<Coordinates, Scale>> min_resolution_points;

    // The length/width of the top level graph
    double top_level_graph_scale;
};

#include <GraphFactory.tpp>

#endif //THUNDERBOTS_NAVIGATOR_GRAPHFACTORY_H
