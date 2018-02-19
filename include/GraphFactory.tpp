#ifndef THUNDERBOTS_NAVIGATOR_GRAPHFACTORY_IMPL_H
#define THUNDERBOTS_NAVIGATOR_GRAPHFACTORY_IMPL_H

#include <GraphFactory.h>

template <typename T>
void GraphFactory<T>::setMaxScaleAtPoint(Coordinates coordinates,
                                         double max_scale) {
    // Save this for when we create the graph
    this->min_resolution_points.emplace_back(
            std::make_pair(coordinates, max_scale)
    );
}

template<typename T>
void GraphFactory<T>::setMaxScaleInArea(GraphFactory::Area &area,
                                        double max_scale) {
    Area* cloned_area = area.clone();
    this->min_resolution_areas.emplace_back(
            std::make_pair(cloned_area, max_scale)
    );
}

template <typename T>
GraphNode<T> GraphFactory<T>::createGraph() {
    // Create a new Graph
    GraphNode<T> graph_node(2, top_level_graph_scale);

    // Set the resolution to the minimum requested at the given locations
    for (auto const& area_and_resolution : min_resolution_areas){
        // Get the area and resolution from the pair
        Area* area = area_and_resolution.first;
        Scale resolution = area_and_resolution.second;
        std::tie(area, resolution) = area_and_resolution;
        // Set the resolution in the requested area
        setMinGraphResolutionForArea(graph_node, *area, resolution);
    }
    for (auto const& point_and_resolution : min_resolution_points){
        // Get the area and resolution from the pair
        Coordinates coordinates = point_and_resolution.first;
        Scale resolution = point_and_resolution.second;
        std::tie(coordinates, resolution) = point_and_resolution;
        // Set the resolution in the requested area
        setMinGraphResolutionForPoint(graph_node, coordinates, resolution);
    }

    // Return the graph, setup as requested
    return graph_node;
}

template <typename T>
void GraphFactory<T>::setGraphSize(double size) {
    this->top_level_graph_scale = size;
}

template <typename T>
void GraphFactory<T>::setMinGraphResolutionForArea(
        GraphNode<T> &graph_node,
        GraphFactory::Area &area,
        GraphFactory::Scale max_scale) {
    // TODO: We should be setting layers of resolution more intelligently (doing something like only make a quadtree),
    // TODO: not just setting the resolution of the first children to match

    // Find the subnodes of the graph in the given area
    std::function<bool(Node<T>&)> area_filter = [&](Node<T> &n) {
        return area.pointInArea(n.getCoordinates());
    };
    std::vector<RealNode<T>*> matching_nodes = graph_node.getAllNodesThatPassFilter(area_filter);

    for (RealNode<T>*& node : matching_nodes){
        // If the node isn't already greater then the requested resolution, increase it
        if (node->getScale() > max_scale) {
            // Choose a high enough resolution that the scale is
            // equal to or greater then the requested scale
            unsigned int new_resolution = std::ceil(node->getScale() / max_scale);
            node->convertToGraphNode(new_resolution);
        }
    }

    // TODO: YOU ARE HERE - NEED TO TEST THIS FUNCTION
}

template<typename T>
void GraphFactory<T>::setMinGraphResolutionForPoint(GraphNode<T> &graph_node,
                                                    Coordinates coordinates,
                                                    GraphFactory::Scale max_scale) {
    // TODO: We should be setting layers of resolution more intelligently (doing something like only make a quadtree),
    // TODO: not just setting the resolution of the first children to match

    // Find the closest node to the given coordinates
    boost::optional<RealNode<T>*> possible_closest_node = graph_node.getClosestNodeToCoordinates(coordinates);
    if (possible_closest_node){
        RealNode<T>* closest_node = *possible_closest_node;
        if (closest_node->getScale() > max_scale) {
            // Choose a high enough resolution that the scale is
            // equal to or greater then the requested scale
            unsigned int new_resolution = std::ceil(closest_node->getScale() / max_scale);
            closest_node->convertToGraphNode(new_resolution);
        }
    }

    // TODO: What if `getClosestNodeToCoordinates` doesn't find anything!? (though this should never happen)
    // TODO: We should probably throw an exception here

    // TODO: Need to test this function
}


#endif // THUNDERBOTS_NAVIGATOR_GRAPHFACTORY_IMPL_H
