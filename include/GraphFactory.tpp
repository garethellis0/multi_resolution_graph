#ifndef THUNDERBOTS_NAVIGATOR_GRAPHFACTORY_IMPL_H
#define THUNDERBOTS_NAVIGATOR_GRAPHFACTORY_IMPL_H

#include <GraphFactory.h>

template <typename T>
void GraphFactory<T>::setMinResolutionAtPoint(Coordinates coordinates,
                                              double radius,
                                              double resolution) {
    Circle area(radius, coordinates);
    // Save this for when we create the graph
    this->min_resolution_areas.emplace_back(
            std::make_pair(area, resolution)
    );
}

template <typename T>
GraphNode<T> GraphFactory::createGraph() {
    // Create a new Graph
    GraphNode<T> graph_node;

    // Set the resolution to the minimum requested at the given locations
    Resolution resolution;
    for (auto const& area : min_resolution_areas){
        changeGraphResolutionForArea(graph_node, area, resolution);
    }

    // Return the graph, setup as requested
    return graph_node;
}

template <typename T>
void GraphFactory<T>::setGraphSize(double size) {
    this->size = size;
}

template <typename T>
void GraphFactory<T>::changeGraphResolutionForArea(
        GraphNode<T> &graph_node,
        GraphFactory::Area area,
        GraphFactory::Resolution min_resolution) {
    // TODO: We should be setting layers of resolution more intelligently (doing something like only make a quadtree),
    // TODO: not just setting the resolution of the first children to match

    // Find the subnodes of the graph in the given area
    // TODO: Waiting on the `getAllNodesThatPassFilter` function in the Node class
}

#endif // THUNDERBOTS_NAVIGATOR_GRAPHFACTORY_IMPL_H
