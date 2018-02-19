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
        GraphFactory::Resolution min_scale) {
    // TODO: We should be setting layers of resolution more intelligently (doing something like only make a quadtree),
    // TODO: not just setting the resolution of the first children to match

    // Find the subnodes of the graph in the given area
    std::function<bool(Node<T>&)> area_filter = [&](Node<T> &n) {
        return area.pointInArea(n.getCoordinates());
    };
    std::vector<RealNode<T>*> matching_nodes = graph_node.getAllNodesThatPassFilter(area_filter);

    for (RealNode<T>*& node : matching_nodes){
        // If the node isn't already greater then the requested resolution, increase it
        if (node->getScale() > min_scale) {
            // Choose a high enough resolution that the scale is
            // equal to or greater then the requested scale
            int new_resolution = std::ceil(node->getScale() / min_scale);
            node->convertToGraphNode(new_resolution);
        }
    }

    // TODO: YOU ARE HERE - NEED TO TEST THIS FUNCTION
}

#endif // THUNDERBOTS_NAVIGATOR_GRAPHFACTORY_IMPL_H
