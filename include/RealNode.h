#ifndef THUNDERBOTS_NAVIGATOR_REALNODE_H
#define THUNDERBOTS_NAVIGATOR_REALNODE_H

#include <Node.h>
#include <GraphNode.h>

// TODO: Some basic tests (not a very complex class)
class RealNode : public Node {
public:
    /**
     * Default constructor is deleted to force the caller to pass in a parent node
     * (this is basically equivalent to declaring the default constructor private)
     */
    RealNode() = delete;

    /**
     * Construct a RealNode with a given parent node
     * @param parent the parent graph node
     */
    explicit RealNode(GraphNode& parent);

    /**
     *  See superclass doc comment
     */
    RealNode& getClosestNodeToCoordinates(Coordinates coordinates) override;

    /**
     *  See superclass doc comment
     */
    boost::optional<RealNode&> getClosestNodeAboveCoordinates(Coordinates coordinates) override;

    /**
     *  See superclass doc comment
     */
    Coordinates getCoordinates() override;

    /**
     * Get the neighbouring nodes to this node
     * @return a vector of all neighbouring nodes
     */
    std::vector<RealNode&> getNeighbours();


private:
    // TODO: Better comment here?
    // The parent "graph" node to this node
    GraphNode& parent;
};


#endif //THUNDERBOTS_NAVIGATOR_REALNODE_H
