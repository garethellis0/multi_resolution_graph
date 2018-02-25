#ifndef THUNDERBOTS_NAVIGATOR_AREA_H
#define THUNDERBOTS_NAVIGATOR_AREA_H

#include "Node.h"

// TODO: Should we namespace this?
// TODO: We may want to replace this with a Thunderbots Geometry class at some point
template <typename T>
class Area {
public:
    // TODO: Can we come up with a more generic impl. for this,
    // TODO: so that we can reduce the amount of work required to create a new Area subclass?
    /**
     * Checks if this area overlaps the given node
     * @param node the node we're checking if this are overlaps
     * @return
     *      `true` if this area *DOES* overlap the given node
     *      `false` if this are *DOES NOT* overlap the given node
     */
    virtual bool overlapsNode(Node<T>& node) = 0;

    /**
     * Clone this Area Object
     * @return a pointer to a clone of this Area object
     */
    virtual Area* clone() const = 0;
};

#endif //THUNDERBOTS_NAVIGATOR_AREA_H
