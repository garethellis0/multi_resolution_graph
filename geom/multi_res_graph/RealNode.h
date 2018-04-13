#ifndef THUNDERBOTS_NAVIGATOR_REALNODE_H
#define THUNDERBOTS_NAVIGATOR_REALNODE_H

#include "geom/multi_res_graph/Node.h"
#include "geom/multi_res_graph/GraphNode.h"

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

    std::vector<RealNode<T>*> getAllNodesThatPassFilter(
            const std::function<bool(Node<T> &)> &filter,
            bool parent_must_pass_filter = false,
            bool search_parent = true) override;

    virtual std::vector<RealNode<T>*> getAllSubNodes();

    Coordinates getCoordinates() override;

    double getScale() override;

    Node<T>* clone(GraphNode<T>* parent) const override;

    /**
     * Delete the assignment operator
     */
    RealNode& operator=(const RealNode&) = delete;

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


    // TODO: This is a likely spot to start for performance improvements
    // TODO: Should not be using raw pointers here
    /**
     * Get the neighbouring nodes to this node
     * @return a vector of all neighbouring nodes
     */
    std::vector<RealNode<T>*> getNeighbours();

    // TODO: Add unit test to make sure that this is returning the right value
    /**
     * Converts this RealNode into a GraphNode
     * Note: Will invalidate any pointers to this Node
     * @param resolution the resolution of the new GraphNode
     */
    Node<T> * convertToGraphNode(unsigned int resolution);

    // TODO: Better comment? Bit hard, since it's so generic
    // TODO: YOU ARE HERE - make this return by reference and remove `setContainedValue` below
    /**
     * Gets the value contained by this node
     * @return a reference to the object contained by this node
     */
    T& containedValue();

private:
    /**
     * Make the copy constructor private
     */
    RealNode(const RealNode&) = default;

    // TODO: This should *NOT* be a raw pointer
    // TODO: Better comment here?
    // The parent "graph" node to this node
    GraphNode<T>* parent;

    // The value this node contains
    T contained_value;

    // TODO: Is there a better way to do this then having a separate bool and Coordinates?
    // Whether or not the currently cached coordinates are valid
    bool have_cached_coordinates;

    // The cached coordinates of this node
    Coordinates cached_coordinates;
};

#include "geom/multi_res_graph/RealNode.tpp"


#endif //THUNDERBOTS_NAVIGATOR_REALNODE_H
