#ifndef THUNDERBOTS_NAVIGATOR_NODE_H
#define THUNDERBOTS_NAVIGATOR_NODE_H

// TODO: naming conventions? do we use snake case for variables? Fix!

#include <vector>
#include <RealNode.h>
#include <cmath>
// TODO: C++ 17 has `std::optional`. We should use it if possible (instead of `boost::optional`)
#include <boost/optional.hpp>

struct Coordinates {
    double x;
    double y;
};
// TODO: Should this functions be within `Coordinates`? Should `Coordiantes` be a class?
double distance(Coordinates c1, Coordinates c2) {
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
    virtual RealNode& getClosestNodeToCoordinates(Coordinates coordinates) = 0;

    /**
     * Get the closest node to a given set of coordinates that is also
     * above (+y) the given set of coordinates
     * @param coordinates the coordinates to search for a node near
     * @return the node closest to, and above, the given set of coordinates
     */
    virtual boost::optional<RealNode&> getClosestNodeAboveCoordinates(Coordinates coordinates) = 0;

    // TODO: We call this function a *LOT*, so some caching here could yield big improvements
    /**
     * Gets the coordinates for this node
     * @return the coordinates for this node
     */
    virtual Coordinates getCoordinates() = 0;
};

#endif //THUNDERBOTS_NAVIGATOR_NODE_H
