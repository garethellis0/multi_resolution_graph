#ifndef THUNDERBOTS_NAVIGATOR_GRAPHNODE_H
#define THUNDERBOTS_NAVIGATOR_GRAPHNODE_H

// TODO: In general, function order in `.cpp` should match that in the relevent `.h` file

#include <Node.h>
#include <stdexcept>
#include <array>
#include <RealNode.h>

class GraphNode : public Node {
public:
    GraphNode();

    /**
     *  See superclass doc comment
     */
    boost::optional<RealNode*> getClosestNodeToCoordinates(Coordinates coordinates) override;

    /**
     *  See superclass doc comment
     */
    boost::optional<RealNode*> getClosestNodeToCoordinatesThatPassesFilter(
            Coordinates coordinates,
            const std::function<bool(Node &)> &filter) override;

    /**
     *  See superclass doc comment
     */
    Coordinates getCoordinates() override;

    /**
     * Gets the coordinates of a given node below this one
     * Throws a NodeNotFoundException if given node is not found below this one
     * @param node the node to get the coordinates of
     * @return
     * - the coordinates of the given node if the the node was found
     * - throws a NodeNotFoundException if the given node was not found
     */
    Coordinates getCoordinatesOfNode(Node* node);

    /**
     * This is thrown when a given node cannot be found beneath this node
     */
    class NodeNotFoundException : public std::runtime_error {
    public:
        explicit NodeNotFoundException(const char* m) : std::runtime_error(m) {}
    };

    // TODO: We call this function a *LOT*, so some caching here could yield big improvements
    // TODO: Does this comment make it clear what the `resolution` is?
    /**
     * Get the resolution for this graph node
     * @return the width/height of each sub-node in this graph node
     */
    double getResolution();

private:
    // TODO: We should *NOT* be using raw pointers here
    // Nodes located below this one
    std::array<std::array<Node*, 2>, 2> subNodes;

    // TODO: This should NOT be a raw pointer
    // The possible parent of this node
    GraphNode* parent;

    /**
     * Initializes all the subNodes to RealNodes with this node as their parent
     * Note that this is used because `std::array` doesn't play nice with constructor initialiser lists
     */
    void initSubNodes();
};


#endif //THUNDERBOTS_NAVIGATOR_GRAPHNODE_H
