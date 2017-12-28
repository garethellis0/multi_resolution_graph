#include "GraphNode.h"

GraphNode::GraphNode() :
    parent(nullptr)
{}

RealNode &GraphNode::getClosestNodeToCoordinates(Coordinates coordinates) {
    // TODO
}

boost::optional<RealNode &> GraphNode::getClosestNodeAboveCoordinates(Coordinates coordinates) {
    // First, look in the sub-nodes of this node

    // Get the distances to each sub-node
    std::vector<std::pair<Node, double>> deltas;
    for (auto& row : subNodes) {
        for (auto& node : row){
            // Only append nodes above the given coordinates
            if (node.getCoordinates().y > coordinates.y){
                double delta = distance(node.getCoordinates(), coordinates);
                deltas.emplace_back(std::make_pair(node, delta));
            }
        }
    }
    // Return the closest node of those found (if any)
    if (!deltas.empty()){
        Node closestNode = std::min_element(deltas.begin(), deltas.end(),
                        [](auto d1, auto d2){ return d1.second < d2.second; }
        )->first;
        return closestNode.getClosestNodeAboveCoordinates(coordinates);
    }

    // If we couldn't find any appropriate node under this node, look in our parent node (if we have one)
    if (parent != nullptr){
        return parent->getClosestNodeAboveCoordinates(coordinates);
    }

    // Couldn't find any node above the given coordinates
    return boost::optional<RealNode&>{};
}

Coordinates GraphNode::getCoordinates() {
}

Coordinates GraphNode::getCoordinatesOfNode(Node &node) {
    // Look through all the sub-nodes to see if any match the given node
    for (int rowIndex = 0; rowIndex < 2; rowIndex++){
        std::array<Node, 2>& row = subNodes[rowIndex];
        for (int colIndex = 0; colIndex < 2; colIndex++){
            Node& potentialNode = row[colIndex];
            // Check if this is the node we're looking for
            if (&potentialNode == &node){
                Coordinates coordinates;
                coordinates.x = this->getResolution() * colIndex;
                coordinates.y = this->getResolution() * rowIndex;
                return coordinates;
            }
        }
    }

    // We couldn't find the given node
    throw NodeNotFoundException("Given node is not a direct sub-node of this node");
}

double GraphNode::getResolution() {
    // If we have a parent, our resolution is 1/2 of our parents
    if (parent != nullptr){
        return parent->getResolution()/2;
    }
    // If we have no parent, our resolution is 1
    return 1;
}

