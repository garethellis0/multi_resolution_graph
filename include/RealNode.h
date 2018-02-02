#ifndef THUNDERBOTS_NAVIGATOR_REALNODE_H
#define THUNDERBOTS_NAVIGATOR_REALNODE_H

#include <Node.h>
#include <GraphNode.h>

template <typename T>
class GraphNode;

// TODO: Really detailed comment explaining what exactly this class is
template <typename T>
class RealNode : public Node<T> {
public:
    boost::optional<RealNode<T>*> getClosestNodeToCoordinates(Coordinates coordinates) override;

    boost::optional<RealNode<T>*> getClosestNodeToCoordinatesThatPassesFilter(
            Coordinates coordinates,
            const std::function<bool(Node<T>&)>& filter,
            bool search_parent = true) override;

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
    explicit RealNode(GraphNode<T>* parent);


    // TODO: Should not be using raw pointers here
    /**
     * Get the neighbouring nodes to this node
     * @return a vector of all neighbouring nodes
     */
    std::vector<RealNode<T>*> getNeighbours();

    /**
     * Converts this RealNode into a GraphNode
     * Note: Will invalidate any pointers to this Node
     * @param resolution the resolution of the new GraphNode
     */
    void convertToGraphNode(unsigned int resolution);

    // TODO: Better comment? Bit hard, since it's so generic
    /**
     * Gets the value contained by this node
     * @return the values contained by this node
     */
    T getContainedValue();

    // TODO: Better comment? Bit hard, since it's so generic
    /**
     * Sets the value contained by this node
     * @param val the value to be contained by this node
     */
    void setContainedValue(T val);

private:
    // TODO: This should *NOT* be a raw pointer
    // TODO: Better comment here?
    // The parent "graph" node to this node
    GraphNode<T>* parent;

    // TODO: Add getter/setters for this
    // The value this node contains
    T contained_value;
};

#include <RealNode.tpp>


#endif //THUNDERBOTS_NAVIGATOR_REALNODE_H
