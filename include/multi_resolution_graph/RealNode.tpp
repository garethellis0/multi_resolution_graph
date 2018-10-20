#ifndef THUNDERBOTS_NAVIGATOR_REALNODE_IMPL_H
#define THUNDERBOTS_NAVIGATOR_REALNODE_IMPL_H

#include "RealNode.h"

template <typename T>
RealNode<T>::RealNode(GraphNode<T>* parent):
        parent(parent),
        have_cached_coordinates(false)
{
}

template <typename T>
boost::optional<std::shared_ptr<RealNode<T>>> RealNode<T>::getClosestNodeToCoordinates(Coordinates coordinates) {
    // We have no sub-nodes, so the closest node is this node
    return this->shared_from_this();
}

template <typename T>
std::vector<std::shared_ptr<RealNode<T>>> RealNode<T>::getNeighbours() {
    // Find the closest nodes above, below, to the left, and to the right of this node
    std::vector<std::function<bool(Node<T>&)>> filters = {
            // Filter to find the node below this one
            [&](Node<T> &n) { return n.getCoordinates().y < this->getCoordinates().y; },
            // Filter to find the node above this one
            [&](Node<T> &n) { return n.getCoordinates().y > this->getCoordinates().y; },
            // Filter to find the node to the left of this one
            [&](Node<T> &n) { return n.getCoordinates().x < this->getCoordinates().x; },
            // Filter to find the node to the right of this one
            [&](Node<T> &n) { return n.getCoordinates().x > this->getCoordinates().x; }
    };

    std::vector<std::shared_ptr<RealNode<T>>> neighbours;
    for (auto& filter : filters){
        boost::optional<std::shared_ptr<RealNode<T>>> node =
                parent->getClosestNodeToCoordinatesThatPassesFilter(
                    this->getCoordinates(), filter,
                    true);
        if (node) {
            neighbours.push_back(*node);
        }
    }

    return neighbours;
}

template <typename T>
Coordinates RealNode<T>::getCoordinates() {
    // Check if we have the coordinates of this node cached
    if (have_cached_coordinates) {
        return cached_coordinates;
    } else {
        // Get the coordinates of this node
        cached_coordinates = parent->getCoordinatesOfNode(this->shared_from_this());
        have_cached_coordinates = true;
        return cached_coordinates;
    }
}

template <typename T>
boost::optional<std::shared_ptr<RealNode<T>>> RealNode<T>::getClosestNodeToCoordinatesThatPassesFilter(
        Coordinates coordinates,
        const std::function<bool(Node<T> &)> &filter,
        bool search_parent) {
    if (search_parent && parent != nullptr) {
        return parent->getClosestNodeToCoordinatesThatPassesFilter(coordinates, filter);
    // TODO: Descriptive comment here
    } else if (filter(*this)){
        return this->shared_from_this();
    }

    return boost::optional<std::shared_ptr<RealNode<T>>>{};
}

template<typename T>
std::vector<std::shared_ptr<RealNode<T>>> RealNode<T>::getAllNodesThatPassFilter(
        const std::function<bool(Node<T> &)> &filter,
        bool parent_must_pass_filter,
        bool search_parent) {
    // If this node passes the given filter, the return a list with just
    // this node, otherwise return an empty list
    if (filter(*this)){
        return {this->shared_from_this()};
    } else {
        return {};
    }
}

template<typename T>
std::vector<std::shared_ptr<RealNode<T>>> RealNode<T>::getAllSubNodes() {
    // Since this is a RealNode, the only node at or below it is itself
    return {this->shared_from_this()};
}


template <typename T>
double RealNode<T>::getScale() {
    return parent->getScale()/parent->getResolution();
}

template <typename T>
std::shared_ptr<Node<T>> RealNode<T>::convertToGraphNode(unsigned int resolution) {
    return parent->changeResolutionOfNode(this->shared_from_this(), resolution);
}

template <typename T>
T& RealNode<T>::containedValue() {
    return contained_value;
}

#endif // THUNDERBOTS_NAVIGATOR_REALNODE_IMPL_H
