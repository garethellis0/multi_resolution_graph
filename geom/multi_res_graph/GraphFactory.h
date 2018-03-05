#ifndef THUNDERBOTS_NAVIGATOR_GRAPHFACTORY_H
#define THUNDERBOTS_NAVIGATOR_GRAPHFACTORY_H

// C++ STD Includes
#include <cmath>
#include <functional>

// Boost Includes
#include <boost/optional.hpp>

// Thunderbots Includes
#include "GraphNode.h"
#include "RealNode.h"
#include "Area.h"

template <typename T>
class GraphFactory {
public:
    using Resolution = double;
    using Scale = double;

    /**
     * Creates a GraphFactory with default values
     */
    GraphFactory();

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
    void setMaxScaleInArea(Area<T> &area, double max_scale);

    /**
     * Sets the overall length/width of the graph
     * (ie. this will set the graph to be of `size x size`)
     * @param size the length and width of the graph
     */
    void setGraphScale(double size);

    /**
     * Sets the top level resolution for the graph
     * (width/height in # of nodes)
     * @param resolution the desired resolution for the top level of the graph
     */
    void setGraphTopLevelResolution(unsigned int resolution);

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
    void setMaxGraphScaleForArea(GraphNode<T> &graph_node,
                                 Area<T> &area,
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
    std::vector<std::pair<Area<T>*, Scale>> min_scale_areas;

    // TODO: Better name for this variable?
    // A list of points with a given max scale
    std::vector<std::pair<Coordinates, Scale>> min_resolution_points;

    // The length/width of the top level graph
    // (in terms of some unit of distance)
    double top_level_graph_scale;

    // The length/width of the top level of the graph
    // (in terms of # of nodes)
    unsigned int top_level_graph_resolution;

    // TODO: Better comment?
    // TODO: Setter function for this
    // The factor by which we subdivide nodes to build up the tree structure
    unsigned int subnode_resolution;
};

#include "GraphFactory.tpp"

#endif //THUNDERBOTS_NAVIGATOR_GRAPHFACTORY_H
