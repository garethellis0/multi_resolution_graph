#include <RealNode.h>

RealNode::RealNode(GraphNode* parent):
        parent(parent)
{
}

boost::optional<RealNode*> RealNode::getClosestNodeToCoordinates(Coordinates coordinates) {
    // We have no sub-nodes, so the closest node is this node
    return this;
}

std::vector<RealNode*> RealNode::getNeighbours() {
    // Find the closest nodes above, below, to the left, and to the right of this node
    std::vector<std::function<bool(Node&)>> filters = {
            [&](Node &n) { return n.getCoordinates().y < this->getCoordinates().y; },
            [&](Node &n) { return n.getCoordinates().y > this->getCoordinates().y; },
            [&](Node &n) { return n.getCoordinates().x < this->getCoordinates().x; },
            [&](Node &n) { return n.getCoordinates().x > this->getCoordinates().x; }
    };

    std::vector<RealNode*> neighbours;
    for (auto& filter : filters){
        boost::optional<RealNode*> node = parent->getClosestNodeToCoordinatesThatPassesFilter(
                this->getCoordinates(), filter
        );
        if (node) {
            neighbours.push_back(*node);
        }
    }

    return neighbours;
}

Coordinates RealNode::getCoordinates() {
    parent->getCoordinatesOfNode(this);
}

boost::optional<RealNode*> RealNode::getClosestNodeToCoordinatesThatPassesFilter(
        Coordinates coordinates,
        const std::function<bool(Node &)> &filter) {
    // TODO: Descriptive comment here
    if (filter(*this)){ return this; }
    return boost::optional<RealNode*>{};
}

double RealNode::getScale() {
    return parent->getResolution()/parent->getScale();
}

void RealNode::convertToGraphNode(unsigned int resolution) {
    parent->increaseResolutionOfNode(this, resolution);
}
