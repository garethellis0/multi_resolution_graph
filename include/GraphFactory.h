#ifndef THUNDERBOTS_NAVIGATOR_GRAPHFACTORY_H
#define THUNDERBOTS_NAVIGATOR_GRAPHFACTORY_H

// Thunderbots Includes
#include <GraphNode.h>
#include <RealNode.h>

template <typename T>
class GraphFactory {
public:
    // TODO: YOU ARE HERE - is this even how you impl. the Factory design pattern?
    /**
     * Sets the minimum resolution for the generated graph at a given point
     * @param coordinates the point at which to set the given resolution
     * @param resolution the minimum resolution this point must be
     */
    void setMinResolutionAtPoint(Coordinates coordinates, double resolution);

    GraphNode<T> createGraph();
private:
};

#endif //THUNDERBOTS_NAVIGATOR_GRAPHFACTORY_H
