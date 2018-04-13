#ifndef THUNDERBOTS_NAVIGATOR_GRAPHNODE_IMPL_H
#define THUNDERBOTS_NAVIGATOR_GRAPHNODE_IMPL_H

// TODO: In general, look for places here that could seriously benefit from parallelism

#include "geom/multi_res_graph/GraphNode.h"

template <typename T>
GraphNode<T>::GraphNode(unsigned int resolution, double scale) :
    resolution(resolution),
    scale(std::abs(scale)),
    parent((GraphNode<T>*)NULL),
    have_cached_coordinates(false)
{
    initSubNodes();
}

template <typename T>
// Note: If we give this node a parent, then we cannot give it a scale, because it's scale
// will be decided by the scale of it's parent (ie. only the topmost parent will have a scale)
GraphNode<T>::GraphNode(unsigned int resolution, GraphNode *parent) :
    resolution(resolution),
    parent(parent),
    have_cached_coordinates(false)
{
    initSubNodes();
}

template<typename T>
Node<T>* GraphNode<T>::clone(GraphNode<T>* parent) const {
    return this->cloneAsGraphNode(parent);
}

template<typename T>
GraphNode<T> *GraphNode<T>::cloneAsGraphNode(GraphNode<T> *parent) const {
    // Make a shallow copy of this GraphNode
    GraphNode* new_graph_node = new GraphNode<T>(*this);

    // Set the parent to the one we were given
    new_graph_node->parent = parent;

    // TODO: Phrase better, literal mind-fuck right now
    // Replace all the children in our shallow copy of the GraphNode
    // with deep copies of them
    for (int rowIndex = 0; rowIndex < resolution; rowIndex++){
        for (int colIndex = 0; colIndex < resolution; colIndex++){
            new_graph_node->subNodes[rowIndex][colIndex] =
                    new_graph_node->subNodes[rowIndex][colIndex]->clone(new_graph_node);
        }
    }

    // Return the (now deep copy) of this Node
    return new_graph_node;
}


template<typename T>
GraphNode<T> &GraphNode<T>::operator=(const GraphNode &graph_node) {
    return *this->cloneAsGraphNode(nullptr);
}

template <typename T>
void GraphNode<T>::initSubNodes() {
    for (int rowIndex = 0; rowIndex < resolution; rowIndex++){
        // Add a new row
        subNodes.emplace_back(std::vector<Node<T>*>{});
        // Populate the row with nodes
        for (int colIndex = 0; colIndex < resolution; colIndex++){
            subNodes[rowIndex].emplace_back(new RealNode<T>(this));
        }
    }
}

template <typename T>
boost::optional<RealNode<T>*> GraphNode<T>::getClosestNodeToCoordinates(Coordinates coordinates) {
    // In this case, we just setup the filter so that all nodes pass,
    // so that we just find the closest node with no restrictions
    return this->getClosestNodeToCoordinatesThatPassesFilter(coordinates,
                                                             [&](Node<T> &n) {
                                                                 return true;
                                                             },
                                                             false);
}

template <typename T>
Coordinates GraphNode<T>::getCoordinates() {
    // Check if we have the coordinates of this node cached
    if (have_cached_coordinates){
        return cached_coordinates;
    } else {
        // If we have our parent, get our coordinates relative to it
        if (parent != nullptr){
            cached_coordinates = parent->getCoordinatesOfNode(this);
        } else {
            // Otherwise we're at the top level graph
            cached_coordinates =  Coordinates{0,0};
        }
        have_cached_coordinates = true;
        return cached_coordinates;
    }
}

template <typename T>
Coordinates GraphNode<T>::getCoordinatesOfNode(Node<T>* node) {
    // Look through all the sub-nodes to see if any match the given node
    for (int rowIndex = 0; rowIndex < resolution; rowIndex++){
        std::vector<Node<T>*>& row = subNodes[rowIndex];
        for (int colIndex = 0; colIndex < resolution; colIndex++){
            Node<T>* potentialNode = row[colIndex];
            // Check if this is the node we're looking for
            if (potentialNode == node){
                Coordinates coordinates;
                coordinates.x = this->getCoordinates().x +
                            // Offset into the graph node for this subnode
                            (this->getScale()/this->getResolution()) * colIndex;
                coordinates.y = this->getCoordinates().y +
                            // Offset into the graph node for this subnode
                            (this->getScale()/this->getResolution()) * rowIndex;
                return coordinates;
            }
        }
    }

    // We couldn't find the given node
    throw NodeNotFoundException("Given node is not a direct sub-node of this node");
}

template <typename T>
int GraphNode<T>::getResolution() {
    return resolution;
}

template <typename T>
double GraphNode<T>::getScale() {
    // If we have a parent, our scale is our parents scale divided by our parents resolution
    if (parent != nullptr){
        return parent->getScale()/parent->getResolution();
    }
    // If we have no parent (ie. if this is the top level node), use the stored scale
    return scale;
}

template <typename T>
boost::optional<RealNode<T> *>
GraphNode<T>::getClosestNodeToCoordinatesThatPassesFilter(
        Coordinates coordinates, const std::function<bool(Node<T> &)> &filter,
        bool search_parent) {

    // First, look in the sub-nodes of this node
    // Find the closest node below this one that passes the filter
    boost::optional<RealNode<T>*> closest_node_found = boost::optional<RealNode<T>*>{};
    double distance_to_closest_node_found = -1;
    for (auto& row : subNodes) {
        for (Node<T>* node : row){
            boost::optional<RealNode<T>*> closest_sub_node = node->getClosestNodeToCoordinatesThatPassesFilter(
                    coordinates, filter, false);
            // Check that we found a node that passed the filter
            if (closest_sub_node){
                double distance_to_closest_sub_node = distance((*closest_sub_node)->getCoordinates(), coordinates);
                // If we haven't found any node yet, or if this one is closer
                // the the closest one found so far, save it as the closest
                if (!closest_node_found.is_initialized() ||
                        distance_to_closest_sub_node < distance_to_closest_node_found) {
                    closest_node_found = closest_sub_node;
                    distance_to_closest_node_found = distance_to_closest_sub_node;
                }
            }
        }

    }

    // If we found a node, return it
    if (closest_node_found){
        return closest_node_found;
    }

    // If we couldn't find any appropriate node under this node and we want to,
    // look in our parent node (if we have one)
    if (parent != nullptr && search_parent){
        return parent->getClosestNodeToCoordinatesThatPassesFilter(coordinates,
                                                                   filter,
                                                                   true);
    }

    // Couldn't find any node that passes the given filter
    return boost::optional<RealNode<T>*>{};
}

template<typename T>
std::vector<RealNode<T> *> GraphNode<T>::getAllNodesThatPassFilter(
        const std::function<bool(Node<T> &)> &filter,
        bool parent_must_pass_filter,
        bool search_parent) {

    // TODO: This comment is likely hard to understand
    // Search through our parent (if we were asked to)
    // This will eventually recurse back over this node
    if (search_parent){
        // Check that we actually have a parent
        if (parent == nullptr){
            // If we don't have a parent, we're at the top of the tree, so recurse back down
            return this->getAllNodesThatPassFilter(filter, parent_must_pass_filter, false);
        } else {
            return parent->getAllNodesThatPassFilter(filter, parent_must_pass_filter, true);
        }
    }

    // If we're checking if the parent must also pass the filter, check if this node
    // does before checking its children
    if (parent_must_pass_filter && !filter(*this)){
        // We didn't pass the filter, so none of our children can either
        return {};
    }

    // TODO: Easy performance improvement with by using OpenMP to add parallelism?
    // Search the the sub-nodes of this node
    std::vector<RealNode<T>*> all_matching_nodes;
    for (auto& row : subNodes) {
        for (Node<T>* node : row){
            // Concatenate all nodes found below this one to those already found
            std::vector<RealNode<T>*> matching_nodes = node->getAllNodesThatPassFilter(
                    filter, parent_must_pass_filter, false);
            all_matching_nodes.insert(all_matching_nodes.end(), matching_nodes.begin(), matching_nodes.end());
        }
    }
    return all_matching_nodes;
}

template<typename T>
std::vector<RealNode<T> *> GraphNode<T>::getAllSubNodes() {
    std::vector<RealNode<T>*> all_subnodes;
    // TODO: Easy performance improvement with by using OpenMP to add parallelism?
    for (auto& row : subNodes) {
        for (Node<T>* node : row) {
            // TODO: Better comment?
            // Add the subnodes below this subnode to the list of all subnodes
            std::vector<RealNode<T>*> curr_subnodes = node->getAllSubNodes();
            all_subnodes.insert(all_subnodes.end(), curr_subnodes.begin(), curr_subnodes.end());
        }
    }
    return all_subnodes;
}



template <typename T>
std::vector<std::vector<Node<T> *>> GraphNode<T>::getSubNodes() {
    return subNodes;
}

template <typename T>
Node<T>* GraphNode<T>::changeResolutionOfNode(Node<T> *node,
                                               unsigned int resolution) {
    for (auto& row : subNodes){
        for (auto& subNode : row){
            if (subNode == node){
                // TODO: We should probably be copying over data from the nodes (if it's a RealNode?)
                delete subNode;
                subNode = new GraphNode<T>(resolution, this);
                return subNode;
            }
        }
    }

    // We couldn't find the given node
    throw NodeNotFoundException("Given node is not a direct sub-node of this node");
}

template <typename T>
void GraphNode<T>::changeResolutionOfClosestNode(Coordinates coordinates,
                                              unsigned int resolution) {
    boost::optional<RealNode<T>*> possibleClosestNode = this->getClosestNodeToCoordinates(coordinates);

    // TODO: What if we can't find any node (should never happen, but.....)
    if (possibleClosestNode){
        RealNode<T>* closestNode = *possibleClosestNode;
        closestNode->convertToGraphNode(resolution);
    }
}

template<typename T>
GraphNode<T>::~GraphNode() {

    // Destroy all children
    for (auto& row : subNodes){
        for (auto& subNode : row){
            subNode->~Node();
        }
    }

}

#endif // THUNDERBOTS_NAVIGATOR_GRAPHNODE_IMPL_H
