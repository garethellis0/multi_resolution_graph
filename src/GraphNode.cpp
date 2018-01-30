#include <GraphNode.h>

GraphNode::GraphNode(unsigned int resolution, double scale) :
    resolution(resolution),
    scale(std::abs(scale)),
    parent(nullptr)
{
    initSubNodes();
}

// Note: If we give this node a parent, then we cannot give it a scale, because it's scale
// will be decided by the scale of it's parent (ie. only the topmost parent will have a scale)
GraphNode::GraphNode(unsigned int resolution, GraphNode *parent) :
    resolution(resolution),
    parent(parent)
{
    initSubNodes();
}

void GraphNode::initSubNodes() {
    for (int rowIndex = 0; rowIndex < resolution; rowIndex++){
        // Add a new row
        subNodes.emplace_back(std::vector<Node*>{});
        // Populate the row with nodes
        for (int colIndex = 0; colIndex < resolution; colIndex++){
            subNodes[rowIndex].emplace_back(new RealNode(this));
        }
    }
}

boost::optional<RealNode*> GraphNode::getClosestNodeToCoordinates(Coordinates coordinates) {
    // In this case, we just setup the filter so that all nodes pass,
    // so that we just find the closest node with no restrictions
    return this->getClosestNodeToCoordinatesThatPassesFilter(
            coordinates,
            [&](Node& n){
                return true;
            }
    );
}

Coordinates GraphNode::getCoordinates() {
    // If we have our parent, get our coordinates relative to it
    if (parent != nullptr){
        return parent->getCoordinatesOfNode(this);
    }
    // Otherwise we're at the top level graph
    return Coordinates{0,0};
}

Coordinates GraphNode::getCoordinatesOfNode(Node* node) {
    // Look through all the sub-nodes to see if any match the given node
    for (int rowIndex = 0; rowIndex < resolution; rowIndex++){
        std::vector<Node*>& row = subNodes[rowIndex];
        for (int colIndex = 0; colIndex < resolution; colIndex++){
            Node* potentialNode = row[colIndex];
            // Check if this is the node we're looking for
            if (potentialNode == node){
                Coordinates coordinates;
                coordinates.x = this->getCoordinates().x +
                            // Offset into the graph node for this subnode
                            (this->getScale()/this->getResolution()) * colIndex
                            // Add on an extra 1/2 of a subnode so that coordinates are in
                            // the middle of the node, rather then at the edge
                            + (this->getScale()/this->getResolution())/2;
                coordinates.y = this->getCoordinates().y +
                            (this->getScale()/this->getResolution()) * rowIndex
                            + (this->getScale()/this->getResolution())/2;
                return coordinates;
            }
        }
    }

    // We couldn't find the given node
    throw NodeNotFoundException("Given node is not a direct sub-node of this node");
}

int GraphNode::getResolution() {
    return resolution;
}

double GraphNode::getScale() {
    // If we have a parent, our scale is our parents scale divided by our parents resolution
    if (parent != nullptr){
        return parent->getScale()/parent->getResolution();
    }
    // If we have no parent, our scale is 1
    return scale;
}

boost::optional<RealNode*> GraphNode::getClosestNodeToCoordinatesThatPassesFilter(
        Coordinates coordinates,
        const std::function<bool(Node &)> &filter) {
    // First, look in the sub-nodes of this node

    // Get the distances to each sub-node
    std::vector<std::pair<Node*, double>> deltas;
    for (auto& row : subNodes) {
        for (auto node : row){
            // Only append nodes that pass the given filter
            if (filter(*node)){
                double delta = distance(node->getCoordinates(), coordinates);
                deltas.push_back(std::make_pair(node, delta));
            }
        }

    }

    // Return the closest node that passed the filter (if any)
    if (!deltas.empty()){
        Node* closestNode = std::min_element(deltas.begin(), deltas.end(),
                                            [](auto d1, auto d2){ return d1.second < d2.second; }
        )->first;
        return closestNode->getClosestNodeToCoordinatesThatPassesFilter(coordinates, filter);
    }

    // If we couldn't find any appropriate node under this node, look in our parent node (if we have one)
    if (parent != nullptr){
        return parent->getClosestNodeToCoordinatesThatPassesFilter(coordinates, filter);
    }

    // Couldn't find any node above the given coordinates
    return boost::optional<RealNode*>{};
}

std::vector<std::vector<Node *>> GraphNode::getSubNodes() {
    return subNodes;
}

void GraphNode::changeResolutionOfNode(Node *node, unsigned int resolution) {
    for (auto& row : subNodes){
        for (auto& subNode : row){
            if (subNode == node){
                delete subNode;
                subNode = new GraphNode(resolution, this);
                return;
            }
        }
    }

    // We couldn't find the given node
    throw NodeNotFoundException("Given node is not a direct sub-node of this node");
}

void GraphNode::changeResolutionOfClosestNode(Coordinates coordinates,
                                              unsigned int resolution) {
    boost::optional<RealNode*> possibleClosestNode = this->getClosestNodeToCoordinates(coordinates);

    // TODO: What if we can't find any node (should never happen, but.....)
    if (possibleClosestNode){
        RealNode* closestNode = *possibleClosestNode;
        closestNode->convertToGraphNode(resolution);
    }
}

