#include <RealNode.h>

RealNode& RealNode::getClosestNodeToCoordinates(Coordinates coordinates) {
    // We have no sub-nodes, so the closest node is this node
    return *this;
}

std::vector<RealNode&> RealNode::getNeighbours() {
    std::vector<RealNode&> neighbours;
    // Find the closest nodes above, below, and to the right and left of this node
    boost::optional<RealNode&> aboveNode = parent.getClosestNodeAboveCoordinates(this->getCoordinates());
    if (aboveNode)
        neighbours.emplace_back(aboveNode);
    // TODO: Below, left, and right

    return neighbours;
}

RealNode::RealNode(GraphNode& parent):
    parent(parent)
{
}

boost::optional<RealNode &> RealNode::getClosestNodeAboveCoordinates(Coordinates coordinates) {
    // If this node is above the given coordinates, return it
    if (this->getCoordinates().y > coordinates.y){
        return *this;
    }
    // Otherwise, no node was found above the given coordinates
    return boost::optional<RealNode&>{};
}

Coordinates RealNode::getCoordinates() {
    parent.getCoordinatesOfNode(*this);
}
