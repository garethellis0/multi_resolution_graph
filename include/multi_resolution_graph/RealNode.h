#pragma once

#include "Node.h"
#include "GraphNode.h"


namespace multi_resolution_graph {
    template <typename T>
    class GraphNode;

    // TODO: Really detailed comment explaining what exactly this class is
    template<typename T>
    class RealNode : public Node<T>, public std::enable_shared_from_this<RealNode<T>> {
    public:
        boost::optional<std::shared_ptr<RealNode<T>>>
        getClosestNodeToCoordinates(Coordinates coordinates) override;

        boost::optional<std::shared_ptr<RealNode<T>>>
        getClosestNodeToCoordinatesThatPassesFilter(
                Coordinates coordinates,
                const std::function<bool(Node < T > &)> &filter,
                bool search_parent = true) override;

        std::vector<std::shared_ptr<RealNode<T>>> getAllNodesThatPassFilter(
                const std::function<bool(Node < T > &)> &filter,
                bool parent_must_pass_filter = false,
                bool search_parent = true) override;

        virtual std::vector<std::shared_ptr<RealNode<T>>> getAllSubNodes();

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
        explicit RealNode(GraphNode<T> *parent);


        // TODO: This is a likely spot to start for performance improvements
        // TODO: Should not be using raw pointers here
        /**
         * Get the neighbouring nodes to this node
         * @return a vector of all neighbouring nodes
         */
        std::vector<std::shared_ptr<RealNode<T>>> getNeighbours();

        // TODO: Add unit test to make sure that this is returning the right value
        /**
         * Converts this RealNode into a GraphNode
         * Note: Will invalidate any pointers to this Node
         * @param resolution the resolution of the new GraphNode
         */
        std::shared_ptr<Node < T>> convertToGraphNode(
        unsigned int resolution
        );

        // TODO: Better comment? Bit hard, since it's so generic
        // TODO: YOU ARE HERE - make this return by reference and remove `setContainedValue` below
        /**
         * Gets the value contained by this node
         * @return a reference to the object contained by this node
         */
        T &containedValue();

    private:
        // TODO: Better comment here?
        // We use a raw pointer here so that we may initialise it in the GraphNode
        // constructor without having to call `share_from_this`
        // This is acceptable because each parent may be said to own it's children
        // and thus we ensure this node will be properly deallocated
        // The parent "graph" node to this node
        GraphNode<T> *parent;

        // The value this node contains
        T contained_value;

        // TODO: Is there a better way to do this then having a separate bool and Coordinates?
        // Whether or not the currently cached coordinates are valid
        bool have_cached_coordinates;

        // The cached coordinates of this node
        Coordinates cached_coordinates;
    };
}

#include "RealNode.tpp"

