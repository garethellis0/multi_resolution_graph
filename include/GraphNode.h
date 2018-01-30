// TODO: In general, redo header guards to "match" the rest of the repo
#ifndef THUNDERBOTS_NAVIGATOR_GRAPHNODE_H
#define THUNDERBOTS_NAVIGATOR_GRAPHNODE_H

// TODO: In general, function order in `.cpp` should match that in the relevent `.h` file

#include <Node.h>
#include <stdexcept>
#include <array>
#include <RealNode.h>

class GraphNode : public Node {
public:

    boost::optional<RealNode*> getClosestNodeToCoordinates(Coordinates coordinates) override;

    boost::optional<RealNode*> getClosestNodeToCoordinatesThatPassesFilter(
            Coordinates coordinates,
            const std::function<bool(Node &)> &filter) override;

    Coordinates getCoordinates() override;

    double getScale() override;

    /**
     * Creates a GraphNode with default values
     */
    GraphNode() : GraphNode(2) {};

    /**
     * Creates a GraphNode with a given resolution
     * @param resolution the length/width of this graph node in units of number of nodes
     * (ie. this graph node will contain `resolution^2` nodes)
     */
    explicit GraphNode(unsigned int resolution) : GraphNode(resolution, 1) {};

    /**
     * Create a GraphNode with a given resolution and scale
     * @param resolution the length/width of this graph node in units of number of nodes
     * (ie. this graph node will contain `resolution^2` nodes)
     * @param scale the length/width of the sides of this graph node.
     * Note: The absolute value of this will be used (as sadly there are no unsigned doubles in C++)
     */
    GraphNode(unsigned int resolution, double scale);

    /**
     * Create a GraphNode with a given resolution and parent
     * @param resolution the length/width of this graph node in units of number of nodes
     * (ie. this graph node will contain `resolution^2` nodes)
     * @param parent the parent node of this node
     */
    GraphNode(unsigned int resolution, GraphNode* parent);

    /**
     * Gets the coordinates of a given node below this one
     * Throws a NodeNotFoundException if given node is not found below this one
     * @param node the node to get the coordinates of
     * @return
     * - the coordinates of the given node if the the node was found
     * - throws a NodeNotFoundException if the given node was not found
     */
    Coordinates getCoordinatesOfNode(Node* node);

    // TODO: Do we need this function? Is it crap?
    /**
     * Change the resolution of a given sub-node
     * @param node the sub-node to increase the resolution of
     * @param resolution the new resolution of the sub-node
     */
    void changeResolutionOfNode(Node *node, unsigned int resolution);

    /**
     * Changes the resolution of the closest node to the given coordinates
     * @param coordinates TODO
     */
    void changeResolutionOfClosestNode(Coordinates coordinates,
                                       unsigned int resolution);

    /**
     * This is thrown when a given node cannot be found beneath this node
     */
    class NodeNotFoundException : public std::runtime_error {
    public:
        explicit NodeNotFoundException(const char* m) : std::runtime_error(m) {}
    };

    /**
     * Get the resolution for this graph node
     * @return the sqrt of the number of sub-nodes in this graph node
     * (guaranteed to be a whole number, as each graph node is a square)
     */
    int getResolution();

    // TODO: Better comment
    /**
     * Get the subnodes for this graph node
     * @return the subnodes for this graph node
     */
    std::vector<std::vector<Node*>> getSubNodes();


private:
    // the length/width of this graph node in units of number of nodes
    // (ie. this graph node will contain `resolution^2` nodes)
    unsigned int resolution;

    // the length/width of this graph node in some arbitrary unit of measurement
    double scale;

    // TODO: We should *NOT* be using raw pointers here
    // Nodes located below this one
    std::vector<std::vector<Node*>> subNodes;

    // TODO: This should *NOT* be a raw pointer
    // The possible parent of this node
    GraphNode* parent;

    /**
     * Initializes subNodes to a 2D vector of size `resolution x resolution`
     * to RealNodes with this node as their parent
     */
    void initSubNodes();
};


#endif //THUNDERBOTS_NAVIGATOR_GRAPHNODE_H
