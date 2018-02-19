#ifndef THUNDERBOTS_NAVIGATOR_REALNODE_IMPL_H
#define THUNDERBOTS_NAVIGATOR_REALNODE_IMPL_H

#include <RealNode.h>

template <typename T>
RealNode<T>::RealNode(GraphNode<T>* parent):
        parent(parent)
{
}

template <typename T>
boost::optional<RealNode<T>*> RealNode<T>::getClosestNodeToCoordinates(Coordinates coordinates) {
    // We have no sub-nodes, so the closest node is this node
    return this;
}

template <typename T>
std::vector<RealNode<T>*> RealNode<T>::getNeighbours() {
    // Find the closest nodes above, below, to the left, and to the right of this node
    std::vector<std::function<bool(Node<T>&)>> filters = {
            [&](Node<T> &n) { return n.getCoordinates().y < this->getCoordinates().y; },
            [&](Node<T> &n) { return n.getCoordinates().y > this->getCoordinates().y; },
            [&](Node<T> &n) { return n.getCoordinates().x < this->getCoordinates().x; },
            [&](Node<T> &n) { return n.getCoordinates().x > this->getCoordinates().x; }
    };

    std::vector<RealNode<T>*> neighbours;
    for (auto& filter : filters){
        boost::optional<RealNode<T>*> node = parent->getClosestNodeToCoordinatesThatPassesFilter(
                this->getCoordinates(), filter,
                true);
        if (node) {
            RealNode<T>* TODO_delete_me = *node;
            neighbours.push_back(*node);
        }
    }

    return neighbours;
}

template <typename T>
Coordinates RealNode<T>::getCoordinates() {
    return parent->getCoordinatesOfNode(this);
}

template <typename T>
boost::optional<RealNode<T>*> RealNode<T>::getClosestNodeToCoordinatesThatPassesFilter(
        Coordinates coordinates,
        const std::function<bool(Node<T> &)> &filter,
        bool search_parent) {
    if (search_parent && parent != nullptr) {
        return parent->getClosestNodeToCoordinatesThatPassesFilter(coordinates, filter);
    // TODO: Descriptive comment here
    } else if (filter(*this)){
        return this;
    }

    return boost::optional<RealNode<T>*>{};
}

template<typename T>
std::vector<RealNode<T> *> RealNode<T>::getAllNodesThatPassFilter(
        const std::function<bool(Node<T> &)> &filter, bool search_parent) {
    // If this node passes the given filter, the return a list with just
    // this node, otherwise return an empty list
    if (filter(*this)){
        return {this};
    } else {
        return {};
    }
}

template <typename T>
double RealNode<T>::getScale() {
    return parent->getResolution()/parent->getScale();
}

template <typename T>
void RealNode<T>::convertToGraphNode(unsigned int resolution) {
    parent->changeResolutionOfNode(this, resolution);
}

template <typename T>
T RealNode<T>::getContainedValue() {
    return contained_value;
}

template <typename T>
void RealNode<T>::setContainedValue(T val) {
    this->contained_value = val;
}


#endif // THUNDERBOTS_NAVIGATOR_REALNODE_IMPL_H
