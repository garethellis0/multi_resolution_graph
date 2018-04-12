#ifndef THUNDERBOTS_NAVIGATOR_NODE_H
#define THUNDERBOTS_NAVIGATOR_NODE_H

// TODO: naming conventions? do we use snake case for variables? Fix!
// TODO: Mathew - Yes use snake case

// C++ STD Includes
#include <vector>
#include <cmath>

// Boost Includes
// TODO: C++ 17 has `std::optional`. We should use it if possible (instead of `boost::optional`)
#include <boost/optional.hpp>

struct Coordinates {
    double x;
    double y;
};
// TODO: Should this functions be within `Coordinates`? Should `Coordinates` be a class?
inline double distance(Coordinates c1, Coordinates c2) {
    double dx = c1.x - c2.x;
    double dy = c1.y - c2.y;
    return sqrt(pow(dx,2) + pow(dy,2));
}
bool inline operator==(const Coordinates& c1, const Coordinates& c2){
    return c1.x == c2.x && c1.y == c2.y;
}

// We need to forward declare this here so that we can use it as a return type in the "Node" class
template <typename T>
class RealNode;

// TODO: Really detailed comment explaining what exactly this class is
template <typename T>
class Node {
public:
    /**
     * Get the node closest to the given coordinates at, or below, this node
     * @param coordinates the coordinates that we're looking for a node at
     * @return a reference to the node closest to the given coordinates
     */
    virtual boost::optional<RealNode<T>*> getClosestNodeToCoordinates(Coordinates coordinates) = 0;

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
    virtual boost::optional<RealNode<T> *>
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
    virtual std::vector<RealNode<T> *>
    getAllNodesThatPassFilter(const std::function<bool(Node<T> &)> &filter,
                              bool parent_must_pass_filter,
                              bool search_parent) = 0;

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
    virtual std::vector<RealNode<T>*> getAllSubNodes() = 0;
};

#endif //THUNDERBOTS_NAVIGATOR_NODE_H
