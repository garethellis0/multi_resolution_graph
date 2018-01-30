#ifndef THUNDERBOTS_NAVIGATOR_REALNODE_H
#define THUNDERBOTS_NAVIGATOR_REALNODE_H

#include <Node.h>
#include <GraphNode.h>

class GraphNode;

// TODO: Some basic tests (not a very complex class)
class RealNode : public Node {
public:
    boost::optional<RealNode*> getClosestNodeToCoordinates(Coordinates coordinates) override;

    boost::optional<RealNode*> getClosestNodeToCoordinatesThatPassesFilter(
            Coordinates coordinates,
            const std::function<bool(Node&)>& filter) override;

    Coordinates getCoordinates() override;

    double getScale() override;

    /**
     * Default constructor is deleted to force the caller to pass in a parent node
     * (this is basically equivalent to declaring the default constructor private)
     */
    RealNode() = delete;

    /**
     * Construct a RealNode with a given parent node
     * @param parent the parent graph node
     */
    explicit RealNode(GraphNode* parent);


    // TODO: Should not be using raw pointers here
    /**
     * Get the neighbouring nodes to this node
     * @return a vector of all neighbouring nodes
     */
    std::vector<RealNode*> getNeighbours();

    /**
     * Converts this RealNode into a GraphNode
     * Note: Will invalidate any pointers to this Node
     * @param resolution the resolution of the new GraphNode
     */
    void convertToGraphNode(unsigned int resolution);

private:
    // TODO: This should *NOT* be a raw pointer
    // TODO: Better comment here?
    // The parent "graph" node to this node
    GraphNode* parent;
};


#endif //THUNDERBOTS_NAVIGATOR_REALNODE_H
