// TODO: Should we add some directories to the folder structure to seperate stuff?
#ifndef THUNDERBOTS_NAVIGATOR_GRAPHFACTORY_IMPL_H
#define THUNDERBOTS_NAVIGATOR_GRAPHFACTORY_IMPL_H

#include <GraphFactory.h>
#include <list>
#include <queue>

template <typename T>
GraphFactory<T>::GraphFactory() :
        top_level_graph_resolution(1),
        top_level_graph_scale(1),
        subnode_resolution(2)
{};

template <typename T>
void GraphFactory<T>::setMaxScaleAtPoint(Coordinates coordinates,
                                         double max_scale) {
    // Save this for when we create the graph
    this->min_resolution_points.emplace_back(
            std::make_pair(coordinates, max_scale)
    );
}

template<typename T>
void GraphFactory<T>::setMaxScaleInArea(Area<T> &area,
                                        double max_scale) {
    Area<T>* cloned_area = area.clone();
    this->min_scale_areas.emplace_back(
            std::make_pair(cloned_area, max_scale)
    );
}

template <typename T>
GraphNode<T> GraphFactory<T>::createGraph() {
    // Create a new Graph
    GraphNode<T> graph_node(top_level_graph_resolution, top_level_graph_scale);

    // Set the resolution to the minimum requested at the given locations
    // in order of decreasing scale
    // (note that if we do not sort in order of decreasing scale first,
    // then we may split nodes in such a way that we end up with greater scale
    // nodes in areas with lesser scale)
    std::sort(min_scale_areas.begin(), min_scale_areas.end(),
              [&](std::pair<Area<T>*,double> p1, std::pair<Area<T>*,double> p2){
                  return p1.second > p2.second;
              });
    for (auto const& area_and_resolution : min_scale_areas){
        // Get the area and resolution from the pair
        Area<T>* area = area_and_resolution.first;
        Scale resolution = area_and_resolution.second;
        std::tie(area, resolution) = area_and_resolution;
        // Set the resolution in the requested area
        setMaxGraphScaleForArea(graph_node, *area, resolution);
    }

    std::sort(min_resolution_points.begin(), min_resolution_points.end(),
              [&](std::pair<Coordinates,double> p1, std::pair<Coordinates,double> p2){
                  return p1.second > p2.second;
              });
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

// TODO: What if this function gets a negative value?
template <typename T>
void GraphFactory<T>::setGraphScale(double size) {
    this->top_level_graph_scale = size;
}

template<typename T>
void GraphFactory<T>::setGraphTopLevelResolution(unsigned int resolution) {
    this->top_level_graph_resolution = resolution;
}

// TODO: Make sure we're unit testing this
template <typename T>
void GraphFactory<T>::setMaxGraphScaleForArea(
        GraphNode<T> &graph_node,
        Area<T> &area,
        GraphFactory::Scale max_scale) {

    // Find the subnodes of the graph in the given area
    std::function<bool(Node<T>&)> area_filter = [&](Node<T> &n) {
        return area.overlapsNode(n);
    };

    // Note: we're using a Queue here to avoid thrashing memory because we
    // constantly add and remove from it when we iteratively split below
    std::queue<RealNode<T>*> nodes_to_split;

    // Get the initial set of nodes to split
    std::vector<RealNode<T>*> initial_nodes_to_split = graph_node.getAllNodesThatPassFilter(area_filter);
    for (auto &node : initial_nodes_to_split){
        nodes_to_split.push(std::move(node));
    }

    // Keep splitting nodes until every node in the given area is of the desired resolution
    while (nodes_to_split.size() > 0) {
        // Get the first node from the queue
        RealNode<T>* current_node = nodes_to_split.front();

        // Remove the node we just got from the queue
        nodes_to_split.pop();

        // Check if the node overlaps the given area and if it's scale is too large
        if (area.overlapsNode(*current_node) &&
            current_node->getScale() >= max_scale){
            // Convert the node to a GraphNode
            Node<T>* newly_split_node = current_node->convertToGraphNode(subnode_resolution);
            // Add all the nodes below the new GraphNode to the queue of nodes to consider splitting
            std::vector<RealNode<T>*> new_nodes_to_split = newly_split_node->getAllSubNodes();
            for (auto &node : new_nodes_to_split){
                nodes_to_split.push(std::move(node));
            }
        }
    }

    return;
}

// TODO: Make sure we're unit testing this
template<typename T>
void GraphFactory<T>::setMinGraphResolutionForPoint(GraphNode<T> &graph_node,
                                                    Coordinates coordinates,
                                                    GraphFactory::Scale max_scale) {
    // TODO: We should be setting layers of resolution more intelligently (doing something like only make a quadtree),
    // TODO: not just setting the resolution of the first children to match
    // TODO: see `setMinGraphResolutionInArea` for an example of this being done well
    // TODO: can we apply DRY with `setMinGraphResolutionInArea`?
    // TODO: Do we even need this function? Is it useful?

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
