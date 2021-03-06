#pragma once

// C++ STD Includes
#include <vector>
#include <memory>
#include <cmath>
#include <functional>

namespace multi_resolution_graph {
    struct Coordinates {
        double x;
        double y;
    };

    // TODO: Should this functions be within `Coordinates`? Should `Coordinates` be a class?
    inline double distance(Coordinates c1, Coordinates c2) {
        double dx = c1.x - c2.x;
        double dy = c1.y - c2.y;
        return sqrt(pow(dx, 2) + pow(dy, 2));
    }

    bool inline operator==(const Coordinates &c1, const Coordinates &c2) {
        return c1.x == c2.x && c1.y == c2.y;
    }

    // We need to forward declare these here so that we can use it in
    // in the "Node" class
    template<typename T>
    class RealNode;
    template<typename T>
    class Area;

    // TODO: Really detailed comment explaining what exactly this class is
    template<typename T>
    class Node {
    public:
        // TODO: Should we even use `std::optional` here or is just an empty shared_ptr sufficient
        /**
         * Get the node closest to the given coordinates at, or below, this node
         * @param coordinates the coordinates that we're looking for a node at
         * @return a reference to the node closest to the given coordinates
         */
        virtual std::optional<std::shared_ptr<RealNode<T>>>
        getClosestNodeToCoordinates(Coordinates coordinates) = 0;

        // TODO: Better doc comment here
        // TODO: Better function name
        /**
         * Gets the closest node that passes the given function
         * @param coordinates the coordinates to find a node relative to
         * @param filter a function that takes a node, and returns if it passes
         * @param search_parent whether or not to try searching through this nodes parent node
         * (this is used to prevent loops when travelling all the way up the tree and back down)
         * @return a possible matching node
         */
        virtual std::optional<std::shared_ptr<RealNode<T>>>
        getClosestNodeToCoordinatesThatPassesFilter(
                Coordinates coordinates, const std::function<bool(Node<T> &)> &filter,
                bool search_parent) = 0;

        /**
         * Gets all the nodes that passes the given function
         * @param filter a function that takes a node, and returns if it passes
         * TODO: This is a crappy @param... hard to understand
         * @param parent_must_pass_filter dictates whether the parents of a node must pass the filter
         * in order for the child to also pass the filter (ex. in the case of area overlap)
         * @param search_parent whether or not to try searching through this nodes parent node
         * (this is used to prevent loops when travelling all the way up the tree and back down)
         * @return a vector of matching nodes
         */
        virtual std::vector<std::shared_ptr<RealNode<T>>>
        getAllNodesThatPassFilter(const std::function<bool(Node<T> &)> &filter,
                                  bool parent_must_pass_filter,
                                  bool search_parent) = 0;

        // TODO: `area` should really be const here...
        /**
         * Gets all RealNodes in a given area
         *
         * @param area TODO?
         *
         * @return all RealNodes in the given Area
         */
        std::vector<std::shared_ptr<RealNode<T>>>
        getAllNodesInArea(Area<T>& area);

        /**
         * Gets the coordinates for this node
         * @return the coordinates for this node
         */
        virtual Coordinates getCoordinates() = 0;

        /**
         * Gets the scale of this node
         * @return the length/width of this node
         */
        virtual double getScale() = 0;

        // TODO: Unit test this
        // TODO: The function name is a bit misleading, see description of what function actually does
        /**
         * Gets all the RealNode's below or at this node
         * @return a vector of all RealNode's at or below this one
         */
        virtual std::vector<std::shared_ptr<RealNode<T>>> getAllSubNodes() = 0;
    };

}

#include "multi_resolution_graph/Node.tpp"
