#ifndef THUNDERBOTS_NAVIGATOR_NODE_H
#define THUNDERBOTS_NAVIGATOR_NODE_H

// TODO: naming conventions? do we use snake case for variables? Fix!

#include <vector>
#include <cmath>
// TODO: C++ 17 has `std::optional`. We should use it if possible (instead of `boost::optional`)
#include <boost/optional.hpp>

struct Coordinates {
    double x;
    double y;
};
// TODO: Should this functions be within `Coordinates`? Should `Coordiantes` be a class?
inline double distance(Coordinates c1, Coordinates c2) {
    double dx = c1.x - c2.x;
    double dy = c1.y - c2.y;
    return sqrt(pow(dx,2) + pow(dy,2));
}

// We need to forward declare this here so that we can use it as a return type in the "Node" class
class RealNode;

class Node {
public:
    /**
     * Get the node closest to the given coordinates at, or below, this node
     * @param coordinates the coordinates that we're looking for a node at
     * @return a reference to the node closest to the given coordinates
     */
    virtual boost::optional<RealNode*> getClosestNodeToCoordinates(Coordinates coordinates) = 0;

    // TODO: Better doc comment here
    // TODO: Better function name
    /**
     * Gets the closest node at or below this node that passes the given function
     * @param coordinates the coordinates to find a node relative to
     * @param filter a function that takes a node, and returns if it passes
     * @return
     */
    virtual boost::optional<RealNode*> getClosestNodeToCoordinatesThatPassesFilter(
            Coordinates coordinates,
            const std::function<bool(Node &)> &filter) = 0;

    // TODO: We call this function a *LOT*, so some caching here could yield big improvements
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
};

#endif //THUNDERBOTS_NAVIGATOR_NODE_H
